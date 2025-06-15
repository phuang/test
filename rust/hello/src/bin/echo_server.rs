use hello::example::server;

fn main() {
    let mut server = server::Server::new("127.0.0.1:7878");
    match server.run() {
        Ok(_) => println!("Server stopped successfully."),
        Err(e) => eprintln!("Server error: {}", e),
    };
}