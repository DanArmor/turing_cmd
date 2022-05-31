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

using namespace ftxui;

Component stateTableUI(std::string alph) {
    class Impl : public ComponentBase {
       public:
        Impl(std::string alph) {
            alph_ = alph;
            expendButton = Button("Add", [&]{
                return;
            });
            removeButton = Button("Remove", [&]{
                return;
            });
        }
        ~Impl() {}

       private:
        Component expendButton;
        Component removeButton;
        Component rows = Container::Vertical({});
        std::string alph_;
        Element Render() override {
            std::vector<Element> rows;
            for(auto c : alph_){
                std::string s{c};
                std::vector<Element> row;
                row.push_back(text(s));
                row.push_back(text(s)); // Test 
                row.push_back(expendButton->Render() | bgcolor(Color::Green));
                row.push_back(removeButton->Render() | bgcolor(Color::Red));
                rows.push_back(hbox(row));
            }
            return vbox(rows);
        }
    };
    return Make<Impl>(alph);
}

const int sizeCells = 20;

int main() {
    TuringState state{{0, 0},
                      {{0, '0'}, {1, '0'}, {2, '0'}, {3, '_'}, {4, '_'}}};

    TuringMachine control;

    // Alph and comment
    std::string alphStr = "01_";
    std::string commentStr = "Базовый комментарий";
    auto alphInput = Input(&alphStr, "");
    auto commentInput = Input(&commentStr, "");

    control.loadState(state);

    // TABLE
    std::vector<std::vector<std::string>> table_strs;
    std::vector<std::vector<Component>> table_inputs;

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

    // Cells funcs
    auto colorCell = [&](int i) {
        if (i == control.getCurPosition()) {
            return cells[i]->Render() | bgcolor(Color::Blue);
        } else {
            return cells[i]->Render();
        }
    };
    auto getFinalCells = [&]() {
        for (int i = 0; i < sizeCells; i++) {
            cells_vbox[i] =
                vbox({cells_numbers[i], separator(), colorCell(i)}) |
                size(WIDTH, Constraint::GREATER_THAN, 3);
            cells_final[i] = hbox(cells_vbox[i], separator());
        }
        return cells_final;
    };

    // Some update funcs
    auto setStartTapeUI = [&]() {
        for (int i = 0; i < sizeCells; i++) {
            cells_strs[i] = state.tape.getChar(i);
        }
    };
    auto updateMachine = [&]() {
        control.setAlph(alphStr);
        control.setComment(commentStr);
    };
    auto stepButtonAction = [&]() {
        updateMachine();
        control.makeTurn();
    };
    auto runButtonAction = [&]() {
        updateMachine();
        control.makeTurn();
    };
    auto updateTapeUI = [&]() {
        TuringTape tape = control.getTape();
        for (int i = 0; i < sizeCells; i++) {
            cells_strs[i] = tape.getChar(i);
        }
    };

    createCells();
    setStartTapeUI();

    // UI

    // Buttons
    auto stepButton = Button("Step", stepButtonAction);
    auto runButton = Button("Run", runButtonAction);

    auto cellsComponent = Container::Horizontal(cells);
    auto tableComponent = stateTableUI(alphStr);
    auto mainComponent =
        Container::Vertical({cellsComponent, stepButton, runButton, alphInput,
                             commentInput, tableComponent});
    std::vector<std::string> testS{"1", "2", "3", "4"};
    auto drawTableUI = [&]() {
        auto t =
            Container::Horizontal({Input(&testS[0], ""), Input(&testS[1], "")});
        auto tt =
            Container::Horizontal({Input(&testS[2], ""), Input(&testS[3], "")});
        tableComponent->Add(t);
        tableComponent->Add(tt);
    };
    drawTableUI();

    auto renderer = Renderer(mainComponent, [&] {
        updateTapeUI();
        return vbox({text("Машина Тьюринга"), separator(),
                     hbox(getFinalCells()), separator(),
                     hbox(stepButton->Render() | size(WIDTH, EQUAL, 10) |
                              color(Color::SkyBlue1),
                          separator(),
                          runButton->Render() | size(WIDTH, EQUAL, 10) |
                              color(Color::SkyBlue1),
                          separator(),
                          vbox({text("Алфавит"), separator(),
                                alphInput->Render()}),
                          separator(),
                          vbox({text("Комментарий"), separator(),
                                commentInput->Render()}),
                          separator()) |
                         flex,
                     separator(),
                     tableComponent->Render()}) |
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
