extension Sequence where Iterator.Element: Hashable {
    func unique() -> [Iterator.Element] {
        var seen: [Iterator.Element: Bool] = [:]
        return self.filter { seen.updateValue(true, forKey: $0) == nil }
    }
}

func anyCommonElements<T: Sequence, U: Sequence>(_ lhs: T, _ rhs: U) -> [Int]
    where T.Iterator.Element: Equatable, T.Iterator.Element == U.Iterator.Element {
        var matchedItems = [Int]()

        for lhsItem in lhs {
            print("lhsItem: \(lhsItem)")
            for rhsItem in rhs {
                print("rhsItem: \(rhsItem)")
                if lhsItem == rhsItem {
                    matchedItems.append(lhsItem as! Int)
                }
            }
        }
//        return Array(Set(matchedItems))
        return matchedItems.unique()
}

// better?
//func anyCommonElements<T: Collection, U: Collection>(_ lhs: T, _ rhs: U) -> [T.Element] where T.Element: Hashable, T.Element == U.Element {
//    var matchedItems = [T.Element]()
//
//    for lhsElement in lhs {
//        print("lhs element: \(lhsElement)")
//        for rhsElement in rhs {
//            if lhsElement == rhsElement {
//                matchedItems.append(lhsElement)
//            }
//        }
//    }
//
//    return Array(Set(matchedItems))
//}

anyCommonElements([1, 2, 3, 4, 4], [4, 5, 6, 7])
anyCommonElements([1, 2, 3], [4, 5, 6])
