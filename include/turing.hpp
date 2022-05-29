#pragma once
#ifndef _INC_TURING_HPP
#define _INC_TURING_HPP

#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

enum TuringDirection { Left, Right, NoMove };

const char LAMBDA = 'λ';

struct TuringTurn {
    TuringTurn()
        : oldState(-1),
          oldSymbol(LAMBDA),
          newState(-1),
          newSymbol(LAMBDA),
          direction(NoMove) {}
    TuringTurn(int oldState, char oldSymbol, int newState, char newSymbol,
               TuringDirection direction)
        : oldState(oldState),
          oldSymbol(oldSymbol),
          newState(newState),
          newSymbol(newSymbol),
          direction(direction) {}
    ~TuringTurn() {}
    int oldState;
    char oldSymbol;
    int newState;
    char newSymbol;
    TuringDirection direction;
};

class TuringTape {
   public:
    TuringTape() {}
    ~TuringTape() {}

    void setChar(int pos, char c) { tape[pos] = c; }

    char getChar(int pos) {
        if (tape.count(pos) == 0) {
            return '\0';
        } else {
            return tape[pos];
        }
    }

    std::map<int, char> getTapeMap(void) { return tape; }

   private:
    std::map<int, char> tape;
    int offset;
};

struct TuringState {
    TuringState() {}
    ~TuringState() {}
    TuringTape tape;
    int position;
    int currState;
};

class TuringMachine {
   public:
    TuringMachine() {}
    ~TuringMachine() {}
    void loadState(TuringState newState) { state = newState; }

    char getCurChar(void) { return state.tape.getChar(state.position); }

    void makeTurn(void) {
        if (stop) {
            return;
        }
        TuringTurn turn = table[std::make_pair(state.currState, getCurChar())];
        state.tape.setChar(state.position, turn.newSymbol);
        if (turn.direction == Left) {
            state.position--;
        } else if (turn.direction == Right) {
            state.position++;
        }
        state.currState = turn.newState;
        if (state.currState == -1) {
            stop = true;
        }
    }

    std::string getTapeStr(void) { return state.tape.getStr(); }

    void clear(void) { stop = false; }

   private:
    std::map<std::pair<int, char>, TuringTurn> table;
    std::set<char> alph;
    bool stop = false;
    std::string comment;
    TuringState state;
};

#endif