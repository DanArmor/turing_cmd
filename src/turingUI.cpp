#include "turingUI.hpp"

#include <chrono>
#include <sstream>
#include <fstream>


TuringCellUI::TuringCellUI(int number_) : number(number_) { input = ftxui::Input(&str, ""); Add(input);}
TuringCellUI::TuringCellUI(int number_, bool isTop_) : isTop(isTop_), number(number_) { input = ftxui::Input(&str, ""); Add(input);}

TuringTurn TuringCellUI::getTurn(){
    char newSymbol = TURING_EMPTY;
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
            ftxui::text("Q" + std::to_string(number)),
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

void TuringCellUI::loadSaved(std::string str_){
    this->str = str_;
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

void TuringRowUI::loadSavedCell(int col, std::string str){
    this->cells[col]->loadSaved(str);
}

// TABLE UI
TuringTableUI::TuringTableUI() {
    Add(ftxui::Container::Vertical({}));
}

ftxui::Element TuringTableUI::Render() {
    std::vector<ftxui::Element> elems;
    for(int i = 0; i < rowsComponents.size(); i++){
        elems.push_back(ftxui::hbox({
            ftxui::text(std::string{alph[i]}), rowsComponents[i]->Render()
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

void TuringTableUI::addRow(char c, bool isTop_) {
    rowsComponents.push_back(ftxui::Make<TuringRowUI>(isTop_));
    ChildAt(0)->Add(rowsComponents.back());
}

void TuringTableUI::addCol(void) {
    for(int i = 0; i < rowsComponents.size(); i++){
        rowsComponents[i]->addCol();
    }
}

void TuringTableUI::updateTable(std::string alph_){
    ChildAt(0)->DetachAllChildren();
    rowsComponents.clear();
    for(int i = 0; i < alph_.size(); i++){
        addRow(alph_[i], i == 0);
    }
    alph = alph_;
}

void TuringTableUI::loadSavedCell(int row, int col, std::string str){
    this->rowsComponents[row]->loadSavedCell(col, str);
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

void TuringTapeUI::setChar(char c, int pos) {
    if (!isValidPos(pos)) return;
    pos = toLocalPos(pos);
    if(c == TURING_EMPTY)
        tapeStrs[pos] = "";
    else
        tapeStrs[pos] = c;
}

std::vector<std::string> &TuringTapeUI::getStrs(void) { return tapeStrs;}

void TuringTapeUI::setPositionAbsolute(int pos) { positionAbsolute = pos; }

ftxui::Element TuringTapeUI::Render() {
    std::vector<ftxui::Element> cells;
    auto tapeCursor = [this, &cells](int x, auto elem) {
        if (x + this->leftIndex == this->positionAbsolute) {
            return ftxui::vbox(
                {elem,
                 ftxui::text(" ▲ ") | ftxui::color(ftxui::Color::DarkOrange3),
                 ftxui::text(" | ") |
                     ftxui::color(ftxui::Color::DarkOrange3)});
        } else {
            return elem;
        }
    };
    for (int i = 0; i < size; i++) {
        cells.push_back(
            ftxui::vbox({ftxui::text(std::to_string(leftIndex + i)) |
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
    localeButton = ftxui::Button("Change lang", [&](){
        if(this->language == LANG::L_EN){
            this->language = LANG::L_RU;
        } else{
            this->language = LANG::L_EN;
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
        if(this->status.status != TuringUIStatus::RUNNING_ON) {
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
        if(this->isResetState){
            this->updateStateTape();
            this->machine.loadState(this->state);
            this->updateComponents();
            this->refresh();
        } else{
            this->status.status = TuringUIStatus::START;
            this->isResetState = true;
            this->isRunning = false;
            this->machine.clear();
            this->machine.loadState(state);
            updateComponents();
        }
    });

    ftxui::InputOption saveOption;
    saveOption.on_enter = [this](void){
        if(this->isResetState){
            this->updateStateTape();
            this->machine.loadState(this->state);
            this->updateComponents();
        }
        this->saveToFile();
        if(this->isErrorFile){
            this->lastSaveStatus = SaveStatus::INVALID;
        } else{
            this->lastSaveStatus = SaveStatus::OK;
        }
        this->wasTriedToSave = true;
    };
    fileInput = ftxui::Input(&fileStr, "", saveOption);


    ftxui::InputOption alphInputOption;
    alphInputOption.on_change = [this](void) {
        this->needToUpdateTable = true;
    };
    alphInput = ftxui::Input(&alphStr, "", alphInputOption);
    commentInput = ftxui::Input(&commentStr, "");

    addButton = ftxui::Button("Add", [&]() { this->tableComponent->addCol(); });
    removeButton =
        ftxui::Button("Remove", [&]() { this->tableComponent->removeCol(); });

    tapeComponent = ftxui::Make<TuringTapeUI>(25);
    tableComponent = ftxui::Make<TuringTableUI>();

    toMainDisplay = ftxui::Container::Vertical(
        {ftxui::Container::Horizontal({helpButton, fileInput, localeButton}),
         ftxui::Container::Horizontal(
             {moveTapeLeftButton, tapeComponent, moveTapeRightButton}),
         ftxui::Container::Horizontal(
             {stepButton, runButton, resetButton, alphInput, commentInput}),
         ftxui::Container::Horizontal({addButton, removeButton}),
         tableComponent});
    toHelpDisplay = ftxui::Container::Horizontal({ftxui::Button("Close help", [&]{
        this->DetachAllChildren();
        this->Add(toMainDisplay);
        this->isShowingHelp = false;
    }),
        localeButton
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
        return ftxui::window(ftxui::text(getText("Title_help")) | ftxui::bold,
        ftxui::vbox({
            ftxui::hbox({
                toHelpDisplay->Render() | ftxui::color(ftxui::Color::Red) | ftxui::size(ftxui::WIDTH, ftxui::LESS_THAN, 30)
            }),
            breakByLinesElement(
                getText("Help_1")
            ),
            ftxui::hbox({
                ftxui::vbox({
                    ftxui::vbox({
                        ftxui::text(getText("Help_state_start")),
                        ftxui::separator(),
                        toShowStatus(TuringUIStatus::START)
                    }) | ftxui::border,
                    ftxui::vbox({
                        ftxui::text(getText("Help_state_step")),
                        ftxui::separator(),
                        toShowStatus(TuringUIStatus::STEP)
                    }) | ftxui::border
                }),
                ftxui::vbox({
                    ftxui::vbox({
                        ftxui::text(getText("Help_state_running_in_process")),
                        ftxui::separator(),
                        toShowStatus(TuringUIStatus::RUNNING_ON)
                    }) | ftxui::border,
                    ftxui::vbox({
                        ftxui::text(getText("Help_state_running_pause")),
                        ftxui::separator(),
                        toShowStatus(TuringUIStatus::RUNNING_OFF)
                    }) | ftxui::border,
                }),
                    ftxui::vbox({
                        ftxui::text(getText("Help_state_stop")),
                        ftxui::separator(),
                        toShowStatus(TuringUIStatus::STOP)
                    }) | ftxui::border,
            }),
            breakByLinesElement(
                getText("Help_2")
            ),
            ftxui::hbox({
                ftxui::vbox({
                    ftxui::vbox({
                        ftxui::text(getText("Help_button_step")),
                        ftxui::separator(),
                        ftxui::text("Step") | ftxui::color(ftxui::Color::SkyBlue1)
                    }) | ftxui::border,
                    ftxui::vbox({
                        ftxui::text(getText("Help_button_run")),
                        ftxui::separator(),
                        ftxui::text("Run") | ftxui::color(ftxui::Color::SkyBlue1)
                    }) | ftxui::border
                }),
                ftxui::vbox({
                    ftxui::vbox({
                        ftxui::text("Help_button_pause"),
                        ftxui::separator(),
                        ftxui::text("Run") | ftxui::color(ftxui::Color::DarkRed)
                    }) | ftxui::border,
                    ftxui::vbox({
                        ftxui::text("Help_button_reset"),
                        ftxui::separator(),
                        ftxui::text("Reset") | ftxui::color(ftxui::Color::SkyBlue1)
                    }) | ftxui::border

                })
            }),
            breakByLinesElement(
                getText("Help_3")
            ),
            ftxui::vbox(
                ftxui::text("Q0"),
                ftxui::hbox({
                    ftxui::text(TURING_EMPTY_STR),
                    ftxui::hbox({ftxui::text("1<0")}) | ftxui::border
                })
            ),
            breakByLinesElement(
                getText("Help_4")
            )
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
            ftxui::vbox({ftxui::text("Alphabet"), ftxui::separatorLight(),
                        alphInput->Render()}) |
                ftxui::borderLight,
            ftxui::vbox({ftxui::text("Comment"), ftxui::separatorLight(),
                        commentInput->Render()}) |
                ftxui::borderLight});

        auto buttonsTable =
            ftxui::hbox({addButton->Render() | ftxui::color(ftxui::Color::Green),
                        removeButton->Render() | ftxui::color(ftxui::Color::Red),
                        ftxui::vbox({
                            ftxui::text("Current state"), ftxui::separatorLight(), ftxui::text(std::to_string(machine.getCurState()))
                            }) | ftxui::borderLight
                        });

        auto fileStatusRenderer = [&](){
            if(this->wasTriedToSave){
                std::thread fileStatusUpdate([&]{
                    using namespace std::chrono_literals;
                    const auto refresh_time = 2.0s;
                    this->wasTriedToSave = false;
                    std::this_thread::sleep_for(refresh_time);
                    this->refresh();
                });
                fileStatusUpdate.detach();
                if(this->lastSaveStatus == SaveStatus::OK){
                    return ftxui::text("Saving is done") | ftxui::color(ftxui::Color::Green1);
                } else if(this->lastSaveStatus == SaveStatus::INVALID){
                    return ftxui::text("Error during saving") | ftxui::color(ftxui::Color::Red1);
                }
            } else{
                return ftxui::text("File name");
            }
        };

        auto mainBox =
            ftxui::vbox({ftxui::vbox({ftxui::text("Turing machine") | ftxui::bold,
                ftxui::hbox({status.Render() | ftxui::border,
                    helpButton->Render()
                    | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 6)
                    | ftxui::color(ftxui::Color::Yellow1),
                    ftxui::vbox({fileStatusRenderer(), fileInput->Render()}) | ftxui::border
                })
            }),
                        ftxui::separatorHeavy(), tapeAndButtons, mainControl,
                        buttonsTable, tableComponent->Render()}) |
            ftxui::border;

        return mainBox;
    }
}

const ftxui::Event CTRL_LEFT = ftxui::Event::Special({27, 91, 49, 59, 53, 68});
const ftxui::Event CTRL_RIGHT = ftxui::Event::Special({27, 91, 49, 59, 53, 67});
const ftxui::Event CTRL_UP = ftxui::Event::Special({27, 91, 49, 59, 53, 65});
const ftxui::Event CTRL_DOWN = ftxui::Event::Special({27, 91, 49, 59, 53, 66});

bool TuringUI::OnEvent(ftxui::Event event) {
    if (event == ftxui::Event::Escape) {
        this->isRunning = false;
        quit();
        return true;
    }
    if( event == CTRL_LEFT){
        this->moveTapeLeftButton->OnEvent(ftxui::Event::Return);
        this->refresh();
        return true;
    }
    if( event == CTRL_RIGHT){
        this->moveTapeRightButton->OnEvent(ftxui::Event::Return);
        this->refresh();
        return true;
    }
    if( event == CTRL_UP){
        this->addButton->OnEvent(ftxui::Event::Return);
        this->refresh();
        return true;
    }
    if( event == CTRL_DOWN){
        this->removeButton->OnEvent(ftxui::Event::Return);
        this->refresh();
        return true;
    }
    if( event == ftxui::Event::F5){
        this->stepButton->OnEvent(ftxui::Event::Return);
        this->refresh();
        return true;
    }
    if( event == ftxui::Event::F6){
        this->runButton->OnEvent(ftxui::Event::Return);
        this->refresh();
        return true;
    }
    if( event == ftxui::Event::F7){
        this->resetButton->OnEvent(ftxui::Event::Return);
        this->refresh();
        return true;
    }
    if( event == ftxui::Event::F9){
        if(this->isShowingHelp){
            this->toHelpDisplay->OnEvent(ftxui::Event::Return);
        } else{
            this->helpButton->OnEvent(ftxui::Event::Return);
        }
        this->refresh();
        return true;
    }
    if( event == ftxui::Event::F10){
        this->fileInput->OnEvent(ftxui::Event::Return);
        this->refresh();
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

void TuringUI::saveToFile(void){
    std::fstream f(this->fileStr, std::ios::out);
    if(!f){
        this->isErrorFile = true;
        this->refresh();
        return;
    }else{
		this->isErrorFile = false;
        this->refresh();
	}

    auto alphToSave = this->alphStr.substr(1);
    auto commentToSave = this->commentStr;
    std::vector<int> tapePos;
    std::vector<std::string> tapeChar;
    if(this->isResetState == false){
        this->resetButton->OnEvent(ftxui::Event::Return);
        this->refresh();
    }
    for(auto it : this->state.tape.getTapeMap()){
        tapePos.push_back(it.first);
        tapeChar.push_back(std::string({it.second}));
    }

    std::vector<int> oldSt;
    std::vector<std::string> oldSym;
    std::vector<int> newSt;
    std::vector<std::string> newSym;
    std::vector<std::string> direction;
    std::vector<TuringTurn> tableTurns = this->tableComponent->getTurns();

    for(int i = 0; i < tableTurns.size(); i++){
        oldSt.push_back(tableTurns[i].oldState);
        if(tableTurns[i].oldSymbol == TURING_EMPTY){
            oldSym.push_back("#");
        }else{
            oldSym.push_back(std::string({tableTurns[i].oldSymbol}));
        }

        if(tableTurns[i].newSymbol == TURING_EMPTY){
            newSym.push_back("#");
        }else{
            newSym.push_back(std::string({tableTurns[i].newSymbol}));
        }

        newSt.push_back(tableTurns[i].newState);
        direction.push_back(pickDirectStr(tableTurns[i].direction));
    }

    json j;
    j["alph"] = alphToSave;
    j["comm"] = commentToSave;

    j["tape"]["pos"] = tapePos;
    j["tape"]["sym"] = tapeChar;
    
    j["table"]["oldSt"] = oldSt;
    j["table"]["oldSym"] = oldSym;
    j["table"]["newSt"] = newSt;
    j["table"]["newSym"] = newSym;
    j["table"]["direct"] = direction;

    f << std::setw(2) << j << "\n";
    f.close();
}

std::string &TuringUI::getText(std::string const &key){
    return locale_getText(key, language);
}

void TuringUI::loadSave(TuringSave save){
    alphStr = save.alph;

    needToUpdateTable = false;

    commentStr = save.comm;
    fileStr = save.fileName;

    state.currState = 0;
    state.position = 0;
    for(auto it : save.tape){
        state.tape.setChar(it.first, it.second);
    }

    machine.loadState(state);
    this->tableComponent->updateTable(this->alphStr);

    auto alphFind = [&](char c){
        for(int i = 0; i < alphStr.size(); i++){
            if(c == alphStr[i])
                return i;
        }
    };

    auto turnToStr = [](TuringTurn turn){
        std::string r = "";
        r += std::string{turn.newSymbol};
        r += pickDirectStr(turn.direction);
        r += std::to_string(turn.newState);
        return r;
    };

    int cols = -1;
    for(auto it : save.table){
        while(cols < it.first.first){
            this->tableComponent->addCol();
            cols++;
        }
        this->tableComponent->loadSavedCell(alphFind(it.first.second), it.first.first, turnToStr(it.second));
    }


    updateComponents();
}

std::vector<std::string> TuringUI::breakByLines(std::string str){
    std::istringstream ss(str);
    std::string t;
    std::vector<std::string> res;
    while(getline(ss, t)){
        res.push_back(t);
    }
    return res;
}

ftxui::Element TuringUI::breakByLinesElement(std::string str){
    std::vector<std::string> strs = breakByLines(str);
    ftxui::Elements list;
    for(int i = 0; i < strs.size(); i++){
        list.push_back(ftxui::hbox({ftxui::paragraph(strs[i])}) | ftxui::flex);
    }
    return ftxui::vbox(list) | ftxui::flex;
}