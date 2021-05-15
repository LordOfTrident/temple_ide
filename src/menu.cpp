#include "./menu.hpp"

LOT::Temple::IDE::Menu::Menu(int p_Width, int p_Height, int p_PosX, int p_PosY):
WinX(p_Width),
WinY(p_Height),
PosX(p_PosX),
PosY(p_PosY),
Highlight(0),
CurrentChoice(-1) {
    winbuf = newwin(WinY, WinX, PosY, PosX);
    keypad(winbuf, true);
    wbkgd(winbuf, COLOR_PAIR(COLOR_MENU));
    wattron(winbuf, COLOR_PAIR(COLOR_MENU));

    wmove(winbuf, 1, 1);
};

WINDOW *LOT::Temple::IDE::Menu::GetWinbuf() {
    return winbuf;
};

void LOT::Temple::IDE::Menu::SetChoices(std::vector <std::string> p_Choices) {
    Choices = p_Choices;
};

void LOT::Temple::IDE::Menu::Draw() {
    wmove(winbuf, 0, 0);
    for (int i = 0; i < (int)Choices.size(); ++ i) {
        wattron(winbuf, COLOR_PAIR(Highlight == i && InFocus? COLOR_MENU_HIGHLIGHT : COLOR_MENU));
        wprintw(winbuf, " %s ", Choices[i].c_str());
    };

    wmove(winbuf, 0, WinX - 7);
    wattron(winbuf, COLOR_PAIR(COLOR_MENU_HIGHLIGHT));
    wprintw(winbuf, "tm-ide");

    wrefresh(winbuf);
};

void LOT::Temple::IDE::Menu::Input() {
    int choice = wgetch(winbuf);
    CurrentChoice = -1;

    switch(choice) {
        case 27: {
            InFocus = false;

            break;
        };

        case KEY_LEFT: {
            if (Highlight == 0) Highlight = (int)Choices.size();
            -- Highlight;

            break;
        };

        case KEY_RIGHT: {
            if (Highlight == (int)Choices.size() - 1) Highlight = -1;
            ++ Highlight;

            break;
        };

        case 10: {
            CurrentChoice = Highlight;

            break;
        };
    };
};

int8_t LOT::Temple::IDE::Menu::GetChoice() {
    return CurrentChoice;
};