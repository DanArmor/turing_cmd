#pragma once
#ifndef _INC_TURING_HPP
#define _INC_TURING_HPP

#include <vector>
#include <string>
#include <stdexcept>

enum TuringDirection{
    Left,
    Right,
    NoMove
};

struct TuringTurn{
    TuringTurn() : oldState(-1), oldSymbol('_'), newState(-1), newSymbol('_'), direction(NoMove){}
    TuringTurn(int oldState, char oldSymbol, int newState, char newSymbol, TuringDirection direction) : 
    oldState(oldState), oldSymbol(oldSymbol), newState(newState), newSymbol(newSymbol), direction(direction){}
    ~TuringTurn(){}
    int oldState;
    char oldSymbol;
    int newState;
    char newSymbol;
    TuringDirection direction;
};

class TuringTape{
    public:
    TuringTape(){}
    TuringTape(std::string tape) : tape(tape){}
    ~TuringTape(){}

    void setChar(int pos, char c){
        if(pos < 0 || pos >= tape.size())
            throw std::invalid_argument("Выход за пределы ленты");
        tape[pos] = c;
    }

    char getChar(int pos){
        if(pos < 0 || pos >= tape.size())
            throw std::invalid_argument("Выход за пределы ленты");
        return tape[pos];
    }

    std::string getStr(void){
        return tape;
    }

    private:
    std::string tape;
    int offset;
};

struct TuringState{
    TuringState(){}
    ~TuringState(){}
    std::string alph;
    std::vector<std::vector<TuringTurn>> table;
    TuringTape tape;
    std::string comment;
    int position;
    int currState;
};

class TuringMachine{
    public:
    TuringMachine(){}
    ~TuringMachine(){}
    void loadState(TuringState newState){ state = newState; }

    int indexInAlph(char c){
        for(int i = 0; i < state.alph.size(); i++)
            if(state.alph[i] == c)
                return i;
        throw std::invalid_argument("Символ не входит в алфавит! " + std::to_string(c));
    }

    char getCurChar(void){
        return state.tape.getChar(state.position);
    }

    void makeTurn(void){
        if(stop){
            return;
        }
        TuringTurn turn = state.table[state.currState][indexInAlph(getCurChar())];
        state.tape.setChar(state.position, turn.newSymbol);
        if(turn.direction == Left){
            state.position--;
        } else if (turn.direction == Right){
            state.position++;
        }
        state.currState = turn.newState;
        if(state.currState == -1){
            stop = true;
        }
    }

    std::string getTapeStr(void){
        return state.tape.getStr();
    }

    void clear(void){
        stop = false;
    }

    private:
    bool stop = false;
    TuringState state;
};


#endif