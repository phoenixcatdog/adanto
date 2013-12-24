#include "imagesdisplay.h"
#include <QApplication>
#include <QObject>

//Copyright: Jose Hevia jose.francisco.hevia (at) gmail
//License :GPLv2


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("adanto");
    a.setApplicationVersion("0.1");

#ifndef QT_NO_OPENGL
    ImagesDisplay w(NULL,&a);
    w.show();
#else
    QLabel err_mess("This program requires OpenGL");
    err_mess.show();
#endif

    return a.exec();
}
