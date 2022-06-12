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

#include "tao/json/contrib/vector_traits.hpp"
#include "tao/json.hpp"


template<typename T>
std::vector<T> getAsArray(const tao::json::value &v, std::string name1, std::string name2){
    auto V = v.at(name1).at(name2).get_array();
    std::vector<T> result;
    for(auto it : V){
        result.push_back(it.as<T>());
    }
    return result;
}

int main(int argc, char **argv) {
    auto screen = ftxui::ScreenInteractive::TerminalOutput();
    auto ui = ftxui::Make<TuringUI>(screen.ExitLoopClosure(), &screen);
    try{
        if(argc == 2){
            TuringSave save;
            save.fileName = argv[1];

            const tao::json::value v = tao::json::from_file(argv[1]);
            save.comm = v.as<std::string> ("comm");
            save.alph = TURING_EMPTY_STR + ftxui::to_wstring(v.as<std::string>("alph"));
            std::vector<int> tapePos = getAsArray<int>(v, "tape", "pos");
            std::vector<std::string> tapeChar = getAsArray<std::string>(v, "tape", "sym");

            std::vector<std::string> oldSym = getAsArray<std::string>(v, "table", "oldSym");
            std::vector<int> oldSt = getAsArray<int>(v, "table", "oldSt");
            std::vector<int> newSt = getAsArray<int>(v, "table", "newSt");
            std::vector<std::string> newSym = getAsArray<std::string>(v, "table", "newSym");
            std::vector<std::string> direct = getAsArray<std::string>(v, "table", "direct");
            for(int i = 0; i < oldSym.size(); i++){
                save.table[std::make_pair(oldSt[i], wchar_t(oldSym[i][0]))] = TuringTurn(
                    oldSt[i],
                    wchar_t(oldSym[i][0] == '#' ? TURING_EMPTY : oldSym[i][0]),
                    newSt[i],
                    wchar_t(newSym[i][0] == '#' ? TURING_EMPTY : newSym[i][0]),
                    pickDirect(wchar_t(direct[i][0]))
                );
            }
            for(int i = 0; i < tapePos.size(); i++){
                save.tape[tapePos[i]] = wchar_t(tapeChar[i][0]);
            }
            ui->loadSave(save);
        } else if(argc != 1){
            std::cout << "Слишком много аргументов. Введите только имя программы или имя программы и имя файла для открытия\n";
            exit(1);
        }
        screen.Loop(ui);
    } catch(std::exception &e){
        std::cout << "Возникла ошибка: " << e.what() << "\n";
    }

    return 0;
}
