#ifndef _GLWIDGET_H
#define _GLWIDGET_H
#include <QGLWidget>
#include "fm.hpp"

class GLWidget : public QGLWidget
{
    Q_OBJECT

private:
    const int FramesPerSecond {60};
    FMSynth &fmsynth;

public:
    GLWidget(QWidget *parent);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    void drawLine(float x1, float y1, float x2, float y2);
    void drawGrid(float scale);
    void drawBar(float x, float h, float w);
};

#endif // GLWIDGET_H
