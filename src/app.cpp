#include "./app.hpp"

App::App() {};

void App::Initialization() {
    initscr();

    cbreak();
    noecho();
    keypad(stdscr, true);

    getmaxyx(stdscr, WinY, WinX);

    start_color();
    use_default_colors();
    if (can_change_color() && COLORS >= 16) {
        init_color(COLOR_BRIGHT_WHITE, 1000, 1000, 1000);
        init_color(COLOR_BRIGHT_YELLOW, 1000, 1000, 0);
        init_color(COLOR_BRIGHT_CYAN, 0, 1000, 1000);
        init_color(COLOR_BRIGHT_BLUE, 600, 600, 1000);
        init_color(COLOR_BRIGHT_RED, 1000, 0, 0);

        init_color(COLOR_BLUE, 200, 200, 800);
        init_color(COLOR_WHITE, 800, 800, 800);
        init_color(COLOR_YELLOW, 800, 800, 0);
        init_color(COLOR_MAGENTA, 800, 0, 800);
        init_color(COLOR_BLACK, 100, 100, 100);
        init_color(COLOR_GREEN, 100, 600, 100);
    };

    init_pair(COLOR_EDITOR, COLOR_BRIGHT_WHITE, COLOR_BLACK);
    init_pair(COLOR_MENU, COLOR_BRIGHT_YELLOW, COLOR_BLUE);
    init_pair(COLOR_MENU_HIGHLIGHT, COLOR_BLUE, COLOR_BRIGHT_YELLOW);
    init_pair(COLOR_BACK, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_INPUTBAR, COLOR_BLACK, COLOR_WHITE);

    init_pair(COLOR_KEYWORD, COLOR_BRIGHT_YELLOW, COLOR_BLACK);
    init_pair(COLOR_NUMBER, COLOR_BRIGHT_BLUE, COLOR_BLACK);
    init_pair(COLOR_STRING, COLOR_BRIGHT_CYAN, COLOR_BLACK);
    init_pair(COLOR_COMMENT, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_INDETIF, COLOR_BRIGHT_RED, COLOR_BLACK);

    attron(COLOR_PAIR(COLOR_BACK));
    bkgd(COLOR_PAIR(COLOR_BACK));
    ResetMainScr();

    Menu = new LOT::Temple::IDE::Menu(WinX, 1, 0, 0);
    Menu->SetChoices({
        "Exit", "Close", "New", "Open", "Save As"
    }); 

    #define EXIT 0
    #define CLOSE 1
    #define NEW 2
    #define OPEN 3
    #define SAVE 4

    Input = new LOT::Temple::IDE::InputBar(30, 3, WinX / 2 - 15, WinY / 2 - 1.5, "Input");

    editors.push_back(new LOT::Temple::IDE::Editor(WinX, WinY - 1, 0, 1, DEFAULT_TITLE));
    EditorPointer = -1;
};

void App::Finalization() {
    endwin();

    for (int i = 0; i < (int)editors.size(); ++ i) delete editors[i];
};

void App::Start() {
    running = true;

    refresh();
    Menu->Draw();

    //editors[EditorPointer]->DrawFrame();
    for (int i = 0; i < (int)editors.size(); ++ i) editors[i]->InFocus = false;
 
    Menu->InFocus = true;
    Input->InFocus = false;

    uint8_t InputMode = OPEN;

    while (running) {
        if (Menu->InFocus) {
            Menu->Draw();
            Menu->Input();

            if (!Menu->InFocus) {
                if (EditorPointer == -1) Menu->InFocus = true;
                else {
                    editors[EditorPointer]->InFocus = true;
                    Menu->Draw();
                };
            };

            switch(Menu->GetChoice()) {
                case EXIT: {
                    running = false;

                    break;
                };

                case NEW: {
                    EditorPointer = MAIN_EDITOR;
                    editors[EditorPointer]->Clear();
                    editors[EditorPointer]->Retitle(DEFAULT_TITLE);
                    editors[EditorPointer]->DrawFrame();
                    editors[EditorPointer]->Draw();

                    break;
                };

                case CLOSE: {
                    if (EditorPointer == -1) break;
                    editors[EditorPointer]->InFocus = false;
                    ResetMainScr();
                    EditorPointer = -1;

                    break;
                };

                case OPEN: case SAVE: {
                    Input->InFocus = true;
                    Input->Retitle("Enter a file name");
                    InputMode = Menu->GetChoice();

                    Menu->InFocus = false;

                    break;
                };
            };
        } else if (Input->InFocus) {
            Input->Draw();
            Input->Input();

            if (!Input->InFocus) {
                if (InputMode == OPEN) {
                    if (Input->GetContents() != "" && Input->GetContents() != " ") {
                        Menu->InFocus = true;
                        if (LOT::Temple::Utils::FileExists(Input->GetContents())) {
                            EditorPointer = MAIN_EDITOR;
                            editors[EditorPointer]->Retitle(Input->GetContents());
                            editors[EditorPointer]->SetContents(LOT::Temple::Utils::FileToString(Input->GetContents()));

                            editors[EditorPointer]->DrawFrame();
                            editors[EditorPointer]->Draw();
                        } else {
                            Menu->InFocus = true;

                            if (EditorPointer == -1) ResetMainScr();
                            else editors[EditorPointer]->Draw();
                        };
                    } else {
                        Menu->InFocus = true;

                        if (EditorPointer == -1) ResetMainScr();
                        else editors[EditorPointer]->Draw();
                    };
                } else {
                    if (Input->GetContents() != "" && EditorPointer != -1) {
                        editors[EditorPointer]->Retitle(Input->GetContents());
                        LOT::Temple::Utils::StringToFile(editors[EditorPointer]->GetContents(), Input->GetContents());

                        editors[EditorPointer]->DrawFrame();
                    } else {
                        Menu->InFocus = true;

                        if (EditorPointer == -1) ResetMainScr();
                        else editors[EditorPointer]->Draw();
                    };
                };
            };
        } else {
            editors[EditorPointer]->Draw();
            editors[EditorPointer]->Input();

            if (!editors[EditorPointer]->InFocus) Menu->InFocus = true;
        };
    };
};

void App::ResetMainScr() {
    clear();

    mvprintw(1, 0, " _..__ ________  _________ _      _____\n|_|| _|  ___|  \\/\\_|| ___ \\ |    |  __/\n  ||| | |__ | . \\._/| |,/ /\\\\    | |__  \n  | |/|  _||| |\\/|_||  \\|/| \\|   |  _|| \n .| ||| |_``| | / _|| ||| | ||.._| |_|| \n \\\\_/\\\\____/\\_| ||_/\\_|   \\___||/\\____/ \n ||   |/        ||            //     ||\n");
    printw(((std::string) "\n* Temple IDE\n* by LordOfTrident\n* current version " + VER + " (" + DATE + ") pre-release\n\n\nArrows - Navigate the cursor/Menu options\nCTRL + U - Update the current file\nCTRL + C - Fast exit\nESC - Switch between windows").c_str());

    refresh();
};