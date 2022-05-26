#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include "ftxui/dom/elements.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/screen/string.hpp"
#include "ftxui/util/ref.hpp"

#include <sstream>

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
    srand(time(NULL) + clock());
    std::string inputStr;
    std::string errPos;

    auto component = Container::Vertical({
    });

    auto renderer = Renderer(component, [&]{
        return vbox({
            text("Программа"),
        }) | border | flex;
    });

    auto screen = ScreenInteractive::TerminalOutput();

    auto toRend = CatchEvent(renderer, [&](Event event){
        if(event == Event::Character('q') || event == Event::Escape){
            screen.ExitLoopClosure()();
            return true;
        }

        if(event == Event::Return){
        }

    });
    screen.Loop(toRend);

    return 0;
}
