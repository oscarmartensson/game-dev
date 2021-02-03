fn main() {

    // Testing if-statements
    let number = 2;

    if number < 5 {
        println!("Condition is true.");
    } else {
        println!("Condition is false.");
    }

    // Testing conditional assignment
    let condition = 5;
    let switch = if condition == 5 { true } else { false };

    println!("Switch is: {}", switch);

    // Testing loop
    let mut counter = 0;
    let result = loop {
        if counter >= 10 {
            break counter * 2;
        }

        counter += 1;
    };

    println!("Result: {}", result);

    // Testing for loops
    let a = [0, 1, 3, 5, 6];
    let mut counter = 0;
    for element in a.iter() {
        println!("Value of element {} is:{}", counter, element);
        counter += 1;
    }
}
