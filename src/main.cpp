#include <unistd.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "turing.hpp"
#include "turingUI.hpp"

#include "nlohmann/json.hpp"

using namespace nlohmann;

int main(int argc, char **argv) {
    auto screen = ftxui::ScreenInteractive::TerminalOutput();
    auto ui = ftxui::Make<TuringUI>(screen.ExitLoopClosure(), &screen);
    try{
        if(argc == 2){
            TuringSave save;
            save.fileName = argv[1];
            std::ifstream saveFileStream(argv[1]);
            if(!saveFileStream){
                std::cout << "Ошибка открытия файла";
                exit(2);
            }

            json j = json::parse(saveFileStream);

            save.comm = j["comm"];
            save.alph = TURING_EMPTY_STR + ftxui::to_wstring(j["alph"].get<std::string>());


            auto tapePos = j["tape"]["pos"].get<std::vector<int>>();
            auto tapeChar = j["tape"]["sym"].get<std::vector<std::string>>();

            auto oldSym = j["table"]["oldSym"].get<std::vector<std::string>>();
            auto oldSt = j["table"]["oldSt"].get<std::vector<int>>();
            auto newSt = j["table"]["newSt"].get<std::vector<int>>();
            auto newSym = j["table"]["newSym"].get<std::vector<std::string>>();
            auto direct = j["table"]["direct"].get<std::vector<std::string>>();

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
