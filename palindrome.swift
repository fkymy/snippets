// is a string a palindrome?
// write a function that accepts a String as its only parameter, and
// returns true if the string reads the same when reversed.

func isPalindrome(_ input: String) -> Bool {
    let reversedInput = String(input.characters.reversed())
    if input == reversedInput {
        return true
    }
    return false
}

func isPalindrome2(_ input: String) -> Bool {
    return String(input.characters.reversed()) == input
}

func isPalindromeCaseInsensitive(_ input: String) -> Bool {
    return String(input.lowercased().characters.reversed()) == input.lowercased()
}

print(isPalindrome("rotator"))     // true
print(isPalindrome("palindrome"))  // false
print(isPalindrome2("rotator"))     // true
print(isPalindrome2("roTator"))     // false - not case sensitive
print(isPalindrome2("palindrome"))  // false
print(isPalindromeCaseInsensitive("rotator"))     // true
print(isPalindromeCaseInsensitive("roTator"))     // true - case sensitive
print(isPalindromeCaseInsensitive("palindrome"))  // false
print(isPalindromeCaseInsensitive("12321"))  // false

// Swift 4, as String.chracters is deprecated
func isPalindrome3(_ word: String) -> Bool {
    return word == String(word.reversed())
}

func isPalindromeCaseInsensitive2(_ word: String) -> Bool {
    return word.lowercased() == String(word.lowercased().reversed())
}

print(isPalindrome3("rotator"))     // true
print(isPalindrome3("roTator"))     // false - not case sensitive
print(isPalindrome3("palindrome"))  // false
print(isPalindromeCaseInsensitive2("rotator"))     // true
print(isPalindromeCaseInsensitive2("roTator"))     // true - case sensitive
print(isPalindromeCaseInsensitive2("palindrome"))  // false
