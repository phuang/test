#![allow(dead_code)]

use mio::net::{TcpListener as MioTcpListener, TcpStream as MioTcpStream};
use mio::{Events, Interest, Poll, Token};
use std::collections::HashMap;
use std::io::{Read, Write};
use std::net::SocketAddr;

pub struct Server {
    address_: String,
    unique_token_: usize,
    clients_: HashMap<Token, MioTcpStream>,
    addr_map_: HashMap<Token, SocketAddr>,
    buffers_: HashMap<Token, String>,
}

impl Server {
    pub fn new(address: &str) -> Self {
        Server {
            address_: address.to_string(),
            unique_token_: 1,
            clients_: HashMap::new(),
            addr_map_: HashMap::new(),
            buffers_: HashMap::new(),
        }
    }

    pub fn run(&mut self) -> std::io::Result<()> {
        let mut poll = Poll::new()?;
        let mut events = Events::with_capacity(128);

        let addr = self.address_.parse().unwrap();
        let mut listener = MioTcpListener::bind(addr)?;
        poll.registry()
            .register(&mut listener, Token(0), Interest::READABLE)?;

        println!("Server listening on {}", self.address_);

        loop {
            poll.poll(&mut events, None)?;
            println!("Events: {}", events.iter().count());
            for event in events.iter() {
                let token = event.token();
                if token == Token(0) {
                    // Accept new connections
                    while let Ok((mut stream, addr)) = listener.accept() {
                        let token = Token(self.unique_token_);
                        self.unique_token_ += 1;
                        poll.registry()
                            .register(&mut stream, token, Interest::READABLE)?;
                        self.clients_.insert(token, stream);
                        self.addr_map_.insert(token, addr);
                        self.buffers_.insert(token, String::new());
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
        if !self.clients_.contains_key(&token) {
            return Ok(());
        }

        let stream = self.clients_.get_mut(&token).unwrap();
        let mut buffer = [0; 1024];
        let mut remove_client = false;

        match stream.read(&mut buffer) {
            Ok(0) => {
                if let Some(addr) = self.addr_map_.remove(&token) {
                    println!("Client disconnected: {}", addr);
                }
                remove_client = true;
                self.buffers_.remove(&token);
            }
            Ok(n) => {
                let data = String::from_utf8_lossy(&buffer[..n]).to_string();
                println!("Received from {}: {}", self.addr_map_[&token], data);
                self.buffers_.entry(token).or_default().push_str(&data);

                if data.trim() == "exit" {
                    println!("Closing connection to {}", self.addr_map_[&token]);
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
            self.clients_.remove(&token);
        }
        Ok(())
    }
}