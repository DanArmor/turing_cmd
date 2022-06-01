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

std::vector<std::vector<std::string>> inputs;

Component stateTableUI(std::string alph) {
    class Impl : public ComponentBase {
       public:
        Impl(std::string alph) {
            alph_ = alph;
            inputs.clear();
            for (int i = 0; i < alph_.size(); i++) {
                rows->Add(Container::Horizontal({}));
            }
            expendButton = Button("Add", [this] {
                inputs.push_back({});
                for (int i = 0; i < alph_.size(); i++) {
                    inputs[colsAmount].push_back("");
                }
                for (int i = 0; i < alph_.size(); i++) {
                    rows->ChildAt(i)->Add(Input(&inputs[colsAmount][i], ""));
                }
                this->colsAmount++;
                return;
            });
            removeButton = Button("Remove", [this] {
                if(inputs.size() == 0)
                    return;
                this->colsAmount--;
                for (int i = 0; i < alph_.size(); i++) {
                    rows->ChildAt(i)->ChildAt(colsAmount)->Detach();
                }
                inputs.resize(inputs.size()-1);
                return;
            });
            this->Add(Container::Vertical(
                {Container::Horizontal({expendButton, removeButton}), rows}));
        }
        ~Impl() {}

       private:
        Component expendButton;
        Component removeButton;
        int colsAmount = 0;
        Component rows = Container::Vertical({});
        std::string alph_;

        Element Render() override {
            std::vector<Element> rows;
            rows.push_back(separatorHeavy());
            int i = 0;
            for (auto c : alph_) {
                std::string s{c};
                std::vector<Element> row;
                row.push_back(text(s));
                row.push_back(separator());
                for (int j = 0; j < this->rows->ChildAt(i)->ChildCount(); j++) {
                    row.push_back(
                        vbox(
                            {text("Q" + std::to_string(j)) | color(Color::Blue),
                             this->rows->ChildAt(i)->ChildAt(j)->Render()}) |
                        size(WIDTH, EQUAL, 5));
                    row.push_back(separator());
                }
                rows.push_back(hbox(row));
                rows.push_back(separatorHeavy());
                i++;
            }
            return vbox({hbox({expendButton->Render() | color(Color::Green),
                               removeButton->Render() | color(Color::Red)}),
                         vbox(rows)});
        }
    };
    return Make<Impl>(alph);
}

const int sizeCells = 20;

int main() {
    TuringState state;
    state.position = 0;
    state.currState = 0;

    TuringMachine control;

    // Alph and comment
    std::string alphStr = "01$";
    std::string commentStr;
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
            cells.push_back(Input(&cells_strs[i], "$"));
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
                vbox({cells_numbers[i] | bold, separator(), colorCell(i) | size(HEIGHT, EQUAL, 2)}) |
                size(WIDTH, Constraint::GREATER_THAN, 3);
            cells_final[i] = hbox(cells_vbox[i], separator());
        }
        return cells_final;
    };

    // Some update funcs
    auto setStartTapeUI = [&]() {
        //for (int i = 0; i < sizeCells; i++) {
            //cells_strs[i] = state.tape.getChar(i);
        //}
    };
    auto updateMachine = [&]() {
        control.setAlph(alphStr);
        control.setComment(commentStr);
        control.loadState(state);
        for(int i = 0; i < inputs.size(); i++){
            for(int j = 0; j < inputs[i].size(); j++){
                if(inputs[i][j].size() < 3)
                    continue;
                TuringDirection direction;
                switch(inputs[i][j][1]){
                    case '>' : direction = Right;
                        break;
                    case '<' : direction = Left;
                        break;
                    case '-' : direction = NoMove;
                        break;
                    default:
                        throw(std::invalid_argument("Неправильный тип движения"));
                }
                TuringTurn turn{inputs[i][j][2] - '0', inputs[i][j][0], direction};
                control.addTurn({i, alphStr[j]}, turn);
            }
        }
    };
    auto updateTapeUI = [&]() {
        state.tape.clear();
        for(int i = 0; i < cells_strs.size(); i++){
            if(cells_strs[i].size() == 0 || cells_strs[i][0] == ' '){
                state.tape.setChar(i, TURING_EMPTY);
            } else{
                state.tape.setChar(i, cells_strs[i][0]);
            }
        }
    };
    auto stepButtonAction = [&]() {
        updateTapeUI();
        state.currState = control.getCurState();
        state.position = control.getCurPosition();
        updateMachine();
        control.makeTurn();
        for(int i = 0; i < cells_strs.size(); i++){
            TuringTape tape = control.getTape();
            cells_strs[i] = tape.getChar(i);
        }
    };
    auto runButtonAction = [&]() {
    };

    createCells();
    setStartTapeUI();

    // UI

    // Buttons
    auto stepButton = Button("Step", stepButtonAction);
    auto runButton = Button("Run", runButtonAction);

    auto cellsComponent = Container::Horizontal(cells);
    auto tableComponent = Container::Vertical({stateTableUI(alphStr)});
    auto updateButtonAction = [&](){
        tableComponent->ChildAt(0)->Detach();
        tableComponent->Add(stateTableUI(alphStr));
    };
    auto updateButton = Button("Update", updateButtonAction);
    auto mainComponent =
        Container::Vertical({cellsComponent,
                             Container::Horizontal({stepButton, runButton, updateButton,
                                                    alphInput, commentInput}),
                             tableComponent});

    auto renderer = Renderer(mainComponent, [&] {
        if(alphStr.find('$') != std::string::npos){
            std::remove(alphStr.begin(), alphStr.end(), '$');
        }
        if(alphStr.back() != TURING_EMPTY){
            alphStr.push_back(TURING_EMPTY);
        }
        return vbox({text("Машина Тьюринга"), separator(),
                     hbox(getFinalCells()), separator(),
                     hbox(stepButton->Render() | size(WIDTH, EQUAL, 10) |
                              color(Color::SkyBlue1),
                          separator(),
                          runButton->Render() | size(WIDTH, EQUAL, 10) |
                              color(Color::SkyBlue1),
                          separator(),
                          updateButton->Render() | size(WIDTH, EQUAL, 10) |
                              color(Color::SkyBlue1),
                          separator(),
                          vbox({text("Алфавит"), separator(),
                                alphInput->Render()}),
                          separator(),
                          vbox({text("Комментарий"), separator(),
                                commentInput->Render()}),
                          separator()) |
                         flex,
                     separator(), tableComponent->Render()}) |
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
