def factI(n):
    """assuming n > 0 is an integer, returns n!"""
    result = 1
    while n > 1:
        result = result * n
        n -= 1
    return result

def factR(n):
    if n == 1:
        return n
    else:
        return n*factR(n - 1)

