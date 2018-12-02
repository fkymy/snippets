/**
 * Primary idea: Exclusive or
 * Time Complexity: O(n), Space Complexity: O(1)
 *
 */

class SingleNumber {
    func singleNumber(nums: [Int]) -> Int {
        var res = 0
        
        for num in nums {
            res ^= num
        }
        
        return res
    }
}
