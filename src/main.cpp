#include "./INCLUDE"

#include "./app.hpp"

int main (int argc, char* argv[]) {
    App app;
    app.Initialization();

    app.Start();

    app.Finalization();

    return 0;
};