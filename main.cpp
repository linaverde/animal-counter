#include "startwindow.h"
#include "databasehelper.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StartWindow w;
    w.show();

    DataBase database;
    database.connectToDataBase();

    return a.exec();
}
