#include "./editor.hpp"

std::string GetLineAt(std::string p_Text, int p_Pos1D) {
    p_Text = "\n" + p_Text;
    ++ p_Pos1D;
    
    int i;
    for (i = p_Pos1D; i > 0; -- i) {
        if (p_Text[i] == '\n') break;
    };

    int LineStart = i;

    for (i = p_Pos1D; i < (int)p_Text.length(); ++ i) {
        if (p_Text[i] == '\n') break;
    };

    int LineEnd = i - LineStart;
    return p_Text.substr(LineStart, LineEnd);
};

bool CompareKeyword(std::string p_Text, int p_idx) {
    std::vector <std::string> Keywords = {
        "push", "pop", "halt",

        "add", "sub", "mult", "div", "mod",

        "out", "outch"
    };

    for (int i = 0; i < (int)Keywords.size(); ++ i) {
        /*if (p_Text.substr(p_idx, (int)Keywords[i].length()) == Keywords[i] 
            && (p_Text[p_idx - 1] == NULL 
                || p_Text[p_idx - 1] == ' ' 
                || p_Text[p_idx - 1] == '\t' 
                || p_Text[p_idx - 1] == '\n' 
                || p_Text[p_idx - 1] == '\0') 
            && (p_Text[p_idx + (int)Keywords[i].length()] == NULL 
                || p_Text[p_idx + (int)Keywords[i].length()] == ' ' 
                || p_Text[p_idx + (int)Keywords[i].length()] == '\t' 
                || p_Text[p_idx + (int)Keywords[i].length()] == '\n' 
                || p_Text[p_idx + (int)Keywords[i].length()] == '\0'
            )
        ) return true;*/

        if (p_Text.substr(p_idx, (int)Keywords[i].length()) == Keywords[i] 
            && (p_Text[p_idx - 1] == NULL 
                || ((int)std::string("\t \n \0").find(p_Text[p_idx - 1]) != -1)
            ) && (p_Text[p_idx + (int)Keywords[i].length()] == NULL
                || ((int)std::string("\t \n \0").find(p_Text[p_idx + (int)Keywords[i].length()]) != -1)
            )
        ) return true;
    };

    return false;
};

LOT::Temple::IDE::Editor::Editor(int p_Width, int p_Height, int p_PosX, int p_PosY, std::string p_Title):
WinX(p_Width),
WinY(p_Height),
PosX(p_PosX),
PosY(p_PosY),
CurPos(0),
CurY(0),
CurX(0),
RenderStart(0),
RenderEnd(1),
title(p_Title) {
    Frame = newwin(WinY, WinX, PosY, PosX);
    wbkgd(Frame, COLOR_PAIR(COLOR_EDITOR));
    wattron(Frame, COLOR_PAIR(COLOR_EDITOR));

    box(Frame, 0, 0);
    mvwprintw(Frame, 0, 1, title.c_str());
    wmove(Frame, 1, 1);

    Contents = newwin(WinY - 2, WinX - 2, PosY + 1, PosX + 1);
    keypad(Contents, true);
    wbkgd(Contents, COLOR_PAIR(COLOR_EDITOR));
    wattron(Contents, COLOR_PAIR(COLOR_EDITOR));

    Position = newwin(1, 8, WinY, 1);
    wbkgd(Position, COLOR_PAIR(COLOR_EDITOR));
    wattron(Position, COLOR_PAIR(COLOR_EDITOR));

    contents = "";
};

std::string LOT::Temple::IDE::Editor::GetTitle() {
    return title;
};

void LOT::Temple::IDE::Editor::Resize(int p_NewX, int p_NewY) {
    WinX = p_NewX;
    WinY = p_NewY;

    wresize(Contents, WinY, WinX);
};

void LOT::Temple::IDE::Editor::Reposition(int p_NewX, int p_NewY) {
    PosX = p_NewX;
    PosY = p_NewY;

    wmove(Contents, PosY, PosX);
};

