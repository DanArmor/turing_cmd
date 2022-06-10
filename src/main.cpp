#include <unistd.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/table.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"
#include "ftxui/util/ref.hpp"

#include "turing.hpp"
#include "turingUI.hpp"
#include "tao/json.hpp"

int main(int argc, char **argv) {
    if(argc == 1){
        auto screen = ftxui::ScreenInteractive::TerminalOutput();

        auto ui = ftxui::Make<TuringUI>(screen.ExitLoopClosure(), &screen);

        screen.Loop(ui);
    } else if(argc == 2){
        std::cout << "Not implemented yet";
        exit(1);
    } else{
        std::cout << "Слишком много аргументов. Введите только имя программы или имя программы и имя файла для открытия\n";
        exit(1);
    }

    return 0;
}
