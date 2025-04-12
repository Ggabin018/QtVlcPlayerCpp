#include <QApplication>
#include "vlcplayer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    VLCPlayer player;
    player.show();
    return app.exec();
}
