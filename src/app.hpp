#pragma once

#include "./INCLUDE"

#include "./editor.hpp"
#include "./menu.hpp"
#include "./inputbar.hpp"
#include "./utils.hpp"

#define MAIN_EDITOR 0
#define DEFAULT_TITLE "Untitled.tm"

class App {
public: App();
    void Initialization();
    void Finalization();
    void Start();
   
private:
    void ResetMainScr();

    uint8_t WinX, WinY;
    bool running;
    int8_t EditorPointer;
    std::vector <LOT::Temple::IDE::Editor*> editors;
    LOT::Temple::IDE::Menu *Menu;
    LOT::Temple::IDE::InputBar *Input;
};