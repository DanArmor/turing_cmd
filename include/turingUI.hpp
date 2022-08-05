#ifndef _INC_TURING_UI_H
#define _INC_TURING_UI_H

#include <string>
#include <atomic>
#include <thread>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"

#include "nlohmann/json.hpp"

using namespace nlohmann;

#include "turing.hpp"
#include "translate.hpp"

class TuringCellUI : public ftxui::ComponentBase {
   private:
    int number;
    std::string str;
    ftxui::Component input;
    bool isTop = false;

   public:
    TuringCellUI(int number_);
    TuringCellUI(int number_, bool isTop_);

    void loadSaved(std::string str_);

    TuringTurn getTurn();
    bool containStr();

    ftxui::Element Render() override;
    bool OnEvent(ftxui::Event) override;
};

class TuringRowUI : public ftxui::ComponentBase {
   private:
    std::vector<std::shared_ptr<TuringCellUI>> cells;
    bool isTop = false;

   public:
    TuringRowUI();
    TuringRowUI(bool isTop_);

    void addCol();
    void removeCol();

    void loadSavedCell(int i, std::string str);

    TuringTurn getTurn(int i);
    bool containStr(int i);
    int size(void);

    ftxui::Element Render() override;
    bool OnEvent(ftxui::Event) override;
};

class TuringTableUI : public ftxui::ComponentBase {
   private:
    std::string alph;
    std::vector<std::shared_ptr<TuringRowUI>> rowsComponents;

    int cols(void);
    int rows(void);

   public:
    TuringTableUI();

    void removeRow(void);
    void removeCol(void);

    void loadSavedCell(int row, int col, std::string str);

    void addRow(char c, bool isTop_);
    void addCol(void);

    std::vector<TuringTurn> getTurns(void);

    void updateTable(std::string alph_);

    ftxui::Element Render() override;
    bool OnEvent(ftxui::Event) override;
};

class TuringTapeUI : public ftxui::ComponentBase {
   private:
    int size;
    int leftIndex;
    int CELL_SIZE = 6;
    int positionAbsolute = 0;
    std::vector<std::string> tapeStrs;
    std::vector<ftxui::Component> tapeInputs;

    int toLocalPos(int pos);
    bool isValidPos(int pos);

   public:
    TuringTapeUI(int size_);

    int getSize(void);
    int getLeftIndex(void);
    std::vector<std::string> &getStrs(void);

    void setChar(char c, int pos);
    void setLeftIndex(int pos);
    void setPositionAbsolute(int pos);

    ftxui::Element Render() override;
    bool OnEvent(ftxui::Event) override;
};

struct TuringUIStatus{
    enum Status{
        START,
        STEP,
        RUNNING_ON,
        RUNNING_OFF,
        STOP
    };
    Status status = START;
    ftxui::Element Render(){
        switch (status) {
            case START:
                return ftxui::text("Start state") | ftxui::color(ftxui::Color::BlueViolet);
            case STEP:
                return ftxui::text("Step") | ftxui::color(ftxui::Color::Green1) | ftxui::blink;
            case RUNNING_ON:
                return ftxui::text("Running") | ftxui::color(ftxui::Color::Green3);
            case RUNNING_OFF:
                return ftxui::text("Running") | ftxui::color(ftxui::Color::DarkGreen);
            case STOP:
                return ftxui::text("Stopped. Need to reset") | ftxui::color(ftxui::Color::Red1);
            default:
                throw std::invalid_argument("Неизвестный статус");
        }
    }
};

struct TuringSave{
    std::string fileName;
    std::string comm;
    std::string alph;
    std::map<std::pair<int, char>, TuringTurn> table;
    std::map<int, char> tape;
};

class TuringUI : public ftxui::ComponentBase {
   private:
    void updateComponents(void);
    void makeTurn(void);
    void updateStateTape(void);
    void saveToFile(void);
    std::vector<std::string> breakByLines(std::string str);
    ftxui::Element breakByLinesElement(std::string str);
    std::function<void()> quit;
    TuringState state;
    TuringMachine machine;

    std::atomic<bool> isResetState = true;
    std::atomic<bool> isRunning = false;
    std::atomic<bool> isShowingHelp = false;
    std::atomic<bool> isErrorFile = false;
    LANG language = LANG::L_EN;

    std::atomic<bool> wasTriedToSave = false;
    enum SaveStatus{
        INVALID,
        OK
    } lastSaveStatus;

    bool needToUpdateTable = false;
    std::string alphStr;
    std::string commentStr, fileStr;
    TuringUIStatus status;

    ftxui::Component helpButton;
    ftxui::Component localeButton;

    ftxui::Component moveTapeLeftButton;
    ftxui::Component moveTapeRightButton;

    ftxui::Component stepButton;
    ftxui::Component runButton;
    ftxui::Component resetButton;
    ftxui::Component alphInput;
    ftxui::Component commentInput;
    ftxui::Component fileInput;

    ftxui::Component addButton;
    ftxui::Component removeButton;

    ftxui::Component toMainDisplay;
    ftxui::Component toHelpDisplay;

    std::shared_ptr<TuringTapeUI> tapeComponent;
    std::shared_ptr<TuringTableUI> tableComponent;
    ftxui::ScreenInteractive *scr;

   public:
    TuringUI(std::function<void()> quitFunc, ftxui::ScreenInteractive *scr_);
    void refresh(void);

    void loadSave(TuringSave);
    std::string &getText(std::string const &key);

    ftxui::Element Render() override;
    bool OnEvent(ftxui::Event) override;
};

#endif
