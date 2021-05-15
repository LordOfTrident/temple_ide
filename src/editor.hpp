#pragma once

#include "./INCLUDE"

#include "./utils.hpp"

#define CTRL(c) ((c) & 037)

namespace LOT {
    namespace Temple {
        namespace IDE {
            class Editor {
            public: Editor(int p_Width, int p_Height, int p_PosX, int p_PosY, std::string p_Title);
                WINDOW *GetWinbuf();

                void Resize(int p_NewX, int p_NewY);
                void Reposition(int p_NewX, int p_NewY);
                void Retitle(std::string p_NewTitle);
                std::string GetTitle();
                void Draw();
                void DrawFrame();
                void Input();
                void Clear();
                std::string GetContents();
                void SetContents(std::string p_Contents);

                bool InFocus;

            private:
                WINDOW *Frame, *Contents, *Position;
                int WinX, WinY, PosX, PosY, 
                    CurPos, CurY, CurX, 
                    RenderStart, RenderEnd;
                std::string contents, title;
                //int SAVE;
            };
        };
    };
};