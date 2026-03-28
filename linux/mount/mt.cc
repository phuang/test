#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>

class MountWatcher {
private:
    int fd;
    struct pollfd pfd;
    
    // Cache of the mount table: Map<Mount_ID, Full_String_Line>
    std::unordered_map<int, std::string> previous_mounts;

    // Helper: Reads the file and parses it into a map
    std::unordered_map<int, std::string> read_mount_table() {
        std::unordered_map<int, std::string> current_mounts;
        
        // 1. Rewind to the beginning to read fresh data and clear poll()
        lseek(fd, 0, SEEK_SET);

        // 2. Slurp the entire file into a std::string
        std::string content;
        char buffer[8192]; // Large chunk size for speed
        ssize_t bytes_read;
        while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
            content.append(buffer, bytes_read);
        }

        // 3. Parse line by line
        std::istringstream file_stream(content);
        std::string line;
        while (std::getline(file_stream, line)) {
            if (line.empty()) continue;

            // Extract the very first token (the Mount ID)
            int mount_id;
            std::istringstream line_stream(line);
            if (line_stream >> mount_id) {
                current_mounts[mount_id] = line;
            }
        }
        
        return current_mounts;
    }

public:
    MountWatcher() {
        fd = open("/proc/self/mountinfo", O_RDONLY);
        if (fd != -1) {
            pfd.fd = fd;
            pfd.events = POLLPRI | POLLERR;
            // Initialize the baseline state immediately
            previous_mounts = read_mount_table();
        }
    }

    ~MountWatcher() {
        if (fd != -1) close(fd);
    }

    bool is_valid() const { return fd != -1; }

    bool wait_for_change() {
        if (fd == -1) return false;
        
        // Block indefinitely
        int ret = poll(&pfd, 1, -1);
        return (ret > 0 && (pfd.revents & (POLLERR | POLLPRI)));
    }

    void process_and_print_diff() {
        if (fd == -1) return;

        // Get the new state
        auto current_mounts = read_mount_table();

        // Check for ADDED mounts (In current, but not in previous)
        for (const auto& [id, line] : current_mounts) {
            if (previous_mounts.find(id) == previous_mounts.end()) {
                std::cout << "[+] MOUNT ADDED:   " << line << "\n";
            }
        }

        // Check for REMOVED mounts (In previous, but not in current)
        for (const auto& [id, line] : previous_mounts) {
            if (current_mounts.find(id) == current_mounts.end()) {
                std::cout << "[-] MOUNT REMOVED: " << line << "\n";
            }
        }

        // Update the baseline for the next event
        previous_mounts = std::move(current_mounts);
    }
};

int main() {
    MountWatcher watcher;
    if (!watcher.is_valid()) {
        std::cerr << "Failed to open mountinfo\n";
        return 1;
    }

    std::cout << "[SYSTEM] Baseline established. Watching for changes...\n";
    
    while (true) {
        if (watcher.wait_for_change()) {
            std::cout << "\n[EVENT] Mount table changed!\n";
            watcher.process_and_print_diff();
        } else {
            std::cerr << "[ERROR] Polling failed.\n";
            break;
        }
    }
    return 0;
}