#include "turingUI.hpp"

#include <chrono>


TuringCellUI::TuringCellUI(int number_) : number(number_) { input = ftxui::Input(&str, L""); Add(input);}
TuringCellUI::TuringCellUI(int number_, bool isTop_) : isTop(isTop_), number(number_) { input = ftxui::Input(&str, L""); Add(input);}

TuringTurn TuringCellUI::getTurn(){
    wchar_t newSymbol = TURING_EMPTY;
    int newState = -1;
    TuringDirection direction = TuringDirection::NoMove;
    int whereToPickFrom = 0;
    if(str[0] != '<' && str[0] != '>' && str[0] != '|'){
        newSymbol = str[0];
        direction = pickDirect(str[1]);
        whereToPickFrom = 2;
    } else{
        direction = pickDirect(str[0]);
        whereToPickFrom = 1;
    }
    if(str.size() != 1){
        newState = std::stoi(str.substr(whereToPickFrom));
    }
    return TuringTurn(newState, newSymbol, direction);
}

bool TuringCellUI::containStr(){
    return str.size() != 0;
}

ftxui::Element TuringCellUI::Render(){
    std::vector<ftxui::Element> elems;
    if(isTop)
        elems.push_back(ftxui::vbox({
            ftxui::text(L"Q" + std::to_wstring(number)),
            ftxui::separator(),
            input->Render() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 5)})
        );
    else
        elems.push_back(input->Render() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 5));
    elems.push_back(ftxui::separator());
    return ftxui::hbox(elems);
}

bool TuringCellUI::OnEvent(ftxui::Event event){
    return this->ChildAt(0)->OnEvent(event);
}

// ROW UI

TuringRowUI::TuringRowUI() { Add(ftxui::Container::Horizontal({})); }
TuringRowUI::TuringRowUI(bool isTop_) : isTop(isTop_) { Add(ftxui::Container::Horizontal({})); }

void TuringRowUI::addCol(){
    cells.push_back(ftxui::Make<TuringCellUI>(size(), isTop));
    ChildAt(0)->Add(cells.back());
}
void TuringRowUI::removeCol(){
    if(cells.size() != 0){
        ChildAt(0)->ChildAt(ChildAt(0)->ChildCount()-1)->Detach();
        cells.pop_back();
    }
}

int TuringRowUI::size(void){
    return cells.size();
}

bool TuringRowUI::containStr(int i){
    return cells[i]->containStr();
}

ftxui::Element TuringRowUI::Render(){
    std::vector<ftxui::Element> elems;
    for(int i = 0; i < cells.size(); i++){
        elems.push_back(cells[i]->Render());
        elems.push_back(ftxui::separator());
    }
    return ftxui::hbox(elems) | ftxui::border;
}

TuringTurn TuringRowUI::getTurn(int i){
    return cells[i]->getTurn();
}

bool TuringRowUI::OnEvent(ftxui::Event event){
    return this->ChildAt(0)->OnEvent(event);
}

// TABLE UI
TuringTableUI::TuringTableUI() {
    Add(ftxui::Container::Vertical({}));
}

ftxui::Element TuringTableUI::Render() {
    std::vector<ftxui::Element> elems;
    for(int i = 0; i < rowsComponents.size(); i++){
        elems.push_back(ftxui::hbox({
            ftxui::text(std::wstring{alph[i]}), rowsComponents[i]->Render()
        }));
    }
    return ftxui::vbox(elems) | ftxui::frame;
}

bool TuringTableUI::OnEvent(ftxui::Event event) {
    return ChildAt(0)->OnEvent(event);
}

int TuringTableUI::cols(void) {
    if(rows() == 0){
        return 0;
    } else{
        return rowsComponents[0]->size();
    }
}

int TuringTableUI::rows(void) { return rowsComponents.size(); }

void TuringTableUI::removeRow(void) {}

void TuringTableUI::removeCol(void) {
    for(int i = 0; i < rowsComponents.size(); i++){
        rowsComponents[i]->removeCol();
    }
}

std::vector<TuringTurn> TuringTableUI::getTurns(void){
    std::vector<TuringTurn> turns;
    for(int i = 0; i < rows(); i++){
        for(int j = 0; j < cols(); j++){
            if(rowsComponents[i]->containStr(j)){
                TuringTurn turn = rowsComponents[i]->getTurn(j);
                turn.oldState = j;
                turn.oldSymbol = alph[i];
                turns.push_back(turn);
            }
        }
    }
    return turns;
}

