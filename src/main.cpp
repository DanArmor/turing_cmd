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

const int sizeCells = 20;

int main() {
    TuringState state{{0, 0},
                      {{0, '0'}, {1, '0'}, {2, '0'}, {3, '_'}, {4, '_'}}};

    TuringMachine control{
        {{{0, '0'}, {0, '1', Right}}, {{0, '_'}, {-1, '_', NoMove}}}};
    control.setAlph("01_");
    control.setComment("Комментарий");

    control.loadState(state);

    // CELLS
    std::vector<Component> cells;
    std::vector<std::string> cells_strs;
    std::vector<Element> cells_numbers;
    std::vector<Element> cells_vbox;
    std::vector<Element> cells_final;

    auto createCells = [&]() {
        for (int i = 0; i < sizeCells; i++) {
            cells_strs.push_back("");
        }
        for (int i = 0; i < sizeCells; i++) {
            cells.push_back(Input(&cells_strs[i], ""));
            cells_numbers.push_back(text(std::to_string(i)));
            cells_vbox.push_back(
                vbox({cells_numbers[i], separator(), cells[i]->Render()}) |
                size(WIDTH, Constraint::GREATER_THAN, 3));
            cells_final.push_back(hbox(cells_vbox[i], separator()));
        }
        // return hbox(cells_final);
    };

    auto getFinal = [&]() {
        for (int i = 0; i < sizeCells; i++) {
            cells_vbox[i] =
                vbox({cells_numbers[i], separator(), cells[i]->Render()})  |
                size(WIDTH, Constraint::GREATER_THAN, 3);
            cells_final[i] = hbox(cells_vbox[i], separator());
        }
        return cells_final;
    };

    createCells();

    // UI

    auto cellsComponent = Container::Horizontal(cells);
    auto mainComponent = Container::Vertical({cellsComponent});
    auto renderer = Renderer(mainComponent, [&] {
        return vbox({text("Машина Тьюринга"), separator(), hbox(getFinal())}) |
               border | flex;
    });

    auto screen = ScreenInteractive::TerminalOutput();

    auto toRend = CatchEvent(renderer, [&](Event event) {
        if (event == Event::Character('q')) {
            screen.ExitLoopClosure()();
            return true;
        }
        return false;
    });

    screen.Loop(toRend);

    return 0;
}
