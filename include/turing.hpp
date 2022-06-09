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

const wchar_t TURING_EMPTY = L'λ';
const std::wstring TURING_EMPTY_STR{TURING_EMPTY};

struct TuringTurn {
    TuringTurn();
    TuringTurn(int oldState, wchar_t oldSymbol, int newState, wchar_t newSymbol,
               TuringDirection direction);
    TuringTurn(int newState, wchar_t newSymbol, TuringDirection direction);
    ~TuringTurn();
    int oldState;
    wchar_t oldSymbol;
    int newState;
    wchar_t newSymbol;
    TuringDirection direction;
};

class TuringTape {
   public:
    TuringTape();
    ~TuringTape();

    void setChar(int pos, wchar_t c);
    wchar_t getChar(int pos);
    void clear(void);

    std::map<int, wchar_t> getTapeMap(void);

   private:
    std::map<int, wchar_t> tape;
    int offset;
};

struct TuringState {
    TuringState();
    TuringState(std::pair<int, int> posAndState,
                std::initializer_list<std::pair<int, wchar_t>> chars);
    ~TuringState();
    TuringTape tape;
    int position;
    int currState;
};

class TuringMachine {
   public:
    TuringMachine();
    TuringMachine(
        std::initializer_list<std::pair<std::pair<int, wchar_t>, TuringTurn>>
            turns);
    ~TuringMachine();

    void loadState(TuringState newState);
    void setAlph(std::wstring str);
    void setComment(std::wstring const &str);

    void addTurn(TuringTurn turn);
    void addTurn(std::pair<int, wchar_t> old, TuringTurn turn);

    void makeTurn(void);

    wchar_t getCurChar(void);
    std::wstring getStrView(int from, int to);
    TuringTape getTape(void);

    int getCurState(void);
    int getCurPosition(void);
    std::map<std::pair<int, wchar_t>, TuringTurn> &getTable(void);

    bool isDone(void);
    void clear(void);
    void clearTurns(void);

   private:
    std::map<std::pair<int, wchar_t>, TuringTurn> table;
    std::set<wchar_t> alph;
    bool stop = false;
    std::wstring comment;
    TuringState state;
};

std::wstring pickDirectStr(TuringDirection direction);
TuringDirection pickDirect(wchar_t c);

#endif