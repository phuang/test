use hello::example::server;

fn main() {
    let mut server = server::Server::new("127.0.0.1:7878");
    server.run().unwrap_or_else(|e| {
        eprintln!("Server error: {}", e);
    });
}