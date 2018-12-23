# epsilon provided for floats

# brute and exhaustive
def exhaustiveFindRoot(x, epsilon):
    step = epsilon**2
    numGuesses = 0
    ans = 0.0

    while abs(ans**2 - x) >= epsilon and ans <= x:
        ans += step
        numGuesses += 1
    print('numGuesses =', numGuesses)
    if abs(ans**2 - x) <= epsilon:
        print('Failed to find root of', x)
    else:
        print(ans, 'is close to root of', x)

# newton-raphson
def newtonRaphson(k, epsilon):
    guess = k / 2.0
    while abs(guess*guess - k) >= epsilon:
        guess = guess - (((guess**2) - k) / (2*guess))
    print(guess)

# bi:
def findRoot(x, power, epsilon):
    if x < 0 and power%2 == 0:
        return None

    low = min(-1.0, x)
    high = max(1.0, x)
    ans = (high + low) / 2.0
    while abs(ans**power - x) >= epsilon:
        if ans**power < x:
            low = ans
        else:
            high = ans
        ans = (high + low) / 2.0
    return ans


def testFindRoot():
    epsilon = 0.001
    for x in (0.25, -0.25, 2, -2, 8, -8):
        for power in range(1, 4):
            print('Testing x =', str(x), 'and power = ', power)
            result = findRoot(x, power, epsilon)
            if result == None:
                print(' No root')
            else:
                print(' ', result**power, '~=', x)

