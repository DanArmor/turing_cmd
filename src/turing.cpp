#include "turing.hpp"

// Turing turn section

TuringTurn::TuringTurn()
    : oldState(-1),
      oldSymbol(TURING_EMPTY),
      newState(-1),
      newSymbol(TURING_EMPTY),
      direction(NoMove) {}
TuringTurn::TuringTurn(int oldState, char oldSymbol, int newState,
                       char newSymbol, TuringDirection direction)
    : oldState(oldState),
      oldSymbol(oldSymbol),
      newState(newState),
      newSymbol(newSymbol),
      direction(direction) {}
TuringTurn::TuringTurn(int newState, char newSymbol, TuringDirection direction)
    : newState(newState), newSymbol(newSymbol), direction(direction) {}
TuringTurn::~TuringTurn() {}

// Turing tape section

TuringTape::TuringTape() {}
TuringTape::~TuringTape() {}

void TuringTape::setChar(int pos, char c) { tape[pos] = c; }

char TuringTape::getChar(int pos) {
    if (tape.count(pos) == 0) {
        return TURING_EMPTY;
    } else {
        return tape[pos];
    }
}

std::map<int, char> TuringTape::getTapeMap(void) { return tape; }

// Turing state section

TuringState::TuringState() {}
TuringState::TuringState(std::pair<int, int> posAndState,
                         std::initializer_list<std::pair<int, char>> chars) {
    position = posAndState.first;
    currState = posAndState.second;
    for (auto &p : chars) {
        tape.setChar(p.first, p.second);
    }
}
TuringState::~TuringState() {}

// Turing machine section

TuringMachine::TuringMachine() {}
TuringMachine::TuringMachine(
    std::initializer_list<std::pair<std::pair<int, char>, TuringTurn>> turns) {
    for (auto &p : turns) {
        addTurn(p.first, p.second);
    }
}
TuringMachine::~TuringMachine() {}

void TuringMachine::loadState(TuringState newState) { state = newState; }
void TuringMachine::setAlph(std::string str) {
    alph.clear();
    alph.insert(TURING_EMPTY);
    for (auto c : str) {
        alph.insert(c);
    }
}
void TuringMachine::setComment(std::string const &str) { comment = str; }

void TuringMachine::addTurn(TuringTurn turn) {
    table[std::make_pair(turn.oldState, turn.oldSymbol)] = turn;
}
void TuringMachine::addTurn(std::pair<int, char> old, TuringTurn turn) {
    table[old] = turn;
}

char TuringMachine::getCurChar(void) {
    return state.tape.getChar(state.position);
}

void TuringMachine::makeTurn(void) {
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

TuringTape TuringMachine::getTape(void) { return state.tape; }

std::string TuringMachine::getStrView(int from, int to) {
    std::string out;
    for (int i = from; i <= to; i++) {
        out += state.tape.getChar(i);
    }
    return out;
}

std::map<std::pair<int, char>, TuringTurn> &TuringMachine::getTable(void) {
    return table;
}

int TuringMachine::getCurPosition(void) { return state.position; }
int TuringMachine::getCurState(void) { return state.currState; }
bool TuringMachine::isDone(void) { return stop; }
void TuringMachine::clear(void) { stop = false; }

std::string pickDirectStr(TuringDirection direction) {
    switch (direction) {
        case TuringDirection::Left:
            return "←";
        case TuringDirection::Right:
            return "→";
        case TuringDirection::NoMove:
            return "↓";
    }
}