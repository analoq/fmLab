#include <QApplication>
#include "mainwindow.h"
#include "fm.hpp"

int main(int argc, char *argv[])
{
    FMSynth fmsynth;

    QApplication a(argc, argv);
    MainWindow w(fmsynth);
    w.show();

    return a.exec();
}
