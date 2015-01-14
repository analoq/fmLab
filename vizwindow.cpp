#include "vizwindow.h"
#include "ui_vizwindow.h"

VizWindow::VizWindow(FMSynth &fm, QWidget *parent) :
    QMainWindow(parent),
    fmsynth(fm),
    ui(new Ui::VizWindow)
{
    ui->setupUi(this);
}

VizWindow::~VizWindow()
{
    delete ui;
}
