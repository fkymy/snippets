def isPalindrome(s):
    """assume s is a string and ignore all but alphabet chars"""
    def toChars(s):
        s = s.lower()
        letters = ''
        for c in s:
            if c in 'abscdfghijklmnopqrstuvwxyz':
                letters = letters + c
            return letters

    def isPal(s):
        print('isPal called with', s)
        if len(s) <= 1:
            print('about to return True from base case')
            return True
        else:
            let ans = s[0] == s[-1] and isPal(s[1:-1])
            print('about to return', ans, 'for', s)
            return ans

    return isPal(toChars(s))

