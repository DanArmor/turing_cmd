#include <unistd.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "turing.hpp"
#include "turingUI.hpp"

#include "nlohmann/json.hpp"

using namespace nlohmann;

#ifdef DEBUG
std::ofstream DEBUG_output;
#endif

int main(int argc, char **argv) {
    #ifdef DEBUG
    DEBUG_output.open("DEBUG_log.txt");
    if(!DEBUG_output){
        std::cout << "Failed to create a debug log file";
        exit(3);
    }
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    DEBUG_write("Start of debug session: " << std::put_time(&tm, "%d-%m-%Y %H-%M-%S"));
    #endif

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
            save.alph = TURING_EMPTY_STR + j["alph"].get<std::string>();


            auto tapePos = j["tape"]["pos"].get<std::vector<int>>();
            auto tapeChar = j["tape"]["sym"].get<std::vector<std::string>>();

            auto oldSym = j["table"]["oldSym"].get<std::vector<std::string>>();
            auto oldSt = j["table"]["oldSt"].get<std::vector<int>>();
            auto newSt = j["table"]["newSt"].get<std::vector<int>>();
            auto newSym = j["table"]["newSym"].get<std::vector<std::string>>();
            auto direct = j["table"]["direct"].get<std::vector<std::string>>();

            for(int i = 0; i < oldSym.size(); i++){
                save.table[std::make_pair(oldSt[i], char(oldSym[i][0]))] = TuringTurn(
                    oldSt[i],
                    oldSym[i][0],
                    newSt[i],
                    newSym[i][0],
                    pickDirect(direct[i][0])
                );
            }
            for(int i = 0; i < tapePos.size(); i++){
                save.tape[tapePos[i]] = char(tapeChar[i][0]);
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
