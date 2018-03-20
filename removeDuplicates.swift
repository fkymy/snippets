// remove duplicate letters from a string.
// write a function that accepts a string as its input and
// returns a string with the duplicate letters removed.

import Foundation

// consecutive duplicate characters
func removeConsecutiveDuplicatesFrom(_ input: String) -> String {
    var result = Array(input.characters)
    var i = 0
    var length = result.count
    
    while i <= length {
        if result[i] == result[i+1] {
            result.remove(at: i + 1)
            length -= 1
        }
        i += 1
    }
    
    return String(result)
}

// remove all following characters that are duplicate
// additive array
func removeAllDuplicatesFrom(_ input: String) -> String {
    var usedCharacters = [Character]()
    for letter in input.characters {
        if !usedCharacters.contains(letter) {
            usedCharacters.append(letter)
        }
    }
    return String(usedCharacters)
}

// functional approach using dict
// (poor performance)
func removeAllDuplicatesFrom2(_ input: String) -> String {
    var usedCharacters: [Character: Bool] = [:]
    let result = input.characters.filter {
        usedCharacters.updateValue(true, forKey: $0) == nil
    }
    return String(result)
}

// NSOrderedSet
func removeAllDuplicatesFrom3(_ input: String) -> String {
    let array = input.characters.map { String($0) }
    let set = NSOrderedSet(array: array)
    let letters = Array(set) as! Array<String>
    return letters.joined()
}

// Swift 4
func removeAllDuplicatesFrom4(_ input: String) -> String {
    var usedChar = String()
    input.forEach { char in
        if !usedChar.contains(char) { usedChar.append(char)}
    }
    return usedChar
}
