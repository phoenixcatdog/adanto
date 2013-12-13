#ifndef IMAGES_KERNEL_GL_H
#define IMAGES_KERNEL_GL_H

//Copyright: Jose Hevia jose.francisco.hevia (at) gmail
//License :GPLv2

#include <QGLFunctions>
#include <QGLShaderProgram>

class Images_kernel_gl : public QGLFunctions
{
public:
    Images_kernel_gl();
    virtual ~Images_kernel_gl();

    void init();
    void draw_image_to_screen(QGLShaderProgram *program);

private:
    void init_images_display();

    GLuint      vbo_handles[2];
};

#endif // IMAGES_KERNEL_GL_H
