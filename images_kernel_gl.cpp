#include "images_kernel_gl.h"

//Copyright: Jose Hevia jose.francisco.hevia (at) gmail
//License :GPLv2

#include <QVector2D>
#include <QVector3D>


#include <QDebug>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

Images_kernel_gl::Images_kernel_gl()
{
}

Images_kernel_gl::~Images_kernel_gl()
{
    glDeleteBuffers(2, vbo_handles);
}

void Images_kernel_gl::init()
{
    initializeGLFunctions();

    // Generate Lots of VBOs
    glGenBuffers(2, vbo_handles);
    glGenBuffers(2, camera_frustrum_vbo);
    glGenBuffers(2, projector_frustrum_vbo);
    glGenBuffers(3, grid_handles);

    // Initializes Image geometry
    init_images_display();
}

void Images_kernel_gl::init_images_display()
{
    VertexData vertices[] =
    {
        //Draw the face that contains the image
        {QVector3D(-1.0,  1.0,  0.0), QVector2D(0.0, 0.0)}, // v0
        {QVector3D(-1.0, -1.0,  0.0), QVector2D(0.0, 1.0)}, // v1
        {QVector3D( 1.0,  1.0,  0.0), QVector2D(1.0, 0.0)}, // v2
        {QVector3D( 1.0, -1.0,  0.0), QVector2D(1.0, 1.0)}, // v3
    };

    GLushort indices[] =
    {
         0,  1,  2,  2,  1,  3     // Image face
    };

    // Transfer vertex data to VBO 0
    glBindBuffer(GL_ARRAY_BUFFER, vbo_handles[0]);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(VertexData), vertices, GL_STATIC_DRAW);

    // Transfer index data to VBO 1
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_handles[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), indices, GL_STATIC_DRAW);

}

void Images_kernel_gl::init_frustrum(image_info *info)
{
    GLuint *frustrum;

    VertexData vertices[] =
    {
        //Draw the face that contains the image
        {QVector3D(-1.0,  1.0,  0.0), QVector2D(0.0, 0.0)}, // v0
        {QVector3D(-1.0, -1.0,  0.0), QVector2D(0.0, 1.0)}, // v1
        {QVector3D( 1.0,  1.0,  0.0), QVector2D(1.0, 0.0)}, // v2
        {QVector3D( 1.0, -1.0,  0.0), QVector2D(1.0, 1.0)}, // v3
    };

    GLushort indices[] =
    {
         0,  1,  2,  2,  1,  3     // Image face
    };

    if ((info->current_step)== 0)
    {  frustrum = &(camera_frustrum_vbo[0]); }
    else
    {  frustrum = &(projector_frustrum_vbo[0]); }

    // Transfer vertex data to VBO 0
    glBindBuffer(GL_ARRAY_BUFFER, *frustrum);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(VertexData), vertices, GL_DYNAMIC_DRAW);

    if ((info->current_step)== 0)
    {  frustrum = &(camera_frustrum_vbo[1]); }
    else
    {  frustrum = &(projector_frustrum_vbo[1]); }

    // Transfer index data to VBO 1
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *frustrum);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), indices, GL_DYNAMIC_DRAW);

}

void Images_kernel_gl::modify_frustrum(image_info *info)
{
    GLuint *frustrum;

    VertexData vertices[] =
    {
        //Draw the face that contains the image
        {QVector3D(-1.0,  1.0,  0.0), QVector2D(0.0, 0.0)}, // v0
        {QVector3D(-1.0, -1.0,  0.0), QVector2D(0.0, 1.0)}, // v1
        {QVector3D( 1.0,  1.0,  0.0), QVector2D(1.0, 0.0)}, // v2
        {QVector3D( 1.0, -1.0,  0.0), QVector2D(1.0, 1.0)}, // v3
    };

    GLushort indices[] =
    {
         0,  1,  2,  2,  1,  3     // Image face
    };

    if ((info->current_step)== 0)
    {  frustrum = &(camera_frustrum_vbo[0]); }
    else
    {  frustrum = &(projector_frustrum_vbo[0]); }

    // Transfer vertex data to VBO 0
    glBindBuffer(GL_ARRAY_BUFFER, *frustrum);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(VertexData), vertices, GL_DYNAMIC_DRAW);

    if ((info->current_step)== 0)
    {  frustrum = &(camera_frustrum_vbo[1]); }
    else
    {  frustrum = &(projector_frustrum_vbo[1]); }

    // Transfer index data to VBO 1
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *frustrum);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), indices, GL_DYNAMIC_DRAW);

}


