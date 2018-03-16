// count the characters
// write a function that accepts a string and returns how many times a specific
// character appears, case sensitive.

// classic
func charCount(_ input: String, find: Character) -> Int {
    var count = 0
    for char in input.characters {
        if char == find {
            count += 1
        }
    }
    return count
}

// no for-in or while loop contraint
func charCount2(_ input: String, find: Character) -> Int {
    return input.characters.reduce(0, { count, char in
        char == find ? count + 1 : count
    })
}

// func charCount2(_ input: String, find: Character) -> Int {
//     return input.characters.reduce(0) { $1 == find ? $0 + 1 : $0}
// }

import Foundation

// arithmetic
func charCount3(_ input: String, find: String) -> Int {
    let newInput = input.replacingOccurrences(of: find, with: "")
    return input.characters.count - newInput.characters.count
}

// Swift 4, as String.characters is deprecated
func charCount4(_ input: String, check: Character) -> Int {
    var count = 0
    input.forEach { if $0 == check { count += 1 } }
    return count
}

func charCount5(_ input: String, check: Character) -> Int {
    return input.reduce(0) { $1 == check ? $0 + 1 : $0 }
