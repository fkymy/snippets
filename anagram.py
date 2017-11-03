#!/bin/usr/python3

"""Anagram Matches
Given a target string and a set of words to compare,
return a list of anagram matches for that target string.

Example:
>>> find_anagram_match('act', {'cat', 'rat', 'dog', 'act'})
['act', 'cat']

Note:
Single words
Assume words are given in lowercase
Returned list should be sorted

"""

import sys
from collections import Counter
from operator import mul
from functools import reduce
from itertools import permutations

from string import ascii_lowercase
from random import choice
from time import time

# Traditional solution
# Sorts candidate string and target string to test for equality
# O(n)
def match_by_sort(string, words):
    target = sorted(string)
    return sorted(word for word in words if sorted(word) == target)


# Use collections.Counter() to make multiset equality test
# They are multiset because {a, a, b} and {a, b} has to be different (altough same set)
# and {a, a, b} and {a, b, a} are the same multiset
# O(n log n) however, consider cost of hashing all characters
def match_by_counter(string, words):
    target = Counter(string)
    return sorted(word for word in words if Counter(word) == target)


# Solution using perfect hash
# built on unique prime factorization theorem

# range to half of num
# def is_prime(num):
#     if not num > 1: return False
#     n = num // 2
#     for i in range(2, n + 1):
#         if num % i == 0: return False
#     return True

# range to sqrt of num
def is_prime(num):
    prime = True
    k = 2
    n = num ** 0.5    # sqrt(num)
    while k <= n and prime is True:
        if num % k == 0:
            prime = False
        k += 1
    return prime

def match_by_perfect_hash(string, words):
    primes = [2, 3, 5, 7, 11]
    primes += [p for p in range(13, 1620) if is_prime(p)]    # 256 primes
    # cumulatively mul chars in string for hashable
    # doesn't need 256 if only lowercase alphabet is given for c
    anagram_hash = lambda s: reduce(mul, (primes[ord(c)] for c in s))
    target = anagram_hash(string)
    return sorted(word for word in words if anagram_hash(word) == target)


# Solution using Permutations
# using itertools.permutations()
# generating permutations on length n generates n factorial candidates :(
# when target str is small and len(words) is potentially large
# this approach can be fit (as set membership testing is O(1))
def match_by_permutations(string, words):
    perms = set(map(''.join, permutations(string)))
    return sorted(word for word in words if word in perms)

def match_by_permutations_set(string, words):
    perms = set(map(''.join, permutations(string)))
    return sorted(words & perms)


# Simple test
def test(fn):
    passed = False
    string_to_test = 'silent'
    words_to_test = {'listen', 'leesin', 'python', 'silen', 'enlist'}
    expected = ['enlist', 'listen']

    got = fn(string_to_test, words_to_test)
    if got == expected:
        prefix = ' OK '
        passed = True
    else:
        prefix = ' X '

    return passed, f"{fn.__name__}", f"{prefix} got: {got}, expected: {expected}"


# Timing test
string_size = 5    # enlarge this to analyze solutions
words_size = 100_0000
population = ascii_lowercase[: string_size+2]
words_to_time = set()
for i in range(words_size):
    word = ''.join([choice(population) for i in range(string_size)])
    words_to_time.add(word)
# Arbitrarily search takes last word to be the target string for testing
string_to_time = word

def main():
    print('Anagram Match\n')

    # Solutions to be tested
    fns = (
        match_by_sort,
        match_by_counter,
        match_by_perfect_hash,
        match_by_permutations,
        match_by_permutations_set
    )

    print('Simple test assert equal')
    for fn in fns:
        passed, fname, message = test(fn)
        if passed:
            print(fname, message)
        else:
            sys.exit(1)

    print(f"\nTiming with string_size={string_size} and words_size={words_size}")
    for fn in fns:
        start = time()
        fn(string_to_time, words_to_time)
        end = time()
        print("{}\t{}".format((end - start), fn.__name__))


if __name__ == '__main__':
    main()

