#pragma once
#ifndef _INC_TURING_HPP
#define _INC_TURING_HPP

#include <initializer_list>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

enum TuringDirection { Left, Right, NoMove };

extern const char TURING_EMPTY;
extern const std::string TURING_EMPTY_STR;

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
    void clear(void);

    std::map<int, char> getTapeMap(void);

   private:
    std::map<int, char> tape;
    int offset;
};

struct TuringState {
    TuringState();
    TuringState(std::pair<int, int> posAndState,
                std::initializer_list<std::pair<int, char>> chars);
    ~TuringState();
    TuringTape tape;
    int position;
    int currState;
};

class TuringMachine {
   public:
    TuringMachine();
    TuringMachine(
        std::initializer_list<std::pair<std::pair<int, char>, TuringTurn>>
            turns);
    ~TuringMachine();

    void loadState(TuringState newState);
    void setAlph(std::string str);
    void setComment(std::string const &str);

    void addTurn(TuringTurn turn);
    void addTurn(std::pair<int, char> old, TuringTurn turn);

    void makeTurn(void);

    char getCurChar(void);
    std::string getStrView(int from, int to);
    TuringTape getTape(void);

    int getCurState(void);
    int getCurPosition(void);
    std::map<std::pair<int, char>, TuringTurn> &getTable(void);

    bool isDone(void);
    void clear(void);
    void clearTurns(void);

   private:
    std::map<std::pair<int, char>, TuringTurn> table;
    std::set<char> alph;
    bool stop = false;
    std::string comment;
    TuringState state;
};

std::string pickDirectStr(TuringDirection direction);
TuringDirection pickDirect(char c);

#endif