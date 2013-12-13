#ifndef IMAGESDISPLAY_H
#define IMAGESDISPLAY_H

#include "images_kernel_gl.h"
#include "controlPanel.h"

#include <QGLWidget>
#include <QGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QGLShaderProgram>

#include <QMouseEvent>

#include <math.h>
#include <locale.h>

class Images_kernel_gl;

class ImagesDisplay : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT

public:
    explicit ImagesDisplay(QWidget *parent = 0);
    ~ImagesDisplay();

public slots:
    void change_threshold_val(int);

protected:
    void   mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void        timerEvent(QTimerEvent *e);

    void                    initializeGL();//Those three are standard QGLWidget methods
    void            resizeGL(int w, int h);
    void                         paintGL();

    void                    init_shaders();
    void                   init_textures();

private:
    QBasicTimer            timer;
    QGLShaderProgram     program;
    Images_kernel_gl      kernel;

    GLuint               texture;

    QMatrix4x4        projection;

    QVector2D     last_press_pos;

    controlPanel          *panel;
    qreal           image_aspect;
    float          threshold_val;//Threshold val range = 0-1.0
};

#endif // IMAGESDISPLAY_H
