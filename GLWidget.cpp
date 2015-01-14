#include <QTimer>
#include "vizwindow.h"
#include "GLWidget.h"
#include "fm.hpp"

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent),
      fmsynth {static_cast<VizWindow *>(parent)->fmsynth}
{
    QTimer *timer {new QTimer(this)};
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000.0 / FramesPerSecond);
}

void GLWidget::drawLine(float x1, float y1, float x2, float y2)
{
    float vertex[] = {x1, y1, x2, y2};
    glVertexPointer(2, GL_FLOAT, 0, vertex);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_LINES, 0, 2);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void GLWidget::drawGrid(float scale)
{
    glLineWidth(1.0);
    glColor3f(0.5, 0.5, 0.5);

    // x-axis
    const float division {100.0}; // hertz
    const float scaled_width {1.0f / (division * scale)};
    for (int i {-static_cast<int>(scaled_width)}; i <= scaled_width; i ++)
    {
        const float x {scale * (i * division - fmod(fmsynth.carrier_frequency, division))};
        drawLine(x, -1.0, x,  1.0);
    }

    // y-axis
    for (float y {-1.0}; y <= 1.0; y += 0.5)
        drawLine(-1.0, y, 1.0, y);

    // center x-axis
    glLineWidth(2.0);
    drawLine(-fmsynth.carrier_frequency*scale, -1.0, -fmsynth.carrier_frequency*scale, 1.0);
}

void GLWidget::drawBar(float x, float h, float w)
{
    glLineWidth(w);
    drawLine(x, 0.0, x, h);
    /*const float offset = 0.05;
    drawLine(x - offset, h - offset, x, h);
    drawLine(x + offset, h - offset, x, h);*/
}

void GLWidget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, (GLint)width, (GLint)height);
}

void GLWidget::paintGL()
{
    const float scale {static_cast<VizWindow *>(parent())->scale};
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid(scale);
    std::map<float, FMSynth::Band> bands {fmsynth.spectra()};
    for ( auto band : bands )
    {
        switch ( band.second.type )
        {
            case FMSynth::Band::NEGATIVE:
                glColor3f(1.0, 0.0, 0.0);
                break;
            case FMSynth::Band::POSITIVE:
                glColor3f(0.0, 1.0, 0.0);
                break;
            case FMSynth::Band::MIXED:
                glColor3f(1.0, 1.0, 0.0);
                break;
            default:
                glColor3f(1.0, 1.0, 1.0);
        }
        const float x {(band.first - fmsynth.carrier_frequency) * scale};
        drawBar(x, band.second.amplitude, 5.0);
    }
}
