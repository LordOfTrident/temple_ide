#pragma once

#include "./INCLUDE"

namespace LOT {
    namespace Temple {
        namespace IDE {
            class InputBar {
            public: InputBar(int p_Width, int p_Height, int p_PosX, int p_PosY, std::string p_Title);
                void Input();
                void Draw();
                void Retitle(std::string p_NewTitle);
                std::string GetContents();

                bool InFocus;
            
            private:
                WINDOW *winbuf;
                int WinX, WinY;
                std::string title;
                std::string contents;
            };
        }
    };
};