void TuringTableUI::addRow(wchar_t c, bool isTop_) {
    rowsComponents.push_back(ftxui::Make<TuringRowUI>(isTop_));
    ChildAt(0)->Add(rowsComponents.back());
}

void TuringTableUI::addCol(void) {
    for(int i = 0; i < rowsComponents.size(); i++){
        rowsComponents[i]->addCol();
    }
}

void TuringTableUI::updateTable(std::wstring alph_){
    ChildAt(0)->DetachAllChildren();
    rowsComponents.clear();
    for(int i = 0; i < alph_.size(); i++){
        addRow(alph_[i], i == 0);
    }
    alph = alph_;
}

// TAPE UI
TuringTapeUI::TuringTapeUI(int size_) {
    size = size_;
    leftIndex = -(size_ / 2);
    tapeStrs.resize(size);
    for (int i = 0; i < size; i++) {
        tapeInputs.push_back(ftxui::Input(&tapeStrs[i], &TURING_EMPTY_STR));
    }
    Add(ftxui::Container::Horizontal(tapeInputs));
}

void TuringTapeUI::setLeftIndex(int pos){
    leftIndex = pos;
}

bool TuringTapeUI::isValidPos(int pos) {
    return pos >= leftIndex && pos < leftIndex + size;
}

int TuringTapeUI::toLocalPos(int pos) { return pos - leftIndex; }

void TuringTapeUI::setChar(wchar_t c, int pos) {
    if (!isValidPos(pos)) return;
    pos = toLocalPos(pos);
    if(c == TURING_EMPTY)
        tapeStrs[pos] = L"";
    else
        tapeStrs[pos] = c;
}

std::vector<std::wstring> &TuringTapeUI::getStrs(void) { return tapeStrs;}

void TuringTapeUI::setPositionAbsolute(int pos) { positionAbsolute = pos; }

ftxui::Element TuringTapeUI::Render() {
    std::vector<ftxui::Element> cells;
    auto tapeCursor = [this, &cells](int x, auto elem) {
        if (x + this->leftIndex == this->positionAbsolute) {
            return ftxui::vbox(
                {elem,
                 ftxui::text(L" ▲ ") | ftxui::color(ftxui::Color::DarkOrange3),
                 ftxui::text(L" | ") |
                     ftxui::color(ftxui::Color::DarkOrange3)});
        } else {
            return elem;
        }
    };
    for (int i = 0; i < size; i++) {
        cells.push_back(
            ftxui::vbox({ftxui::text(std::to_wstring(leftIndex + i)) |
                             ftxui::color(ftxui::Color::DarkOrange),
                         ftxui::separatorHeavy(),
                         tapeCursor(i, tapeInputs[i]->Render())}) |
            ftxui::borderLight |
            ftxui::size(ftxui::WIDTH, ftxui::EQUAL, CELL_SIZE));
    }
    return ftxui::hbox({cells});
}

int TuringTapeUI::getSize(void) { return size; }
int TuringTapeUI::getLeftIndex(void) { return leftIndex; }

bool TuringTapeUI::OnEvent(ftxui::Event event) {
    return ChildAt(0)->OnEvent(event);
}

void TuringUI::updateComponents(void){
    auto tape = this->machine.getTape();
    int leftIndex = this->tapeComponent->getLeftIndex();
    int size = this->tapeComponent->getSize();
    for(int i = 0; i < size; i++){
        this->tapeComponent->setChar(tape.getChar(i + leftIndex), i + leftIndex);
    }
    this->tapeComponent->setPositionAbsolute(this->machine.getCurPosition());
}

void TuringUI::makeTurn(void){
    this->machine.clearTurns();
    this->machine.setAlph(this->alphStr);
    auto v = this->tableComponent->getTurns();
    for(auto turn : v){
        this->machine.addTurn(turn);
    }
    this->machine.makeTurn();
    updateComponents();
}

void TuringUI::updateStateTape(void){
    auto v = this->tapeComponent->getStrs();
    int leftIndex = this->tapeComponent->getLeftIndex();
    int size = this->tapeComponent->getSize();
    for(int i = 0; i < size; i++){
        this->state.tape.setChar(i+leftIndex, v[i].size() ? v[i][0] : TURING_EMPTY);
    }
}

