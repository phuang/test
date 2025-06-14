mod oop;
mod server;

fn main() {
    // let mut server = server::Server::new("0.0.0.0:7878");
    // if let Err(e) = server.run() {
    //     eprintln!("Server error: {}", e);
    // }
    let obj = oop::Animal::new("Dog");
    println!("{}", obj.speak());
}
