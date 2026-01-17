#include "Game.h"
#include "Config.h"
#include <QApplication>
#include <ctime>

int main(int argc, char* argv[]) {
    srand(static_cast<unsigned>(time(0)));
    QApplication application(argc, argv);
    Game gameWindow;
    gameWindow.show();
    return application.exec();
}