void LOT::Temple::IDE::Editor::Draw() {
    wclear(Contents);

    wattron(Contents, COLOR_PAIR(COLOR_EDITOR));

    int line = 0, y = RenderStart > 1? -1 : 0;
    bool str = false, comment = false, end = false;
    RenderEnd = RenderStart + WinY - (RenderStart > 1? 3 : 3);
    for (int i = 0; i < (int)contents.length(); ++ i) {
        switch (contents[i]) {
            case '\n': {
                ++ line;

                if (line >= RenderEnd) end = true;

                break;
            };
        };

        if (end) break;

        if (line >= RenderStart - 1) switch (contents[i]) {
            case 9: {
                wprintw(Contents, "    ");

                break;
            };

            case '\n': {
                ++ y;
                wmove(Contents, y, 0);

                comment = false;
                wattron(Contents, COLOR_PAIR(COLOR_EDITOR));

                break;
            };

            case '\'': {
                if (!comment) {
                    str = !str;
                    
                    if (str) wattron(Contents, COLOR_PAIR(COLOR_STRING));
                    else {
                        wprintw(Contents, "%c", contents[i]);
                        wattron(Contents, COLOR_PAIR(COLOR_EDITOR));

                        break;
                    };
                };
                
                wprintw(Contents, "%c", '\'');

                break;
            };

            case ';': {
                if (!str) {
                    comment = true;

                    wattron(Contents, COLOR_PAIR(COLOR_COMMENT));
                };

                wprintw(Contents, "%c", ';');

                break;
            };

            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': {
                if (!str && !comment) {
                    wattron(Contents, COLOR_PAIR(COLOR_NUMBER));
                    wprintw(Contents, "%c", contents[i]);
                    wattron(Contents, COLOR_PAIR(COLOR_EDITOR));

                    break;
                };

                wprintw(Contents, "%c", contents[i]);

                break;
            };

            case '#': case '&': {
                if (!str && !comment) {
                    wattron(Contents, COLOR_PAIR(COLOR_INDETIF));
                    wprintw(Contents, "%c", contents[i]);
                    wattron(Contents, COLOR_PAIR(COLOR_EDITOR));

                    break;
                };

                wprintw(Contents, "%c", contents[i]);

                break;
            };

            default: {
                if (!str && !comment) {
                    if (contents[i] == ' ') wattron(Contents, COLOR_PAIR(COLOR_EDITOR));
                    if (CompareKeyword(contents, i)) wattron(Contents, COLOR_PAIR(COLOR_KEYWORD));
                };

                wprintw(Contents, "%c", contents[i]);

                break;
            };
        };
    };

    wclear(Position);
    wprintw(Position, "--------");
    mvwprintw(Position, 0, 0, "%i:%i", CurY, CurX);
    wrefresh(Position);

    //wmove(Contents, CurY, CurPos - (WinX - 2) * CurY);
    wmove(Contents, CurY - RenderStart + (RenderStart >= 1? 1 : 0), CurX); //wprintw(Contents, " | x%i y%i s%i rs%i re%i", CurX, CurY, SAVE, RenderStart, RenderEnd);
    wrefresh(Contents);
};

void LOT::Temple::IDE::Editor::DrawFrame() {
    wrefresh(Frame);
};

void LOT::Temple::IDE::Editor::Retitle(std::string p_NewTitle) {
    title = p_NewTitle;

    box(Frame, 0, 0);
    mvwprintw(Frame, 0, 1, title.c_str());
    wmove(Frame, 1, 1);
};

void LOT::Temple::IDE::Editor::Clear() {
    contents = "";
    CurPos = CurX = RenderStart = CurY = 0;
    RenderEnd = 1;
};

std::string LOT::Temple::IDE::Editor::GetContents() {
    return contents;
};

void LOT::Temple::IDE::Editor::SetContents(std::string p_Contents) {
    Clear();

    contents = p_Contents;
};

