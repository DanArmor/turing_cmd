#include <unistd.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"
#include "ftxui/util/ref.hpp"
#include "turing.hpp"

using namespace ftxui;

class TuringFTXUIController {
   public:
    TuringFTXUIController(TuringMachine machine) : machine(machine) {}
    TuringFTXUIController(){}

    void loadState(TuringState newState) { machine.loadState(newState); }
    void setAlph(std::string str) { machine.setAlph(str); }
    void setComment(std::string const& str) { machine.setComment(str); }

    void addTurn(TuringTurn turn) { machine.addTurn(turn); }
    void addTurn(std::pair<int, char> old, TuringTurn turn) {
        turn.oldState = old.first;
        turn.oldSymbol = old.second;
        machine.addTurn(turn);
    }

    void makeTurn(void) { machine.makeTurn(); }

    char getCurChar(void) { return machine.getCurChar(); }
    std::map<int, char> getTapeMap(void) { return machine.getTapeMap(); }
    std::string getStrView(int from, int to) {
        return machine.getStrView(from, to);
    }

    int getCurPosition(void){ return machine.getCurPosition(); }
    int getCurState(void) { return machine.getCurState(); }
    bool isDone(void) { return machine.isDone(); }
    void clear(void);

   private:
    TuringMachine machine;
};

int main() {
    TuringState state;
    state.currState = 0;
    state.position = 0;
    state.tape.setChar(0, '0');
    state.tape.setChar(1, '0');
    state.tape.setChar(2, '0');
    state.tape.setChar(3, '_');
    state.tape.setChar(4, '_');

    TuringTurn turn1{0, '1', Right};
    TuringTurn turn2{-1, '_', NoMove};


    // UI
    TuringFTXUIController control;
    control.setAlph("01_");
    control.setComment("Комментарий");

    control.addTurn(std::make_pair(0, '0'), turn1);
    control.addTurn(std::make_pair(0, '_'), turn2);
    control.loadState(state);

    std::vector<std::string> cells_strs(5);
    std::vector<Component> cells{
        Input(&cells_strs[0], ""), Input(&cells_strs[1], ""),
        Input(&cells_strs[2], ""), Input(&cells_strs[3], ""),
        Input(&cells_strs[4], "")};

    auto cellRenderer = [&](int i) {
        if(i == control.getCurPosition()){
            return cells[i]->Render() | bgcolor(Color::Red1);
        } else{
            return cells[i]->Render();
        }
    };

    auto showTape = [&]() {
        std::map<int, char> tapeMap = control.getTapeMap();
        for (int i = 0; i < cells_strs.size(); i++) {
            cells_strs[i] = tapeMap[i];
        }
    };

    Element curState = text("");

    auto showState = [&]() {
        curState = text("Q " + std::to_string(control.getCurState()));
    };

    showTape();
    showState();

    auto stepFunc = [&]() {
        control.makeTurn();
        showTape();
        showState();
    };

    Component button = Button("Step", stepFunc);

    auto component = Container::Vertical(
        {cells[0], cells[1], cells[2], cells[3], cells[4], button});
    auto renderer = Renderer(component, [&] {
        return vbox({text("Программа"), separator(),
                     hbox(cellRenderer(0), separator(), cellRenderer(1),
                          separator(), cellRenderer(2), separator(),
                          cellRenderer(3), separator(), cellRenderer(4)),
                     separator(), button->Render(), hbox(vbox(text("Текущее состояние"), curState))}) |
               border | flex;
    });

    auto screen = ScreenInteractive::TerminalOutput();

    auto toRend = CatchEvent(renderer, [&](Event event) {
        if (event == Event::Character('q') || event == Event::Escape) {
            screen.ExitLoopClosure()();
            return true;
        }

        if (event == Event::Return) {
        }
    });
    screen.Loop(toRend);

    return 0;
}