gl_fbuffer*
Images_kernel_gl::create_gl_fbuffer(int size)
{
  gl_fbuffer *complete_buffer;
  GLfloat             *buffer;

  complete_buffer = (gl_fbuffer*)malloc(sizeof(gl_fbuffer));
  complete_buffer->size = size;
  size *= sizeof(GLfloat);
  complete_buffer->size_bytes = size;
  complete_buffer->elements = 0;//Empty coords,(3 elemements per 3d point)
  buffer = (GLfloat*)malloc(size);
  complete_buffer->buffer = buffer;
  return (complete_buffer);
}


void Images_kernel_gl::add_point_to_gl_fbuffer(gl_fbuffer *buffer,GLfloat x_val,GLfloat y_val,GLfloat z_val)
{
  int      elements;
  GLfloat *p_buffer;

  elements = buffer->elements;
  if ((elements + 3) > buffer->size)
  {exit (-1);}

  p_buffer = buffer->buffer;

  p_buffer[elements  ] = x_val;
  p_buffer[elements+1] = y_val;
  p_buffer[elements+2] = z_val;

  buffer->elements += 3;
}

void Images_kernel_gl::create_grid(image_info *info, int h_lines, int v_lines)
{
    GLfloat  delta_v;
    GLfloat  delta_h;
    GLfloat    x_val;
    GLfloat    y_val;
    GLfloat    z_val;

    int            i;
    int         size;

    size  = (h_lines+1) * 2 * 3+ (v_lines+1) * 2 *3;//2 points per line, (x,y,z) coords
    grid_buffer = create_gl_fbuffer(size);

    delta_v = 2.0 / ((GLfloat)(v_lines));
    delta_h = 2.0 / ((GLfloat)(h_lines));

    //Draw vertical lines
    for ( i = 0 ; i <= v_lines ; i++)
    {
      x_val = -1.0 + (delta_v * ((GLfloat)i));
      y_val = -1.0;
      z_val =  0.0;
      add_point_to_gl_fbuffer(grid_buffer,x_val,y_val,z_val);
      y_val = 1.0;
      add_point_to_gl_fbuffer(grid_buffer,x_val,y_val,z_val);
    }

    //Draw Horizontal lines
    for ( i = 0 ; i <= h_lines ; i++)
    {
      x_val = -1.0;
      y_val = -1.0 + (delta_h * ((GLfloat)i));
      z_val =  0.0;
      add_point_to_gl_fbuffer(grid_buffer, x_val, y_val, z_val);
      x_val = 1.0;
      add_point_to_gl_fbuffer(grid_buffer, x_val, y_val, z_val);
    }

    glBindBuffer(GL_ARRAY_BUFFER, grid_handles[0]);
    glBufferData(GL_ARRAY_BUFFER, grid_buffer->size_bytes,grid_buffer->buffer, GL_DYNAMIC_DRAW);
}

void Images_kernel_gl::draw_image_to_screen(QGLShaderProgram *program)
{
    // Assign current vbos
    glBindBuffer(GL_ARRAY_BUFFER, vbo_handles[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_handles[1]);

    // Offset for vertex coords buffer
    quintptr offset = 0;

    // send vertex coords reference to shader
    int vertexLocation = program->attributeLocation("vertex_coords");
    program->enableAttributeArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);

    // Offset for texture UV coords buffer
    offset += sizeof(QVector3D);

    // Send texture UV coords to shader
    int texcoordLocation = program->attributeLocation("vertex_uv_coords");
    program->enableAttributeArray(texcoordLocation);
    glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);

    // Draw image from VBO
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}

int Images_kernel_gl::load_texture_from_image(image_info *info)
{
  int             level;
  int         error_val;
  int    rewriting_size;

  level   =  0;

  QString  slash = "/";
  QString extension = QString(".jpg");
  QChar           *digit1;
  QChar           *digit2;

  QString id     = "H";
  if (info->selected_orientation == 1)
  {  id.replace(0,1,"V");}

  QString number_l1  = "00";
  QString number_l2  = "00";

  QString name = *(info->folder);
  rewriting_size = (info->folder)->size() + slash.size() + id.size() + number_l1.size();

  name.reserve( rewriting_size + number_l2.size() + extension.size() );
  name += slash;
  name += id;
  name += number_l1;
  name += number_l2;
  name += extension;

  level = info->selected_list_item;
  fill_digits(info,level, &digit1, &digit2);
  name.replace(rewriting_size  ,1,*digit1);
  name.replace(rewriting_size+1,1,*digit2);

  qDebug() << "folder 2 is " << name;

  error_val = 0;//No errors if everything goes well

  //FIXME No error handling, we will do it later
  if ((info->ima_loaded) != 0)
  {delete info->ima_loaded;}
  info->ima_loaded = new QImage(name);

  if (error_val != 0 )
  {  return(error_val);  }

  return(error_val);
}

void     Images_kernel_gl::update_image_on_screen(image_info *info)
{

}
