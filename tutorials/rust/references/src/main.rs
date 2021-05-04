// - An immutable variable can't be made mutable and changed
// - A scope can maximum change one mutable variable
// - Rust detects and prevents dangling references during compile time


fn main() {

    let s1 = String::from("Hello");

    let len = calculate_length(&s1);

    println!("The length of {} is {}", s1, len);

    let mut s2 = String::from("Hello again");

    // This won't work since s2 is immutable
    // change(&s2);

    change(&mut s2);

    println!("S2: {}", s2)
}

fn calculate_length(s: &String) -> usize { // s is a reference to a String
    s.len()

    // Here, s goes out of scope. But because it does not have ownership of what
    // it refers to, nothing happens.
}

// This won't work since the string needs to be mut if it's to change the reference
// fn change(s: &String) {
fn change(s: &mut String) {
    s.push_str(", mjao");
}