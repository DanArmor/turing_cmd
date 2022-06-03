#include "turingUI.hpp"

// TABLE UI
TuringTableUI::TuringTableUI() {}

ftxui::Element TuringTableUI::Render(){
    return ftxui::hbox({});
}

bool TuringTableUI::OnEvent(ftxui::Event event){
    return false;
}

// TAPE UI
TuringTapeUI::TuringTapeUI(int size_) {
    size = size_;
    leftIndex = -(size_/2);
    tapeStrs.resize(size);
    for(int i = 0; i < size; i++){
        tapeInputs.push_back(ftxui::Input(&tapeStrs[i], &TURING_EMPTY_STR));
    }

    Add(ftxui::Container::Horizontal(tapeInputs));
}

bool TuringTapeUI::isValidPos(int pos){
    return pos >= leftIndex && pos < leftIndex+size;
}

int TuringTapeUI::toLocalPos(int pos){
    return pos - leftIndex;
}

void TuringTapeUI::setChar(wchar_t c, int pos){
    if(!isValidPos(pos))
        return;
    pos = toLocalPos(pos);
    tapeStrs[pos] = c;
}

void TuringTapeUI::setPositionAbsolute(int pos){
    positionAbsolute = pos;
}

ftxui::Element TuringTapeUI::Render(){
    std::vector<ftxui::Element> cells;
    auto tapeCursor = [this, &cells](int x, auto elem){
        if(x+this->leftIndex == this->positionAbsolute){
            return ftxui::vbox({
                elem,
                ftxui::text(L" ▲ ") | ftxui::color(ftxui::Color::DarkOrange3),
                ftxui::text(L" | ") | ftxui::color(ftxui::Color::DarkOrange3)
            });
        }else{
            return elem;
        }
    };
    for(int i = 0; i < size; i++){
        cells.push_back(
            ftxui::vbox({
                ftxui::text(std::to_wstring(leftIndex + i)) | ftxui::color(ftxui::Color::DarkOrange),
                ftxui::separatorHeavy(),
                tapeCursor(i,tapeInputs[i]->Render())
            }) | ftxui::borderLight | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, CELL_SIZE)
        );
    }
    return ftxui::hbox({cells});
}

bool TuringTapeUI::OnEvent(ftxui::Event event){
    return ChildAt(0)->OnEvent(event);
}

// MAIN UI
TuringUI::TuringUI(std::function<void()> quitFunc) {
    quit = quitFunc;

    moveTapeLeftButton = ftxui::Button("←Move", [&](){});
    moveTapeRightButton = ftxui::Button("Move➔", [&](){});

    stepButton = ftxui::Button("Step", [&]() {});
    runButton = ftxui::Button("Run", [&]() {});
    alphInput = ftxui::Input(&alphStr, "");
    commentInput = ftxui::Input(&commentStr, "");

    addButton = ftxui::Button("Add", [&]() {});
    removeButton = ftxui::Button("Remove", [&]() {});

    tapeComponent = ftxui::Make<TuringTapeUI>(25);
    tableComponent = ftxui::Make<TuringTableUI>();

    Add(
        ftxui::Container::Vertical(
            {
                ftxui::Container::Horizontal({moveTapeLeftButton, tapeComponent, moveTapeRightButton}),
                ftxui::Container::Horizontal({stepButton, runButton, alphInput, commentInput}),
                ftxui::Container::Horizontal({addButton, removeButton}),
                tableComponent
            }
        )
    );
}

ftxui::Element TuringUI::Render(){
    
    auto tapeAndButtons = ftxui::hbox({
        moveTapeLeftButton->Render() | ftxui::color(ftxui::Color::DarkOrange),
        tapeComponent->Render(),
        moveTapeRightButton->Render() | ftxui::color(ftxui::Color::DarkOrange)
    });

    auto mainControl = ftxui::hbox({
        stepButton->Render() | ftxui::color(ftxui::Color::SkyBlue1),
        runButton->Render() | ftxui::color(ftxui::Color::SkyBlue1),
        ftxui::vbox({ftxui::text(L"Алфавит"), ftxui::separatorLight(), alphInput->Render()}) | ftxui::borderLight,
        ftxui::vbox({ftxui::text(L"Комментарий"), ftxui::separatorLight(), commentInput->Render()}) | ftxui::borderLight
    });

    auto buttonsTable = ftxui::hbox({
        addButton->Render() | ftxui::color(ftxui::Color::Green),
        removeButton->Render() | ftxui::color(ftxui::Color::Red)
    });

    auto mainBox = ftxui::vbox({
        ftxui::text(L"Машина Тьюринга") | ftxui::bold,
        ftxui::separatorHeavy(),
        tapeAndButtons,
        mainControl,
        buttonsTable,
        tableComponent->Render()
    }) | ftxui::border;

    return mainBox;
}

bool TuringUI::OnEvent(ftxui::Event event){
    if(event == ftxui::Event::Escape){
        quit();
        return true;
    }

    return ChildAt(0)->OnEvent(event);
}