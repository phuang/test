#![allow(dead_code)]

pub struct Object {
    name: String,
}

impl Object {
    pub fn new(name: &str) -> Self {
        Object {
            name: name.to_string(),
        }
    }

    pub fn greet(&self) -> String {
        format!("Hello, {}!", self.name)
    }
}

impl Drop for Object {
    fn drop(&mut self) {
        println!("Object {} is being dropped", self.name);
    }
}
pub struct Animal {
    object: Object,
}
impl Animal {
    pub fn new(name: &str) -> Self {
        Animal {
            object: Object::new(&format!("Animal:{}", name)),
        }
    }

    pub fn speak(&self) -> String {
        format!("{} says hello!", self.object.name)
    }
}