// MAIN UI
TuringUI::TuringUI(std::function<void()> quitFunc, ftxui::ScreenInteractive *scr_) {
    quit = quitFunc;
    scr = scr_;

    status.status = TuringUIStatus::START;
    alphStr = TURING_EMPTY_STR;

    state.position = 0;
    state.currState = 0;

    machine.loadState(state);

    helpButton = ftxui::Button("Help", [&](){
        if(!this->isShowingHelp){
            this->DetachAllChildren();
            this->Add(this->toHelpDisplay);
            if(this->isRunning){
                this->isRunning = false;
                this->status.status = TuringUIStatus::RUNNING_OFF;
            }
            this->isShowingHelp = true;
        }
    });
    
    moveTapeLeftButton = ftxui::Button("←Move", [&]() {
        if(this->isResetState){
            this->updateStateTape();
            this->machine.loadState(this->state);
            this->tapeComponent->setLeftIndex(this->tapeComponent->getLeftIndex() - 5);
            this->updateComponents();
        } else{
            this->tapeComponent->setLeftIndex(this->tapeComponent->getLeftIndex() - 5);
            this->updateComponents();
        }
    });
    moveTapeRightButton = ftxui::Button("Move➔", [&]() {
        if(this->isResetState){
            this->updateStateTape();
            this->machine.loadState(this->state);
            this->tapeComponent->setLeftIndex(this->tapeComponent->getLeftIndex() + 5);
            this->updateComponents();
        } else{
            this->tapeComponent->setLeftIndex(this->tapeComponent->getLeftIndex() + 5);
            this->updateComponents();
        }
    });


    stepButton = ftxui::Button("Step", [this]() {
        this->status.status = TuringUIStatus::STEP;
        if(this->isResetState){
            this->isResetState = false;
            this->updateStateTape();
            this->machine.loadState(this->state);
            this->updateComponents();
        }
        if(!this->machine.isDone()){
            this->makeTurn();
        } else if(this->status.status != TuringUIStatus::STOP) {
            this->status.status = TuringUIStatus::STOP;
        }
    });

    runButton = ftxui::Button("Run", [&]() {
        this->status.status = TuringUIStatus::RUNNING_ON;
        if(this->isResetState){
            this->isResetState = false;
            this->updateStateTape();
            this->machine.loadState(this->state);
            this->updateComponents();
        }
        if(this->isRunning){
            this->status.status = TuringUIStatus::RUNNING_OFF;
            this->isRunning = false;
        } else{
            this->isRunning = true;
            std::thread runFunc([&]{
                using namespace std::chrono_literals;
                const auto refresh_time = 0.4s;
                while(this->isRunning){
                    if(!this->machine.isDone()){
                        this->makeTurn();
                        std::this_thread::sleep_for(refresh_time);
                    } else if(this->status.status != TuringUIStatus::STOP) {
                        this->status.status = TuringUIStatus::STOP;
                        this->isRunning = false;
                    }
                    this->refresh();
                }
            });
            runFunc.detach();
        }
    });

    resetButton = ftxui::Button("Reset", [&](){
        this->status.status = TuringUIStatus::START;
        this->isResetState = true;
        this->isRunning = false;
        this->machine.clear();
        this->machine.loadState(state);
        updateComponents();
    });

    fileInput = ftxui::Input(&fileStr, "");

    ftxui::InputOption alphInputOption;
    alphInputOption.on_change = [this](void) {
        this->needToUpdateTable = true;
    };
    alphInput = ftxui::Input(&alphStr, L"", alphInputOption);
    commentInput = ftxui::Input(&commentStr, "");

    addButton = ftxui::Button("Add", [&]() { this->tableComponent->addCol(); });
    removeButton =
        ftxui::Button("Remove", [&]() { this->tableComponent->removeCol(); });

    tapeComponent = ftxui::Make<TuringTapeUI>(25);
    tableComponent = ftxui::Make<TuringTableUI>();

    toMainDisplay = ftxui::Container::Vertical(
        {ftxui::Container::Horizontal({helpButton, fileInput}),
         ftxui::Container::Horizontal(
             {moveTapeLeftButton, tapeComponent, moveTapeRightButton}),
         ftxui::Container::Horizontal(
             {stepButton, runButton, resetButton, alphInput, commentInput}),
         ftxui::Container::Horizontal({addButton, removeButton}),
         tableComponent});
    toHelpDisplay = ftxui::Button("Close help", [&]{
        this->DetachAllChildren();
        this->Add(toMainDisplay);
        this->isShowingHelp = false;
    });
    Add(toMainDisplay);
}

void TuringUI::refresh(void){
    scr->PostEvent(ftxui::Event::Custom);
}

