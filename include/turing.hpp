#pragma once
#ifndef _INC_TURING_HPP
#define _INC_TURING_HPP

#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

enum TuringDirection { Left, Right, NoMove };

const char TURING_EMPTY = '$';

struct TuringTurn {
    TuringTurn()
        : oldState(-1),
          oldSymbol(TURING_EMPTY),
          newState(-1),
          newSymbol(TURING_EMPTY),
          direction(NoMove) {}
    TuringTurn(int oldState, char oldSymbol, int newState, char newSymbol,
               TuringDirection direction)
        : oldState(oldState),
          oldSymbol(oldSymbol),
          newState(newState),
          newSymbol(newSymbol),
          direction(direction) {}
    TuringTurn(int newState, char newSymbol, TuringDirection direction)
        : newState(newState), newSymbol(newSymbol), direction(direction) {}
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
            return TURING_EMPTY;
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
    void setAlph(std::string str) {
        alph.clear();
        alph.insert(TURING_EMPTY);
        for (auto c : str) {
            alph.insert(c);
        }
    }
    void setComment(std::string const &str) { comment = str; }

    void addTurn(TuringTurn turn) {
        table[std::make_pair(turn.oldState, turn.oldSymbol)] = turn;
    }
    void addTurn(std::pair<int, char> old, TuringTurn turn) {
        table[old] = turn;
    }

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

    std::map<int, char> getTapeStr(void) { return state.tape.getTapeMap(); }
    std::string getStrView(int from, int to) {
        std::string out;
        for (int i = from; i <= to; i++) {
            out += state.tape.getChar(i);
        }
        return out;
    }

    void clear(void) { stop = false; }

   private:
    std::map<std::pair<int, char>, TuringTurn> table;
    std::set<char> alph;
    bool stop = false;
    std::string comment;
    TuringState state;
};

#endif