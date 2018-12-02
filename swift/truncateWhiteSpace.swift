// condensed white space
// write a function that returns a string with any consecutive spaces
// replaced with a single space.

func truncateWhiteSpace(_ input: String) -> String {
    var result = Array(input.characters)
    var length = result.count
    var i = 0
    while i <= length {
        if result[i] == " " && result[i+1] == " " {
            result.remove(at: i+1)
            length -= 1
            continue
        }
        i += 1
    }

    return String(result)
}

// for-in
func truncateWhiteSpace2(_ input: String) -> String {
    var spaceDetected = false
    var result = String()
    for letter in input.characters {
        if letter == " " {
            if spaceDetected { continue }
            spaceDetected = true
        } else {
            spaceDetected = false
        }
        result.append(letter)
    }
    return String(result)
}

import Foundation

// regex
func truncateWhilteSpace3(_ input: String) -> String {
    return input.replacingOccurrences(of: " +", with: " ", options: .regularExpression, range: nil)
}

