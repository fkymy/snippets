// string is rotated.
// write a function that accepts two strings, and
// returns true if one string is a rotation of the other


// classic brute force
func isRotate(string1: String, string2: String) -> Bool {
    var letter = Array(string1.characters)[0]
    var string2_index = 0
    
    if string1.characters.count != string2.character.count {
        return false
    }
    
    for match in string2.characters {
        if letter == match {
            let rotationIndex = string2.index(string2.startIndex, offsetBy: string2_index)
            let firstHalf = string2.substring(to: rotationIndex)
            let secondHalf = string2.substring(from: rotationIndex)
            let potetial_rotate = "\(secondHalf)\(firstHalf)"
            
            if string1 == potential_rotate {
                return true
            }
        }
        string2_index += 1
    }
    return false
}


// Swift 4
func isRotateDoubler2(input: String, rotated: String) -> Bool {
    guard input.count == rotated.count else { return false }
    return (rotated+rotated).contains(input)
}

