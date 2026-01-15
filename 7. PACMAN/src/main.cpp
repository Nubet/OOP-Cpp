#include "Game.h"
#include "Config.h"
#include <QApplication>
#include <ctime>

int main(int argc, char* argv[]) {
    qputenv("QT_SCALE_FACTOR", QByteArray::number(config::ViewScale));
    srand(static_cast<unsigned>(time(0)));
    QApplication application(argc, argv);
    Game gameWindow;
    gameWindow.show();
    return application.exec();
}
