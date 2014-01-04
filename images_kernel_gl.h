#ifndef IMAGES_KERNEL_GL_H
#define IMAGES_KERNEL_GL_H

//Copyright: Jose Hevia jose.francisco.hevia (at) gmail
//License :GPLv2

#include <QGLFunctions>
#include <QGLShaderProgram>

#include "inter_class_data.h"

class Images_kernel_gl : public QGLFunctions
{
public:
  Images_kernel_gl();
  virtual ~Images_kernel_gl();

  void                                            init();
  void          draw_image_to_screen(QGLShaderProgram *);
   int             load_texture_from_image(image_info *);
  void              update_image_on_screen(image_info *);
  void                       init_frustrum(image_info *);
  void                     modify_frustrum(image_info *);

  void                  create_grid(image_info*,int,int);
  void                            draw_grid(image_info*);
  gl_fbuffer*                     create_gl_fbuffer(int);
  void add_point_to_gl_fbuffer(gl_fbuffer *,GLfloat,GLfloat,GLfloat);

  GLuint            grid_handles[3];
private:
  void init_images_display();

  gl_fbuffer           *grid_buffer;

  GLuint             vbo_handles[2];
  GLuint     camera_frustrum_vbo[2];
  GLuint  projector_frustrum_vbo[2];
};

#endif // IMAGES_KERNEL_GL_H
