mod example;

fn main() {
    let obj = example::oop::Animal::new("Dog");
    println!("{}", obj.speak());
}
