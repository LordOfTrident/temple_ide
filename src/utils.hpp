#pragma once

#include "./INCLUDE"

namespace LOT {
    namespace Temple {
        namespace Utils {
            bool FileExists(std::string p_Name);

            std::string FileToString(std::string p_FileName);
            void StringToFile(std::string p_Text, std::string p_FileName);
        };
    };
};