// GameOfSet isSet algorithm
// https://en.wikipedia.org/wiki/Set_(game)
// given a list of cards, determine whether they construct a set or not

struct Constants {
    static let numberOfCardsInSet = 3
    static let numberOfFeaturesInCard = 4
}

struct Card {

    let number: Numbers
    let color: Colors
    let shape: Shapes
    let fill: Fills
    
    init(with n: Int, _ c: Int, _ s: Int, _ f: Int) {
        self.number = Numbers(rawValue: n)!
        self.color = Colors(rawValue: c)!
        self.shape = Shapes(rawValue: s)!
        self.fill = Fills(rawValue: f)!
    }
    
    enum Numbers: Int {
        case one = 1
        case two
        case three
    }
    
    enum Colors: Int {
        case red = 1
        case green
        case purple
    }
    
    enum Shapes: Int {
        case diamond = 1
        case squiggle
        case oval
    }
    
    enum Fills: Int {
        case solid = 1
        case striped
        case open
    }
}

extension Card {
    var rawValuesAsMatrix: [Int] {
        return [self.number.rawValue, color.rawValue, shape.rawValue, fill.rawValue]
    }
}

func isSet(cards: [Card]) -> Bool {
    guard cards.count == Constants.numberOfCardsInSet else { return false }
    var sumMatrix = [Int](repeating: 0, count: cards[0].rawValuesAsMatrix.count)
    for card in cards {
        let matrix = card.rawValuesAsMatrix
        for index in matrix.indices {
            sumMatrix[index] += matrix[index]
        }
    }
    return sumMatrix.reduce(true, { $0 && $1 % Constants.numberOfCardsInSet == 0 })
}