ftxui::Element TuringUI::Render() {
    if(isShowingHelp){
        auto toShowStatus = [&](TuringUIStatus::Status s){
            TuringUIStatus status;
            status.status = s;
            return status.Render();
        };
        return ftxui::window(ftxui::text(L"Машина Тьюринга - справка") | ftxui::bold,
        ftxui::vbox({
            toHelpDisplay->Render() | ftxui::color(ftxui::Color::Red) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 10),
            ftxui::paragraphAlignLeft("Данная программа предназначена для работы с Машиной Тьюринга."),
            ftxui::paragraphAlignLeft("Слева сверху располагается индикатор состояния программы"),
            ftxui::paragraphAlignLeft("Возможны следующие состояния программы"),
            ftxui::hbox({
                ftxui::vbox({
                    ftxui::vbox({
                        ftxui::text(L"Начальное состояние программы"),
                        ftxui::separator(),
                        toShowStatus(TuringUIStatus::START)
                    }) | ftxui::border,
                    ftxui::vbox({
                        ftxui::text(L"Исполнение по шагам"),
                        ftxui::separator(),
                        toShowStatus(TuringUIStatus::STEP)
                    }) | ftxui::border
                }),
                ftxui::vbox({
                    ftxui::vbox({
                        ftxui::text(L"Непрерывное исполнение. В процессе"),
                        ftxui::separator(),
                        toShowStatus(TuringUIStatus::RUNNING_ON)
                    }) | ftxui::border,
                    ftxui::vbox({
                        ftxui::text(L"Непрерывное исполнение. Пауза"),
                        ftxui::separator(),
                        toShowStatus(TuringUIStatus::RUNNING_OFF)
                    }) | ftxui::border,
                }),
                    ftxui::vbox({
                        ftxui::text(L"Остановка в случае достижения"),
                        ftxui::text(L"конечного состояния. Требуется сброс"),
                        ftxui::separator(),
                        toShowStatus(TuringUIStatus::STOP)
                    }) | ftxui::border,
            }),
            ftxui::paragraphAlignLeft("Правее кнопки справки находится поле для ввода имени файла."),
            ftxui::paragraphAlignLeft("При нажатии кнопки Enter будет произведено сохранение текущей таблицы состояний."),
            ftxui::paragraphAlignLeft("и ленты в состоянии Start state в файл, а так же комментарий"),
            ftxui::text(""),
            ftxui::paragraphAlignLeft("Для загрузки Машины Тьюринга из файла выйдите из программы и запустите, указав имя файла после имени программы."),
            ftxui::paragraphAlignLeft("Формат сохранений json. При необходимости, вы можете редактировать их вручную, соблюдая структуру"),
            ftxui::paragraphAlignLeft("Для примера структуры попробуйте сохранить одну из своих Машин"),
            ftxui::text(""),
            ftxui::text(L"Символ " + TURING_EMPTY_STR + L" обозначает <пустой> символ Машины."),
            ftxui::paragraphAlignLeft("Кнопки слева и справ от ленты перемещают ваше поле зрение на 5 ячеек."),
            ftxui::hbox({
                ftxui::vbox({
                    ftxui::vbox({
                        ftxui::text(L"Шаг выполнения машины"),
                        ftxui::separator(),
                        ftxui::text(L"Step") | ftxui::color(ftxui::Color::SkyBlue1)
                    }) | ftxui::border,
                    ftxui::vbox({
                        ftxui::text(L"Запуск непрерывного выполнения"),
                        ftxui::separator(),
                        ftxui::text(L"Run") | ftxui::color(ftxui::Color::SkyBlue1)
                    }) | ftxui::border
                }),
                ftxui::vbox({
                    ftxui::vbox({
                        ftxui::text(L"Пауза непрерывного выполнения"),
                        ftxui::separator(),
                        ftxui::text(L"Run") | ftxui::color(ftxui::Color::DarkRed)
                    }) | ftxui::border,
                    ftxui::vbox({
                        ftxui::text(L"Сброс в первоначальное состояние"),
                        ftxui::separator(),
                        ftxui::text(L"Reset") | ftxui::color(ftxui::Color::SkyBlue1)
                    }) | ftxui::border

                })
            }),
            ftxui::text(L"В поле алфавита вводите символы после " + TURING_EMPTY_STR),
            ftxui::paragraphAlignLeft("При изменении алфавита таблица полностью сбрасывается - запишите полностью алфавит заранее."),
            ftxui::text(""),
            ftxui::paragraphAlignLeft("Поле комментария служит для заметок пользователя"),
            ftxui::text(""),
            ftxui::paragraphAlignLeft("Ниже находится таблица состояний и индикатор текущего состояния."),
            ftxui::paragraphAlignLeft("Добавление и удаление новых состояний кнопками Add и Remove не влияет на имеющиеся записи таблицы состояний"),
            ftxui::paragraphAlignLeft("Слева указаны символы алфавита, сверху - номер состояния."),
            ftxui::paragraphAlignLeft("На пересечении вписывается символ, который необходимо поставить, направление движения головки и новое состояние"),
            ftxui::paragraphAlignLeft("Пример. При встрече пустого символа в состоянии 0 поставить символ 1, сдвинуть головку влево и перейти в состояние 0"),
            ftxui::vbox(
                ftxui::text("Q0"),
                ftxui::hbox({
                    ftxui::text(TURING_EMPTY_STR),
                    ftxui::hbox({ftxui::text("1<0")}) | ftxui::border
                })
            ),
            ftxui::paragraphAlignLeft("Возможные движения головки: < влево, > вправо, | не двигаться"),
            ftxui::paragraphAlignLeft("Отсутствие символа для установки в ячейке означает, что поставится пустой символ"),
            ftxui::paragraphAlignLeft("Отсутствие нового состояния в записи таблицы обозначает переход в конечное состояние (-1)"),
            ftxui::text(""),
            ftxui::paragraphAlignLeft("Пример. Установить пустой символ, сдвинуться влево и перейти в конечное состояние: \"<\""),
            ftxui::paragraphAlignLeft("Отсутствие указаний к действию при определенной комбинации в таблице переходов означает <поставить пустой символ, не двигаться и перейти в конечное состояние>"),
        }) | ftxui::yframe
        );
    } else{
        auto tapeAndButtons = ftxui::hbox(
            {moveTapeLeftButton->Render() | ftxui::color(ftxui::Color::DarkOrange),
            tapeComponent->Render(),
            moveTapeRightButton->Render() |
        ftxui::color(ftxui::Color::DarkOrange)});

        auto runButtonColor = [&](){
            if(this->isRunning){
                return ftxui::color(ftxui::Color::DarkRed);
            } else{
                return ftxui::color(ftxui::Color::SkyBlue1);
            }
        };

        auto mainControl = ftxui::hbox(
            {stepButton->Render() | ftxui::color(ftxui::Color::SkyBlue1),
            runButton->Render() | runButtonColor(),
            resetButton->Render() | ftxui::color(ftxui::Color::SkyBlue1),
            ftxui::vbox({ftxui::text(L"Алфавит"), ftxui::separatorLight(),
                        alphInput->Render()}) |
                ftxui::borderLight,
            ftxui::vbox({ftxui::text(L"Комментарий"), ftxui::separatorLight(),
                        commentInput->Render()}) |
                ftxui::borderLight});

        auto buttonsTable =
            ftxui::hbox({addButton->Render() | ftxui::color(ftxui::Color::Green),
                        removeButton->Render() | ftxui::color(ftxui::Color::Red),
                        ftxui::vbox({
                            ftxui::text(L"Текущее состояние"), ftxui::separatorLight(), ftxui::text(std::to_string(machine.getCurState()))
                            }) | ftxui::borderLight
                        });

        auto mainBox =
            ftxui::vbox({ftxui::vbox({ftxui::text(L"Машина Тьюринга") | ftxui::bold,
            ftxui::hbox({status.Render() | ftxui::border,
            helpButton->Render()
            | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 6)
            | ftxui::color(ftxui::Color::Yellow1),
            ftxui::vbox({ftxui::text(L"Имя файла"), fileInput->Render()}) | ftxui::border}),
            }),
                        ftxui::separatorHeavy(), tapeAndButtons, mainControl,
                        buttonsTable, tableComponent->Render()}) |
            ftxui::border;

        return mainBox;
    }
}

bool TuringUI::OnEvent(ftxui::Event event) {
    if (event == ftxui::Event::Escape) {
        this->isRunning = false;
        quit();
        return true;
    }

    bool answer = ChildAt(0)->OnEvent(event);

    if(alphStr.size() == 0 || alphStr[0] != TURING_EMPTY){
        alphStr = TURING_EMPTY_STR + alphStr;
    }

    if (needToUpdateTable) {
        needToUpdateTable = false;
        this->tableComponent->updateTable(alphStr);
    }

    return answer;
}

void TuringUI::loadSave(TuringSave save){
    alphStr = save.alph;
    commentStr = save.comm;
    fileStr = save.fileName;

    state.currState = 0;
    state.position = 0;
    for(auto it : save.tape){
        state.tape.setChar(it.first, it.second);
    }
    machine.loadState(state);
    this->tableComponent->updateTable(this->alphStr);

    int cols = -1;
    for(auto it : save.table){
        while(cols < it.first.first){
            this->tableComponent->addCol();
            cols++;
        }
    }



    updateComponents();
}