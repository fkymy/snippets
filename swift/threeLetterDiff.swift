// three different letters
// write a function that accepts two strings, and returns true if they are
// identical in length but have no more than three different letters.

func threeLetterDiff(inputA: String, inputB: String) -> Bool {
    if inputA.characters.count != inputB.characters.count { return false }
    var diff = 0
    
    for letter in inputA.characters {
        if !inputB.characters.contains(letter) {
            diff += 1
        }
        if diff > 3 { return false }
    }
    
    return true
}

func threeLetterDiff(_ strOne: String, _ strTwo: String) -> Bool {
    guard strOne.count == strTwo.count else { return false }
    let setOne = Set(strOne)
    return setOne.subtracting(strTwo).count <= 3
}

