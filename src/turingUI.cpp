#include "turingUI.hpp"

// ROW UI

TuringRowUI::TuringRowUI() { Add(ftxui::Container::Horizontal({})); }

void TuringRowUI::addCol(){
    strs.push_back(L"");
    inputs.push_back(ftxui::Input(strs.back(), L""));
    ChildAt(0)->Add(inputs.back());
}
void TuringRowUI::removeCol(){
    strs.pop_back();
    ChildAt(0)->ChildAt(ChildAt(0)->ChildCount()-1)->Detach();
    inputs.pop_back();
}

ftxui::Element TuringRowUI::Render(){
    std::vector<ftxui::Element> elems;
    for(int i = 0; i < strs.size(); i++){
        elems.push_back(inputs[i]->Render());
        elems.push_back(ftxui::separator());
    }
    return ftxui::hbox(elems) | ftxui::border;
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
        elems.push_back(rowsComponents[i]->Render());
    }
    return ftxui::vbox(elems);
}

bool TuringTableUI::OnEvent(ftxui::Event event) {
    return ChildAt(0)->OnEvent(event);
}

int TuringTableUI::cols(void) { return 0; }

int TuringTableUI::rows(void) { return rowsComponents.size(); }

void TuringTableUI::removeRow(void) {}
void TuringTableUI::removeCol(void) {}

void TuringTableUI::addRow(wchar_t c) {
    rowsComponents.push_back(ftxui::Make<TuringRowUI>());
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
        addRow(alph_[i]);
    }
    alph = alph_;
}

// TAPE UI
TuringTapeUI::TuringTapeUI(int size_) {
    size = size_;
    leftIndex = -(size_ / 2);
    tapeStrs.resize(size);
    for (int i = 0; i < size; i++) {
        tapeInputs.push_back(ftxui::Input(tapeStrs[i], &TURING_EMPTY_STR));
    }
    Add(ftxui::Container::Horizontal(tapeInputs));
}

bool TuringTapeUI::isValidPos(int pos) {
    return pos >= leftIndex && pos < leftIndex + size;
}

int TuringTapeUI::toLocalPos(int pos) { return pos - leftIndex; }

void TuringTapeUI::setChar(wchar_t c, int pos) {
    if (!isValidPos(pos)) return;
    pos = toLocalPos(pos);
    tapeStrs[pos] = c;
}

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

bool TuringTapeUI::OnEvent(ftxui::Event event) {
    return ChildAt(0)->OnEvent(event);
}

// MAIN UI
TuringUI::TuringUI(std::function<void()> quitFunc) {
    quit = quitFunc;

    moveTapeLeftButton = ftxui::Button("←Move", [&]() {});
    moveTapeRightButton = ftxui::Button("Move➔", [&]() {});

    stepButton = ftxui::Button("Step", [&]() {});
    runButton = ftxui::Button("Run", [&]() {});

    ftxui::InputOption alphInputOption;
    alphInputOption.on_change = [this](void) {
        this->needToUpdateTable = true;
    };
    alphInput = ftxui::Input(&alphStr, L"", alphInputOption);
    commentInput = ftxui::Input(&commentStr, L"");

    addButton = ftxui::Button("Add", [&]() { this->tableComponent->addCol(); });
    removeButton =
        ftxui::Button("Remove", [&]() { this->tableComponent->removeCol(); });

    tapeComponent = ftxui::Make<TuringTapeUI>(25);
    tableComponent = ftxui::Make<TuringTableUI>();

    Add(ftxui::Container::Vertical(
        {ftxui::Container::Horizontal(
             {moveTapeLeftButton, tapeComponent, moveTapeRightButton}),
         ftxui::Container::Horizontal(
             {stepButton, runButton, alphInput, commentInput}),
         ftxui::Container::Horizontal({addButton, removeButton}),
         tableComponent}));
}

ftxui::Element TuringUI::Render() {
    auto tapeAndButtons = ftxui::hbox(
        {moveTapeLeftButton->Render() | ftxui::color(ftxui::Color::DarkOrange),
         tapeComponent->Render(),
         moveTapeRightButton->Render() |
             ftxui::color(ftxui::Color::DarkOrange)});

    auto mainControl = ftxui::hbox(
        {stepButton->Render() | ftxui::color(ftxui::Color::SkyBlue1),
         runButton->Render() | ftxui::color(ftxui::Color::SkyBlue1),
         ftxui::vbox({ftxui::text(L"Алфавит"), ftxui::separatorLight(),
                      alphInput->Render()}) |
             ftxui::borderLight,
         ftxui::vbox({ftxui::text(L"Комментарий"), ftxui::separatorLight(),
                      commentInput->Render()}) |
             ftxui::borderLight});

    auto buttonsTable =
        ftxui::hbox({addButton->Render() | ftxui::color(ftxui::Color::Green),
                     removeButton->Render() | ftxui::color(ftxui::Color::Red)});

    auto mainBox =
        ftxui::vbox({ftxui::text(L"Машина Тьюринга") | ftxui::bold,
                     ftxui::separatorHeavy(), tapeAndButtons, mainControl,
                     buttonsTable, tableComponent->Render()}) |
        ftxui::border;

    return mainBox;
}

bool TuringUI::OnEvent(ftxui::Event event) {
    if (event == ftxui::Event::Escape) {
        quit();
        return true;
    }

    bool answer = ChildAt(0)->OnEvent(event);

    if (needToUpdateTable) {
        needToUpdateTable = false;
        this->tableComponent->updateTable(alphStr);
    }

    return answer;
}