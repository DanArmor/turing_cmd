#ifndef _INC_TURING_UI_H
#define _INC_TURING_UI_H

#include <string>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/table.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"
#include "ftxui/util/ref.hpp"
#include "turing.hpp"

class TuringRowUI : public ftxui::ComponentBase {
   private:
    std::vector<std::wstring> strs;
    std::vector<ftxui::Component> inputs;
    bool isTop = false;

   public:
    TuringRowUI();
    TuringRowUI(bool isTop_);

    void addCol();
    void removeCol();

    ftxui::Element Render() override;
    bool OnEvent(ftxui::Event) override;
};

class TuringTableUI : public ftxui::ComponentBase {
   private:
    std::wstring alph;
    std::vector<std::shared_ptr<TuringRowUI>> rowsComponents;

    int cols(void);
    int rows(void);

   public:
    TuringTableUI();

    void removeRow(void);
    void removeCol(void);

    void addRow(wchar_t c, bool isTop_);
    void addCol(void);

    void updateTable(std::wstring alph_);

    ftxui::Element Render() override;
    bool OnEvent(ftxui::Event) override;
};

class TuringTapeUI : public ftxui::ComponentBase {
   private:
    int size;
    int leftIndex;
    int CELL_SIZE = 5;
    int positionAbsolute = 0;
    std::vector<std::wstring> tapeStrs;
    std::vector<ftxui::Component> tapeInputs;

    int toLocalPos(int pos);
    bool isValidPos(int pos);

   public:
    TuringTapeUI(int size_);

    void setChar(wchar_t c, int pos);
    void setPositionAbsolute(int pos);

    ftxui::Element Render() override;
    bool OnEvent(ftxui::Event) override;
};

class TuringUI : public ftxui::ComponentBase {
   private:
    std::function<void()> quit;
    TuringState state;
    TuringMachine machine;

    bool isRunning = false;
    bool needToUpdateTable = false;
    std::wstring alphStr, commentStr;

    ftxui::Component helpButton;

    ftxui::Component moveTapeLeftButton;
    ftxui::Component moveTapeRightButton;

    ftxui::Component stepButton;
    ftxui::Component runButton;
    ftxui::Component resetButton;
    ftxui::Component alphInput;
    ftxui::Component commentInput;

    ftxui::Component addButton;
    ftxui::Component removeButton;

    std::shared_ptr<TuringTapeUI> tapeComponent;
    std::shared_ptr<TuringTableUI> tableComponent;

   public:
    TuringUI(std::function<void()> quitFunc);

    ftxui::Element Render() override;
    bool OnEvent(ftxui::Event) override;
};

#endif
