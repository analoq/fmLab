#include <iostream>
#include <boost/math/common_factor_rt.hpp>
#include "audio.hpp"
#include "fm.hpp"
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
using namespace boost;

MainWindow::MainWindow(FMSynth &fm, QWidget *parent) :
    QMainWindow(parent),
    fmsynth {fm},
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    vizwindow.show();

    audioEngine.init();
}

MainWindow::~MainWindow()
{
    audioEngine.close();
    delete ui;
}

void MainWindow::on_button_trigger_clicked()
{
    // set synth params
    fmsynth.carrier_frequency = ui->carrier_textbox->text().toFloat();
    fmsynth.modulating_frequency = ui->modulating_textbox->text().toFloat();
    fmsynth.modulation_index.set(ui->modulation_index_textbox->text().toFloat());

    // find ratio
    int carrier_frequency = ui->carrier_textbox->text().toInt();
    int modulating_frequency = ui->modulating_textbox->text().toInt();
    int gcd = math::gcd(carrier_frequency, modulating_frequency);
    int numerator = carrier_frequency / gcd;
    int denominator = modulating_frequency / gcd;
    ui->ratio->setText(QString("%1 / %2").arg(numerator).arg(denominator));

    // find peak deviation
    ui->peak_deviation->setText(QString("%1 Hz").arg(fmsynth.peak_deviation()));
}

void MainWindow::on_volume_sliderMoved(int position)
{
    const float a = pow(10, position / 20.0);
    audioEngine.volume.set(a);
    QString str {QString("%1 dB").arg(position)};
    ui->label_volume_db->setText(str);
}

void MainWindow::on_carrier_slider_sliderMoved(int position)
{
    const float frequency {440.0f * powf(2, position/100.0f)};
    ui->carrier_textbox->setText(QString("%1").arg(frequency));
    audioEngine.fmsynth.carrier_frequency = frequency;
}

void MainWindow::on_modulating_slider_sliderMoved(int position)
{
    const float frequency {static_cast<float>(position)};
    ui->modulating_textbox->setText(QString("%1").arg(frequency));
    audioEngine.fmsynth.modulating_frequency = frequency;
}

void MainWindow::on_modulation_index_slider_sliderMoved(int position)
{
    const float index {static_cast<float>(position) / 10.0f};
    ui->modulation_index_textbox->setText(QString("%1").arg(index));
    audioEngine.fmsynth.modulation_index.set(index);
}

void MainWindow::on_carrier_textbox_editingFinished()
{
    fmsynth.carrier_frequency = ui->carrier_textbox->text().toFloat();
}

void MainWindow::on_modulating_textbox_editingFinished()
{
    fmsynth.modulating_frequency = ui->modulating_textbox->text().toFloat();
}

void MainWindow::on_modulation_index_textbox_editingFinished()
{
    fmsynth.modulation_index.set(ui->modulation_index_textbox->text().toFloat());
}

void MainWindow::on_scale_sliderMoved(int position)
{
    vizwindow.scale = position/1000.0 * (2.0/7040.0 - 1.0/100.0) + 1.0/100.0;
}

