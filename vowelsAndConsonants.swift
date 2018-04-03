// vowels and consonants
// write a function that returns a tuple containing the number of vowels and consonants.

func vowelConsonants(_ input: String) -> (vowels: Int, consonants: Int) {
    let vowels = "aeiou".characters
    let consonants = "bcdfghjklmnpqrstvwxyz".characters
    var vowelCount = 0
    var consonantCount = 0
    
    for letter in input.lowercased().characters {
        if consonants.contains(letter) {
            consonantCount += 1
        } else if vowels.contains(letter) {
            vowelCount += 1
        }
    }
    
    return (vowelCount, consonantCount)
}

func vowelConsonants2(_ input: String) -> (vowels: Int, consonants: Int) {
    var vowelCount = 0
    var consonantCount = 0
    input.lowercased().forEach {
        if "bcdfghjklmnpqrstvwxyz".contains($0) { consonantCount += 1 }
        else if "aeiou".contains($0) { vowelCount += 1 }
    }
    return (vowelCount, consonantCount)
}

