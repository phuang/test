use mio::net::{TcpListener as MioTcpListener, TcpStream as MioTcpStream};
use mio::{Events, Interest, Poll, Token};
use std::collections::HashMap;
use std::io::{self, Read, Write};
use std::net::SocketAddr;

struct Server {
    address: String,
    unique_token: usize,
    clients: HashMap<Token, MioTcpStream>,
    addr_map: HashMap<Token, SocketAddr>,
    buffers: HashMap<Token, String>,
}

impl Server {
    fn new(address: &str) -> Self {
        Server {
            address: address.to_string(),
            unique_token: 1,
            clients: HashMap::new(),
            addr_map: HashMap::new(),
            buffers: HashMap::new(),
        }
    }

    fn run(&mut self) -> std::io::Result<()> {
        let mut poll = Poll::new()?;
        let mut events = Events::with_capacity(128);

        let addr = self.address.parse().unwrap();
        let mut listener = MioTcpListener::bind(addr)?;
        poll.registry()
            .register(&mut listener, Token(0), Interest::READABLE)?;

        println!("Server listening on {}", self.address);

        loop {
            poll.poll(&mut events, None)?;
            println!("Events: {}", events.iter().count());
            for event in events.iter() {
                let token = event.token();
                if token == Token(0) {
                    // Accept new connections
                    while let Ok((mut stream, addr)) = listener.accept() {
                        let token = Token(self.unique_token);
                        self.unique_token += 1;
                        poll.registry()
                            .register(&mut stream, token, Interest::READABLE)?;
                        self.clients.insert(token, stream);
                        self.addr_map.insert(token, addr);
                        self.buffers.insert(token, String::new());
                        println!("New connection: {}", addr);
                    }
                } else {
                    self.handle_client_event(
                        token,
                        &mut poll,
                    )?;
                }
            }
        }
    }

    fn handle_client_event(
        &mut self,
        token: Token,
        poll: &mut Poll,
    ) -> std::io::Result<()> {
        if !self.clients.contains_key(&token) {
            return Ok(());
        }

        let stream = self.clients.get_mut(&token).unwrap();
        let mut buffer = [0; 1024];
        let mut remove_client = false;

        match stream.read(&mut buffer) {
            Ok(0) => {
                if let Some(addr) = self.addr_map.remove(&token) {
                    println!("Client disconnected: {}", addr);
                }
                remove_client = true;
                self.buffers.remove(&token);
            }
            Ok(n) => {
                let data = String::from_utf8_lossy(&buffer[..n]).to_string();
                println!("Received from {}: {}", self.addr_map[&token], data);
                self.buffers.entry(token).or_default().push_str(&data);

                if data.trim() == "exit" {
                    println!("Closing connection to {}", self.addr_map[&token]);
                    remove_client = true;
                } else if data.trim() == "shutdown" {
                    println!("Shutdown command received. Stopping server.");
                    std::process::exit(0);
                } else {
                    let response = format!("echo: {}", data);
                    stream.write_all(response.as_bytes())?;
                }
            }
            Err(e) if e.kind() == std::io::ErrorKind::WouldBlock => {}
            Err(e) => {
                eprintln!("Read error: {}", e);
            }
        }
        poll.registry()
            .reregister(stream, token, Interest::READABLE)?;

        if remove_client {
            self.clients.remove(&token);
        }
        Ok(())
    }
}

fn main() {
    let mut server = Server::new("0.0.0.0:7878");
    if let Err(e) = server.run() {
        eprintln!("Server error: {}", e);
    }
    println!("Server stopped.");
    io::stdout().flush().unwrap();
    std::process::exit(0);
}
