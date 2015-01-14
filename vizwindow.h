#ifndef VIZWINDOW_H
#define VIZWINDOW_H

#include <QMainWindow>
#include "fm.hpp"

namespace Ui {
class VizWindow;
}

class VizWindow : public QMainWindow
{
    Q_OBJECT

public:
    FMSynth &fmsynth;
    explicit VizWindow(FMSynth &fm, QWidget *parent = 0);
    ~VizWindow();

    float scale {1000.0};

private:
    Ui::VizWindow *ui;
};

#endif // VIZWINDOW_H
