#include "./utils.hpp"

std::string LOT::Temple::Utils::FileToString(std::string p_FileName) {
    std::string f_string = "";
    std::ifstream f_Handle(p_FileName.c_str());

    if (f_Handle.is_open()) {
        std::string f_line = "";

        while (getline(f_Handle, f_line)) {
            f_string += f_line + '\n';
        };
    };

    f_Handle.close();

    return f_string;
};

void LOT::Temple::Utils::StringToFile(std::string p_Text, std::string p_FileName) {
    std::ofstream f_Handle(p_FileName.c_str());

    f_Handle << p_Text;
};

bool LOT::Temple::Utils::FileExists(std::string p_Name) {
    return (access(p_Name.c_str(), F_OK) != -1);
};