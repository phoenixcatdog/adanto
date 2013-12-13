#include "imagesdisplay.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("adanto");
    a.setApplicationVersion("0.1");

#ifndef QT_NO_OPENGL
    ImagesDisplay w;
    w.show();
#else
    QLabel err_mess("This program requires OpenGL");
    err_mess.show();
#endif

    return a.exec();
}
