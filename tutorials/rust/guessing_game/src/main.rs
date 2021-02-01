use std::io;

fn main() {
    println!("Input a number between 0-100:");

    let mut guess = String::new();

    io::stdin()
        .read_line(&mut guess)
        .expect("Failed to read line!");

    println!("You guessed: {}", guess);
}
