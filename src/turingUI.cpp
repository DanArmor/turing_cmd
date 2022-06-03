#include "turingUI.hpp"

// TABLE UI
TuringTableUI::TuringTableUI() {}

ftxui::Element TuringTableUI::Render(){
    return ftxui::hbox({});
}

bool TuringTableUI::OnEvent(ftxui::Event){
    return false;
}

// TAPE UI
TuringTapeUI::TuringTapeUI(int size_) {
    size = size_;
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

ftxui::Element TuringTapeUI::Render(){
    return ftxui::hbox({});
}

bool TuringTapeUI::OnEvent(ftxui::Event){
    return false;
}

// MAIN UI
TuringUI::TuringUI(std::function<void()> quitFunc) {
    quit = quitFunc;

    stepButton = ftxui::Button("Step", [&]() {});
    runButton = ftxui::Button("Run", [&]() {});
    alphInput = ftxui::Input(&alphStr, "");
    commentInput = ftxui::Input(&commentStr, "");

    addButton = ftxui::Button("Add", [&]() {});
    removeButton = ftxui::Button("Remove", [&]() {});

    tapeComponent = ftxui::Make<TuringTapeUI>(20);
    tableComponent = ftxui::Make<TuringTableUI>();

    Add(
        ftxui::Container::Vertical(
            {
                tapeComponent,
                ftxui::Container::Horizontal({stepButton, runButton, alphInput, commentInput}),
                ftxui::Container::Horizontal({addButton, removeButton}),
                tableComponent
            }
        )
    );
}

ftxui::Element TuringUI::Render(){
    auto buttonsTape = ftxui::hbox({
        stepButton->Render(),
        runButton->Render(),
        alphInput->Render(),
        commentInput->Render()
    });

    auto buttonsTable = ftxui::hbox({
        addButton->Render(),
        removeButton->Render()
    });

    auto mainBox = ftxui::vbox({
        ftxui::text(L"Машина Тьюринга"),
        ftxui::separatorHeavy(),
        tableComponent->Render(),
        buttonsTape,
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