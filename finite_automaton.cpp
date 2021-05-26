#include <cctype>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>

using namespace std;

struct DFA {
    map<pair<int, char>, int> transitions;
    set<int> acceptingState;
    int startState;
};

struct NFA {
    multimap<pair<int, char>, int> transitions;
    set<int> acceptingStates;
    int startState;
};

bool SimulateDFA(DFA& d, string input) {
    int currState = d.startState;
    for (string::iterator itr = input.begin(); itr != input.end(); ++itr) {
        currState = d.transitions[make_pair(currState, *itr)];
    }

    return d.acceptingState.find(currState) != d.acceptingState.end();
}

bool SimulateNFA(NFA& nfa, string input) {
    set<int> currStates;
    currStates.insert(nfa.startState);

    for (string::iterator itr = input.begin(); itr != input.end(); ++itr) {
        set<int> nextStates;
        for (set<int>::iterator state = currStates.begin();
             state != currStates.end(); ++state) {
            pair<multimap<pair<int, char>, int>::iterator,
                 multimap<pair<int, char>, int>::iterator>
                transitions =
                    nfa.transitions.equal_range(make_pair(*state, *itr));

            for (; transitions.first != transitions.second;
                 ++transitions.first) {
                nextStates.insert(transitions.first->second);
            }
        }
    }

    for (set<int>::iterator itr = currStates.begin(); itr != currStates.end();
         ++itr) {
        if (nfa.acceptingStates.count(*itr)) return true;
    }
    return false;
}
