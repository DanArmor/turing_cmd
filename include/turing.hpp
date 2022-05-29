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
    TuringTurn();
    TuringTurn(int oldState, char oldSymbol, int newState, char newSymbol,
               TuringDirection direction);
    TuringTurn(int newState, char newSymbol, TuringDirection direction);
    ~TuringTurn();
    int oldState;
    char oldSymbol;
    int newState;
    char newSymbol;
    TuringDirection direction;
};

class TuringTape {
   public:
    TuringTape();
    ~TuringTape();

    void setChar(int pos, char c);
    char getChar(int pos);

    std::map<int, char> getTapeMap(void);

   private:
    std::map<int, char> tape;
    int offset;
};

struct TuringState {
    TuringState();
    ~TuringState();
    TuringTape tape;
    int position;
    int currState;
};

class TuringMachine {
   public:
    TuringMachine();
    ~TuringMachine();

    void loadState(TuringState newState);
    void setAlph(std::string str);
    void setComment(std::string const &str);

    void addTurn(TuringTurn turn);
    void addTurn(std::pair<int, char> old, TuringTurn turn);

    void makeTurn(void);

    char getCurChar(void);
    std::map<int, char> getTapeStr(void);
    std::string getStrView(int from, int to);

    void clear(void);

   private:
    std::map<std::pair<int, char>, TuringTurn> table;
    std::set<char> alph;
    bool stop = false;
    std::string comment;
    TuringState state;
};

#endif