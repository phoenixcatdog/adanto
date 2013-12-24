#ifndef IMAGESDISPLAY_H
#define IMAGESDISPLAY_H

//Copyright: Jose Hevia jose.francisco.hevia (at) gmail
//License :GPLv2

#include "images_kernel_gl.h"
#include "controlPanel.h"
#include "scannerfiledialog.h"
#include "inter_class_data.h"

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

#include <QAction>
#include <QApplication>
#include <QMenuBar>
#include <QMenu>

class Images_kernel_gl;//Import all Opengl functions

class ImagesDisplay : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT

public:
    ImagesDisplay(QWidget *,QApplication *);
    ~ImagesDisplay();

public slots:
    void change_threshold_val(int);
    void    load_new_project(int, int, QString *);
    void          toggle_panel();
    void       toggle_explorer();

protected:
    void   mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void        timerEvent(QTimerEvent *e);

    void                    initializeGL();//Those three are standard QGLWidget methods
    void            resizeGL(int w, int h);
    void                         paintGL();

    void                    init_shaders();
    void                   init_textures();

    void         closeEvent(QCloseEvent *);

private:
    QApplication       *main_app;
    QBasicTimer            timer;
    QGLShaderProgram     program;
    Images_kernel_gl      kernel;

    GLuint               texture;

    QMatrix4x4        projection;

    QVector2D     last_press_pos;

    image_info          ima_info;

    controlPanel          *panel;
    scannerFileDialog  *explorer;
    qreal           image_aspect;
    float          threshold_val;//Threshold val range = 0-1.0

    QMenu             *window_menu;
    QAction         *window_action;
    QAction  *toggle_control_panel;
    QAction *toggle_explorer_panel;

    bool     visible_control_panel;
    bool    visible_explorer_panel;

    int        create_menus(void);
    QMenuBar        *main_menubar;
    QAction          *help_action;
    QAction          *exit_action;

};

#endif // IMAGESDISPLAY_H
