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

int main() {
    auto screen = ftxui::ScreenInteractive::TerminalOutput();

    auto ui = ftxui::Make<TuringUI>(screen.ExitLoopClosure());

    screen.Loop(ui);

    return 0;
}
