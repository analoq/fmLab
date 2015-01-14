#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vizwindow.h"
#include "audio.hpp"
#include "fm.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(FMSynth &fm, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_button_trigger_clicked();

    void on_volume_sliderMoved(int position);

    void on_carrier_slider_sliderMoved(int position);

    void on_modulating_slider_sliderMoved(int position);

    void on_modulation_index_slider_sliderMoved(int position);

    void on_carrier_textbox_editingFinished();

    void on_modulating_textbox_editingFinished();

    void on_modulation_index_textbox_editingFinished();

    void on_scale_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
    FMSynth &fmsynth;
    VizWindow vizwindow {fmsynth};
    AudioEngine audioEngine {fmsynth};
};

#endif // MAINWINDOW_H
