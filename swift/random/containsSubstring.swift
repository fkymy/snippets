import Foundation

// does one String contain another String?
// write your own version of the contains method on String that
// ignores letter case.
// See: https://developer.apple.com/reference/swift/string/1643241-contains
//  returns true iff other is non-empty and contained within self by
//  case-sensitive, non-literal search.

extension String {
    func doesContain(_ substring: String) -> Bool {
        return self.lowercased().range(of: substring.lowercased()) != nil
    }

    // Swift 4
    func doesContain2(_ substring: String) -> Bool {
        return self.lowercased().contains(substring.lowercased())
    }

    func fuzzyContains(_ substring: String) -> Bool {
        return range(of: substring, options: .caseInsensitive) != nil
    }
}

print("hello".doesContain("world")) // false
print("hello".doesContain("llo")) // true

print("hello".doesContain2("world")) // false
print("hello".doesContain2("ello")) // true
