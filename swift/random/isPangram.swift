// is the string a pangram
// write a function that returns true if a given string is an english pangram

func isPangram(_ input: String) -> Bool {
    var alphabet = Array("abcdefghijklmnopqrstuvwxyz".characters)
    
    for letter in input.lowercased().characters {
        if let index = alphabet.index(of: letter) {
            alphabet.remove(at: index)
        }
    }
    if alphabet.count == 0 {
        return true
    } else {
        return false
    }
}

// fuzzy?
func isPangram2(_ input: String) -> Bool {
    var isPan = true
    "abcdefghijklmnopqrstuvwxyz".forEach({
        if !input.lowercased().contains($0) { isPan = false }
    })
    return isPan
}

