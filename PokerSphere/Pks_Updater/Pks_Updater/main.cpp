#include "PksUpdater.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PksUpdater w(argv[1]);
    w.show();
    return a.exec();
}