void LOT::Temple::IDE::Editor::Input() {
    int key = wgetch(Contents);

    switch (key) {
        case 27: {
            InFocus = false;

            break;
        };

        case CTRL('u'): {
            LOT::Temple::Utils::StringToFile(contents, title);
            
            break;
        };

        case KEY_UP: case KEY_LEFT: {
            if (CurPos < 1) break;

            if (CurX < 1) {
                -- CurY;
                int LineLength = (int)GetLineAt(contents, CurPos - 2).length();
                CurX = LineLength == 0? 1 : LineLength;
            };
            
            -- CurPos;
            -- CurX;

            if (CurY < RenderStart) -- RenderStart;

            break;
        };

        case KEY_DOWN: case KEY_RIGHT: {
            if (CurPos + 1 > (int)contents.length()) break;
            if (CurX + 1 > (int)GetLineAt(contents, CurPos).length()) {
                ++ CurPos;
                ++ CurY;
                CurX = 0;

                if (CurY - RenderStart + (RenderStart >= 1? 1 : 0) > WinY - 4) ++ RenderStart;

                break;
            };

            /*if (CurX > WinX - 4) {
                ++ CurPos;
                ++ CurY;
                CurX = 0;

                if (CurY > WinY - 3) ++ RenderStart;

                break;
            };*/

            ++ CurPos;
            ++ CurX;

            break;
        };

        case KEY_BACKSPACE: {
            if (CurPos < 1) break;

            if (CurX < 1) {
                -- CurY;
                int LineLength = (int)GetLineAt(contents, CurPos - 2).length();
                CurX = LineLength == 0? 1 : LineLength;
            };
            
            contents.erase(CurPos - 1, 1);
            -- CurPos; 
            -- CurX;

            if (CurY < RenderStart) -- RenderStart;

            break;
        };

        case 10: {
            contents.insert(CurPos, "\n");
            ++ CurPos;
            ++ CurY;
            CurX = 0;

            if (CurY - RenderStart + (RenderStart >= 1? 1 : 0) > WinY - 4) ++ RenderStart;

            break;
        };

        default: {
            if (CurX > WinX - 4) {
                contents.insert(CurPos, "\n");
                ++ CurPos;
                ++ CurY;
                CurX = 0;

                if (CurY - RenderStart + (RenderStart >= 1? 1 : 0) > WinY - 4) ++ RenderStart;

                break;
            };

            std::string intersection = "";
            intersection += char(key);

            contents.insert(CurPos, intersection);
            ++ CurPos;
            ++ CurX;

            break;
        };
    };

    /*if (CurY > WinY - 3) {
        -- CurY;
        contents.erase(CurPos - 1, 1);
        -- CurPos;
    };

    switch (key) {
        case 'a' & 0x1F: {
            if (CurPos > 1) {
                if (contents[CurPos - 1] == '\n') -- CurY;

                -- CurPos;
            };

            break;
        };

        case 'd' & 0x1F: {
            if (!(CurPos == (int)contents.length())) ++ CurPos;

            break;
        };

        case 8: case 127: {
            if (CurPos == 0) break;
            if (contents[CurPos - 1] == '\n') -- CurY;

            contents.erase(CurPos - 1, 1);
            -- CurPos;

            break;
        };

        default: {    
            if (key == '\n') ++ CurY;
            else if ((int)GetLineAt(contents, CurPos).length() > WinX - 2) {
                contents.insert(CurPos, "\n");
                ++ CurPos;
                ++ CurY;

                if (CurY > WinY - 3) {
                    -- CurY;
                    contents.erase(CurPos - 1, 1);
                    -- CurPos;

                    break;
                };
            };

            std::string intersection = "";
            intersection += char(key);

            contents.insert(CurPos, intersection);
            ++ CurPos;

            break;
        };
    };*/
};

WINDOW *LOT::Temple::IDE::Editor::GetWinbuf() {
    return Contents;
};