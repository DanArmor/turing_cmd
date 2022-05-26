#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <sstream>

#include "ftxui/dom/elements.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/screen/string.hpp"
#include "ftxui/util/ref.hpp"

#include "turing.hpp"

std::stringstream ss;
std::vector<std::string> lines;

using namespace ftxui;

Element RenderLines(void) {
    Elements list;

    std::string line;
    while(std::getline(ss, line)){
        lines.push_back(line);
    }

    for (auto& line : lines) {
        list.push_back(text(line));
    }
    ss.clear();
    return vbox(std::move(list)) | frame | border | flex;
}

std::vector<std::string> breakBySpaces(std::string str){
    std::string space_delimiter = " ";
    std::vector<std::string> words;

    size_t pos = 0;
    while ((pos = str.find(space_delimiter)) != std::string::npos) {
        words.push_back(str.substr(0, pos));
        str.erase(0, pos + space_delimiter.length());
    }
    if(str.size() != 0){
        words.push_back(str);
    }
    return words;
}

int main() {
//    std::string inputStr;
//    std::string errPos;
//
//    auto component = Container::Vertical({
//    });
//
//    auto renderer = Renderer(component, [&]{
//        return vbox({
//            text("Программа"),
//        }) | border | flex;
//    });
//
//    auto screen = ScreenInteractive::TerminalOutput();
//
//    auto toRend = CatchEvent(renderer, [&](Event event){
//        if(event == Event::Character('q') || event == Event::Escape){
//            screen.ExitLoopClosure()();
//            return true;
//        }
//
//        if(event == Event::Return){
//        }
//
//    });
//    screen.Loop(toRend);

    TuringMachine machine;   
    TuringState state;
    TuringTape tape("000__");
    state.alph = "01_";
    state.comment = "Комментарий";
    state.currState = 0;
    state.position = 0;
    state.tape = tape;
    state.table.resize(2);
    for(int i = 0; i < 2; i++){
        state.table[i].resize(3);
    }
    TuringTurn turn1{0, '0', 0, '1', Right};
    TuringTurn turn2{0, '_', -1, '_', NoMove};
    state.table[0][0] = turn1;
    state.table[0][2] = turn2;

    machine.loadState(state);
    std::cout << machine.getTapeStr() << "\n";
    machine.makeTurn();
    std::cout << machine.getTapeStr() << "\n";
    machine.makeTurn();
    std::cout << machine.getTapeStr() << "\n";
    machine.makeTurn();
    std::cout << machine.getTapeStr() << "\n";
    machine.makeTurn();
    std::cout << machine.getTapeStr() << "\n";

    return 0;
}
