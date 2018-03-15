// do two strings contain the same characters?
// write a function that accepts two String parameters, and
// returns true if they contain the same chracters in any order.

// case sensitive
// can be superset - subset equality, or exact
// brute force - O(n^2)
func isSameCharacters(_ input: String, _ check: String) -> Bool {
    var inputarray = array(input.characters)
    var checkarray = array(check.characters)

    for inputChar in inputArray {
        for checkChar in checkArray {
            if inputChar == checkChar {
                if let index = checkArray.index(of: checkChar) {
                    checkArray.remove(at: index)
                }
            }
        }
    }
    if checkArray.count == 0 {
        return true
    }
    return false
}

// tighten up above
func isSameCharacters2(_ input: String, check: String) -> Bool {
    var inputArray = Array(input.characters)
    var checkArray = Array(check.characters)
    
    for inputChar in inputArray {
        if let index = inputArray.index(of: inputChar) {
            checkArray.remove(at: index)
        }
    }
    return checkArray.count == 0
}

// case sensitive
// exact same character equality
// sortedArrays - O(n log n)
func isSameCharactersSorted(_ input: String, _ check: String) -> Bool {
    let inputArray = Array(input.characters)
    let checkArray = Array(check.characters)
    
    return inputArray.sorted() == checkArray.sorted()
}

// Swift 4
func isSameCharactersSorted2(_ first: String, _ second: String) -> Bool {
    return first.sorted() == second.sorted()
