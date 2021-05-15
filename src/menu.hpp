#pragma once

#include "./INCLUDE"

namespace LOT {
    namespace Temple {
        namespace IDE {
            class Menu {
            public: Menu(int p_Width, int p_Height, int p_PosX, int p_PosY);
                WINDOW *GetWinbuf();

                void SetChoices(std::vector <std::string> p_Choices);
                void Draw();
                void Input();
                int8_t GetChoice();

                bool InFocus;

            private:
                WINDOW *winbuf;
                int WinX, WinY, PosX, PosY;
                std::vector <std::string> Choices;
                uint8_t Highlight;
                int8_t CurrentChoice;
            };
        };
    };
};