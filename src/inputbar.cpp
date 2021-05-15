#include "./inputbar.hpp"

LOT::Temple::IDE::InputBar::InputBar(int p_Width, int p_Height, int p_PosX, int p_PosY, std::string p_Title):
WinX(p_Width),
WinY(p_Height),
title(p_Title) {
    winbuf = newwin(WinY, WinX, p_PosY, p_PosX);
    keypad(winbuf, true);
    wbkgd(winbuf, COLOR_PAIR(COLOR_INPUTBAR));
    wattron(winbuf, COLOR_PAIR(COLOR_INPUTBAR));
    box(winbuf, 0, 0);
    mvwprintw(winbuf, 0, 1, title.c_str());

    wmove(winbuf, 1, 1);
};

void LOT::Temple::IDE::InputBar::Draw() {
    wmove(winbuf, 1, 1);
    mvwprintw(winbuf, 1, 1, std::string(WinX - 2, ' ').c_str());
    mvwprintw(winbuf, 1, 1, contents.c_str());

    wrefresh(winbuf);
};


void LOT::Temple::IDE::InputBar::Retitle(std::string p_NewTitle) {
    title = p_NewTitle;
    
    box(winbuf, 0, 0);
    mvwprintw(winbuf, 0, 1, title.c_str());
};

void LOT::Temple::IDE::InputBar::Input() {
    int key = wgetch(winbuf);

    switch(key) {
        case 10: {
            InFocus = false;

            break;
        };

        case KEY_RIGHT: case KEY_LEFT: case KEY_UP: case KEY_DOWN: {
            break;
        };

        case KEY_BACKSPACE: {
            if ((int)contents.length() > 0) contents.erase((int)contents.length() - 1, 1);

            break;
        };

        default: {
            if (!((int)contents.length() >= WinX - 2)) contents += char(key);
        };
    };
};

std::string LOT::Temple::IDE::InputBar::GetContents() {
    return contents;
};