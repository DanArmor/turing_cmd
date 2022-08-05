#include "translate.hpp"

std::string TRANSLATE_EMPTY_STR = "";

extern std::string TURING_EMPTY_STR;

std::map<std::string, std::vector<std::string>> LANGUAGE = 
{
    {"Title_help", {
        "Turing machine", 
        "Машина Тьюринга - справка"
    }},
    {"Help_1", {
        "This program provides a Turing machine functionality\n"
        "Hotkeys:\n"
        "CTRL-LEFT - move 5 cells to the left\n"
        "CTRL-RIGHT - move 5 cells to the right\n"
        "CTRL-UP - add new state to the table\n"
        "CTRL-DOWN - remove last state from the table\n"
        "F5 - step of the execution\n"
        "F6 - run/pause of the continuous execution\n"
        "F7 - reset to the start state\n"
        "F9 - help\n"
        "F10 - save machine to the file\n"
        " \n"
        "There is state indicator in the left upper corner\n"
        "Next states are provided:\n"
        ,
        "Данная программа предназначена для работы с Машиной Тьюринга.\n"
        "Горячие клавиши:\n"
        "CTRL-LEFT - сместиться по ленте на 5 ячеек влево\n"
        "CTRL-RIGHT - сместиться по ленте на 5 ячеек вправо\n"
        "CTRL-UP - добавить новое состояние в таблицу\n"
        "CTRL-DOWN - убрать последнее состояние из таблицы\n"
        "F5 - шаг выполнения\n"
        "F6 - запуск/пауза непрерывного выполнения\n"
        "F7 - возврат в изначальное состояние\n"
        "F9 - справка\n"
        "F10 - сохранить машину в файл\n"
        " \n"
        "Слева сверху располагается индикатор состояния программы\n"
        "Возможны следующие состояния программы\n"
    }},
    {"Help_state_start", {
        "Start state of the machine",
        "Начальное состояние программы"
    }},
    {"Help_state_step", {
        "Step by step execution",
        "Исполнение по шагам"
    }},
    {"Help_state_running_in_process", {
        "Continuous execution. In process",
        "Непрерывное выполнение. В процессе"
    }},
    {"Help_state_running_pause", {
        "Continuous execution. Pause",
        "Непрерывное выполнение. Пауза"
    }},
    {"Help_state_stop", {
        "End state. Need to reset",
        "Конечное состояние. Требуется сброс"
    }},
    {"Help_2", {
        "To the right of help button there is an input field for the save file name\n"
        "After you press Enter the table of states, tape, alphabet and comment will be saved to the file\n"
        "as they were presented in Start state\n"
        " \n"
        "To load from file you need to start a program with save-file name as argument\n"
        "JSON is used as save format. It can be edited by hand\n"
        " \n"
        "Symbol " + TURING_EMPTY_STR + " is an empty symbol of Machine.\n"
        "Buttons to the left and right of the tape are moving your view by 5 cells\n"
        ,
        "Правее кнопки справки находится поле для ввода имени файла.\n"
        "При нажатии кнопки Enter будет произведено сохранение текущей таблицы состояний\n"
        "и ленты в состоянии Start state в файл, а так же комментарий и алфавит\n"
        " \n"
        "Для загрузки Машины Тьюринга из файла выйдите из программы и запустите, указав имя файла после имени программы.\n"
        "Формат сохранений json. При необходимости, вы можете редактировать их вручную, соблюдая структуру\n"
        "Для примера структуры попробуйте сохранить одну из своих Машин\n"
        " \n"
        "Символ " + TURING_EMPTY_STR + " обозначает пустой символ Машины.\n"
        "Кнопки слева и справ от ленты перемещают ваше поле зрение на 5 ячеек.\n"
    }},
    {"Help_button_step", {
        "Step of the execution",
        "Шаг выполнения машины"
    }},
    {"Help_button_run", {
        "Run of the continuous execution",
        "Запуск непрерывного выполнения"
    }},
    {"Help_button_pause", {
        "Pause of the continuous execution",
        "Пауза непрерывного выполнения"
    }},
    {"Help_button_reset", {
        "Reset to the start state",
        "Сброс в начальное состояние"
    }},
    {"Help_3", {
        "In Alphabet field inter symbols after " + TURING_EMPTY_STR + "\n"
        "If you change alphabet - the table of states will be cleared - be careful\n"
        " \n"
        "Comment field for notes\n"
        " \n"
        "Below it there is table of states and indicator of the current state.\n"
        "Adding or removing states deal no effect to the other entries of the table\n"
        "Alphabet symbols are placed by lines and states by columns.\n"
        "On their intersection you need to write: a symbol to place, direction to move and number of new state\n"
        "Example. If you meet empty symbol, when you are in state 0 - place a symbol '1', move to the left and change state to '0'\n"
        ,
        "В поле алфавита вводите символы после " + TURING_EMPTY_STR + "\n"
        "При изменении алфавита таблица полностью сбрасывается - запишите полностью алфавит заранее.\n"
        " \n"
        "Поле комментария служит для заметок пользователя\n"
        " \n"
        "Ниже находится таблица состояний и индикатор текущего состояния.\n"
        "Добавление и удаление новых состояний кнопками Add и Remove не влияет на имеющиеся записи таблицы состояний\n"
        "Слева указаны символы алфавита, сверху - номер состояния.\n"
        "На пересечении вписывается символ, который необходимо поставить, направление движения головки и новое состояние\n"
        "Пример. При встрече пустого символа в состоянии 0 поставить символ 1, сдвинуть головку влево и перейти в состояние 0\n"
    }},
    {"Help_4",{
        "Possible directions: < left, > right, | no move\n"
        "If there is no symbol to place it is treated as 'place an empty symbol'"
        "If there is no new state in cell table it is treated as 'change state to end state'"
        " \n"
        "Example. Place empty symbol, move to the left and change state to the end state: \"<\"\n"
        ,
        "Возможные движения головки: < влево, > вправо, | не двигаться\n"
        "Отсутствие символа для установки в ячейке означает, что поставится пустой символ\n"
        "Отсутствие нового состояния в записи таблицы обозначает переход в конечное состояние (-1)\n"
        " \n"
        "Пример. Установить пустой символ, сдвинуться влево и перейти в конечное состояние: \"<\"\n"
    }}
};

std::string &locale_getText(std::string const &key, LANG lang){
    
    if(LANGUAGE.count(key) == 0){
    #ifdef DEBUG
        DEBUG_write("No string with key " + key + " in locale");
    #else
        exit(4);
    #endif
        return TRANSLATE_EMPTY_STR;
    }
    return LANGUAGE[key][lang];
}