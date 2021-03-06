#include "imagesdisplay.h"

//Copyright: Jose Hevia jose.francisco.hevia (at) gmail
//License :GPLv2

#include <QDebug>

ImagesDisplay::ImagesDisplay(QWidget *parent,QApplication *App) :
    QGLWidget(parent = 0)
{
  main_app = App;
  panel = new controlPanel(0,&ima_info);
  visible_control_panel = true;
  full_screen_on        = false;

  threshold_val = 0.5;

  //reload_file_action = new QAction(tr("display load new file"), this);
  connect(panel, SIGNAL(display_new_file()), this, SLOT(load_new_file()));

  connect(panel->hor_slider, SIGNAL(valueChanged(int)), this, SLOT(change_threshold_val(int)));
  connect(panel, SIGNAL(closing_panel()), this, SLOT(toggle_panel()));

  explorer = new scannerFileDialog(0, &ima_info);
  connect(explorer, SIGNAL(load_new_project(int,int,QString*)), this, SLOT(load_new_project(int,int,QString*)));
  visible_explorer_panel = true;
  connect(explorer, SIGNAL(closing_panel()), this, SLOT(toggle_explorer()));

  create_menus();
  ima_info.update_screen = 1;
}

ImagesDisplay::~ImagesDisplay()
{
    deleteTexture(texture);
}

int ImagesDisplay::create_menus(void)
{
  main_menubar = new QMenuBar(this);
  window_menu = main_menubar->addMenu(tr("&Window"));

  toggle_control_panel = new QAction(tr("Toggle Control panel"), this);
  window_menu->addAction(toggle_control_panel);
  connect(toggle_control_panel, SIGNAL(triggered()), this, SLOT(toggle_panel()));

  toggle_explorer_panel = new QAction(tr("Toggle Explorer panel"), this);
  window_menu->addAction(toggle_explorer_panel);
  connect(toggle_explorer_panel, SIGNAL(triggered()), this, SLOT(toggle_explorer()));

  //toggle_full_screen = new QAction(tr("Show full screen"), this);
  //window_menu->addAction(toggle_full_screen);
  //connect(toggle_full_screen, SIGNAL(triggered()), this, SLOT(toggle_fullscreen()));

  main_menubar->show();

  return (0);
}


void ImagesDisplay::toggle_panel()
{
  if (visible_control_panel ==true)
  {
    panel->hide();
    visible_control_panel = false;
  }
  else
  {
    panel->show();
    visible_control_panel = true;
  }
}

void ImagesDisplay::toggle_explorer()
{
  if (visible_explorer_panel ==true)
  {
    explorer->hide();
    visible_explorer_panel = false;
  }
  else
  {
    explorer->show();
    visible_explorer_panel = true;
  }
}

void ImagesDisplay::toggle_fullscreen()
{
  Qt::WindowFlags flags = 0;

  if (full_screen_on == true)
  {
    this->setWindowFlags(flags);
    this->showNormal();
    full_screen_on = false;
  }
  else
  {
    flags |= Qt::FramelessWindowHint;
    flags |= Qt::WindowStaysOnTopHint;
    this->setWindowFlags(flags);
    this->showFullScreen();
    full_screen_on = true;
  }
}

void ImagesDisplay::change_threshold_val(int val)
{
  float  fval;

  fval  = (float)val;
  fval /= 1024;

  if (fval <= 1.0)
  {
      threshold_val = fval;
  }
}

void ImagesDisplay::load_new_project(int level_h,int level_v, QString *folder)
{
  image_info *info;

  info = &ima_info;

  info->level_h_max = level_h;
  info->level_v_max = level_v;
  info->folder = folder;
  info->info_flag = 0;
  info->info_flag |= INFO_FLAG_DRAW_IMAGE;

  info->selected_list_item  = 0;
  info->selected_orientation = 0;

  qDebug() << "levelY_h is " << level_h;
  qDebug() << "level_v is " << level_v;
  qDebug() << "folder is " << *folder;

  panel->update_control_panel(info);
  panel->update_list_view(info);
  kernel.load_texture_from_image(info);
  load_texture(info);
  kernel.update_image_on_screen(info);
  recalculate_perspective(info->display_w,info->display_h);
  show();
}

void ImagesDisplay::load_new_file(void)
{
  image_info *info;

  info = &ima_info;

  kernel.load_texture_from_image(info);
  load_texture(info);
  kernel.update_image_on_screen(info);
  recalculate_perspective(info->display_w,info->display_h);
  show();
}

void ImagesDisplay::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press coords in pixels
    last_press_pos = QVector2D(e->localPos());
}

void ImagesDisplay::mouseReleaseEvent(QMouseEvent *e)
{
    // This will be useful in the future
//    QVector2D diff = QVector2D(e->localPos()) - last_press_pos;

}

void ImagesDisplay::timerEvent(QTimerEvent *)
{

  // Update drawing
  updateGL();

}

void ImagesDisplay::init_shaders()
{
    // Override system locale to use simple c locale for the compilation
    setlocale(LC_NUMERIC, "C");

    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QGLShader::Vertex, "://shaders/vx_image_threshold.glsl"))
    { close();}

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QGLShader::Fragment, "://shaders/fr_image_threshold.glsl"))
    {  close();}

    // Link shader pipeline
    if (!program.link())
    {  close();}

    // Select shader as current
    if (!program.bind())
    {  close();}

    //Compile threshold program
    // Compile vertex shader
    if (!program_threshold.addShaderFromSourceFile(QGLShader::Vertex, "://shaders/vx_image_threshold.glsl"))
    { close();}

    // Compile fragment shader
    if (!program_threshold.addShaderFromSourceFile(QGLShader::Fragment, "://shaders/fr_image_threshold2.glsl"))
    {  close();}

    // Link shader pipeline
    if (!program_threshold.link())
    {  close();}

    // Select shader as current
    if (!program_threshold.bind())
    {  close();}

    // Restore system locale
    setlocale(LC_ALL, "");
}

void ImagesDisplay::initializeGL()
{
    image_aspect       =  1.0;
    ima_info.no_textures =  1;
    initializeGLFunctions();
    //qglClearColor(Qt::black);
    this->view_proj_function[0] = &ImagesDisplay::recalculate_perspective_frustrum;
    this->view_proj_function[1] = &ImagesDisplay::recalculate_perspective_frustrum;
    this->view_proj_function[2] = &ImagesDisplay::recalculate_perspective_frustrum;
    this->view_proj_function[3] = &ImagesDisplay::recalculate_perspective;
    this->view_proj_function[4] = &ImagesDisplay::recalculate_perspective;
    this->view_proj_function[5] = &ImagesDisplay::recalculate_perspective;

    this->paint_function[0] = &ImagesDisplay::paint_camera_frustrum;
    this->paint_function[1] = &ImagesDisplay::paint_projector_frustrum;
    this->paint_function[2] = &ImagesDisplay::paint_set_view;
    this->paint_function[3] = &ImagesDisplay::paint_preview_project;
    this->paint_function[4] = &ImagesDisplay::paint_projector_analysis;
    this->paint_function[5] = &ImagesDisplay::paint_3d_output;

    ima_info.current_step = 3;
    ima_info.step_color[0] = new QColor(Qt::blue);
    ima_info.step_color[1] = new QColor(Qt::magenta);
    ima_info.step_color[2] = new QColor(Qt::yellow);
    ima_info.step_color[3] = new QColor(Qt::red);
    ima_info.step_color[4] = new QColor(Qt::cyan);
    ima_info.step_color[5] = new QColor(Qt::green);

    qglClearColor(Qt::red);
    init_shaders();
    glEnable(GL_TEXTURE_2D);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling so it is more efficient
    glEnable(GL_CULL_FACE);

    kernel.init();

    kernel.create_grid(&ima_info, 10, 10);

    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
}


void ImagesDisplay::load_texture(image_info *info)
{
  QImage          *ima_loaded;
  qreal                 width;
  qreal                height;

  ima_loaded = info->ima_loaded;

  if (ima_loaded == NULL)
  {  return;  }
  if (info->no_textures)
  { info->no_textures = 0;}
  else
  { deleteTexture(texture); }
  texture = bindTexture(*(ima_loaded));

  //FIXME Assert all pictures have the same dimentions in the future
  width = ima_loaded->width();
  height = ima_loaded->height();
  if (height == 0){height = 1.0;}
  image_aspect = width/height;//Aspect ratio of image

  // Set nearest filtering mode for texture minification
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Set bilinear filtering mode for texture magnification
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Make texture coordinates to repeat, not really useful as we will never get out of the vertex coords
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  return;
}


void ImagesDisplay::recalculate_perspective_frustrum(int w, int h)
{
    qreal   val_x;
    qreal   val_y;
    // Set OpenGL viewport to cover whole widget
    glViewport(0, 0, w, h);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -5.0);
    //glRotatef(cameraAngleX, 1, 0, 0); // pitch
    //glRotatef(cameraAngleY, 0, 1, 0); // heading

    // Calculate aspect ratio with precision on all plattforms
    //qreal  val;

    if (h == 0)
    {h = 1;}
    qreal aspect = qreal(w) / qreal(h);

    const qreal zNear = 1.0;//Near plane depth
    const qreal  zFar = 10.0;//far plane depth
    const qreal  fov = 45.0;//field of view 45 degrees

    // Reset projection

    val_x = 1.0;
    val_y = 1.0;

    if(aspect > image_aspect)
    {val_x = aspect/image_aspect;}
    else
    {val_y = image_aspect/aspect;}

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    // Set perspective projection
    glOrtho(-val_x,val_x,-val_y, val_y,zNear,zFar);
    glMatrixMode (GL_MODELVIEW);
}

void ImagesDisplay::recalculate_perspective(int w, int h)
{
    qreal   val_x;
    qreal   val_y;
    // Set OpenGL viewport to cover whole widget
    glViewport(0, 0, w, h);

    //When we resize we select betwwen two modes:
    //MODE IMAGE= For viewing images at full screen
    //MODE 3D for viewing 3D cloud of recognized particles

    // Calculate aspect ratio with precision on all plattforms
    //qreal  val;

    if (h == 0)
    {h = 1;}
    qreal aspect = qreal(w) / qreal(h);

    const qreal zNear = 3.0;//Near plane depth
    const qreal  zFar = 7.0;//far plane depth
    //const qreal  fov = 45.0;//field of view 45 degrees

    // Reset projection
    projection.setToIdentity();

    val_x = 1.0;
    val_y = 1.0;

    if(aspect > image_aspect)
    {val_x = aspect/image_aspect;}
    else
    {val_y = image_aspect/aspect;}

    projection.ortho(-val_x,val_x,-val_y,val_y, zNear, zFar);

    // Set perspective projection
    //projection.perspective(fov, aspect, zNear, zFar);
}

void ImagesDisplay::resizeGL(int w, int h)
{
    ima_info.display_h = h;
    ima_info.display_w = w;
    (this->*view_proj_function[(ima_info.current_step)])(w,h);
}


void ImagesDisplay::paint_camera_frustrum()
{

    glUseProgram(0);

    glColor3f(1.0,1.0,1.0);
    glBindBuffer(GL_ARRAY_BUFFER, kernel.grid_handles[0]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_LINES, 0, 2 * (20+2));
}

void ImagesDisplay::paint_projector_frustrum()
{
    float                     val;
    QMatrix4x4             matrix;
    QGLShaderProgram *sel_program;

    // Calculate model view transformation
    matrix.translate(0.0, 0.0, -5.0);
    //matrix.rotate(rotation);

    if (ima_info.preview_mode == 0)
    {  sel_program = &program;  }
    else
    {  sel_program = &program_threshold;  }
    sel_program->bind();

    // Set modelview-projection matrix
    sel_program->setUniformValue("mvp_matrix", projection *matrix);

    // Default texture unit which contains the texture from resources or file
    sel_program->setUniformValue("texture", 0);

    val = threshold_val;
    sel_program->setUniformValue("threshold_val",val);

    if (ima_info.info_flag & INFO_FLAG_DRAW_IMAGE)
    {
      kernel.draw_image_to_screen(sel_program);
    }
}

void ImagesDisplay::paint_set_view()
{
    float                     val;
    QMatrix4x4             matrix;
    QGLShaderProgram *sel_program;

    // Calculate model view transformation
    matrix.translate(0.0, 0.0, -5.0);
    //matrix.rotate(rotation);

    if (ima_info.preview_mode == 0)
    {  sel_program = &program;  }
    else
    {  sel_program = &program_threshold;  }
    sel_program->bind();

    // Set modelview-projection matrix
    sel_program->setUniformValue("mvp_matrix", projection *matrix);

    // Default texture unit which contains the texture from resources or file
    sel_program->setUniformValue("texture", 0);

    val = threshold_val;
    sel_program->setUniformValue("threshold_val",val);

    if (ima_info.info_flag & INFO_FLAG_DRAW_IMAGE)
    {
      kernel.draw_image_to_screen(sel_program);
    }
}

void ImagesDisplay::paint_preview_project()
{
    float                     val;
    QMatrix4x4             matrix;
    QGLShaderProgram *sel_program;

    // Calculate model view transformation
    matrix.translate(0.0, 0.0, -5.0);
    //matrix.rotate(rotation);

    if (ima_info.preview_mode == 0)
    {  sel_program = &program;  }
    else
    {  sel_program = &program_threshold;  }
    sel_program->bind();

    // Set modelview-projection matrix
    sel_program->setUniformValue("mvp_matrix", projection *matrix);

    // Default texture unit which contains the texture from resources or file
    sel_program->setUniformValue("texture", 0);

    val = threshold_val;
    sel_program->setUniformValue("threshold_val",val);

    if (ima_info.info_flag & INFO_FLAG_DRAW_IMAGE)
    {
      kernel.draw_image_to_screen(sel_program);
    }
}


void ImagesDisplay::paint_projector_analysis()
{
    float                     val;
    QMatrix4x4             matrix;
    QGLShaderProgram *sel_program;

    // Calculate model view transformation
    matrix.translate(0.0, 0.0, -5.0);
    //matrix.rotate(rotation);

    if (ima_info.preview_mode == 0)
    {  sel_program = &program;  }
    else
    {  sel_program = &program_threshold;  }
    sel_program->bind();

    // Set modelview-projection matrix
    sel_program->setUniformValue("mvp_matrix", projection *matrix);

    // Default texture unit which contains the texture from resources or file
    sel_program->setUniformValue("texture", 0);

    val = threshold_val;
    sel_program->setUniformValue("threshold_val",val);

    if (ima_info.info_flag & INFO_FLAG_DRAW_IMAGE)
    {
      kernel.draw_image_to_screen(sel_program);
    }
}

void ImagesDisplay::paint_3d_output()
{
    float                     val;
    QMatrix4x4             matrix;
    QGLShaderProgram *sel_program;

    // Calculate model view transformation
    matrix.translate(0.0, 0.0, -5.0);
    //matrix.rotate(rotation);

    if (ima_info.preview_mode == 0)
    {  sel_program = &program;  }
    else
    {  sel_program = &program_threshold;  }
    sel_program->bind();

    // Set modelview-projection matrix
    sel_program->setUniformValue("mvp_matrix", projection *matrix);

    // Default texture unit which contains the texture from resources or file
    sel_program->setUniformValue("texture", 0);

    val = threshold_val;
    sel_program->setUniformValue("threshold_val",val);

    if (ima_info.info_flag & INFO_FLAG_DRAW_IMAGE)
    {
      kernel.draw_image_to_screen(sel_program);
    }
}


void ImagesDisplay::paintGL()
{
  // if (!(ima_info.update_screen))
  // {return;}
  // ima_info.update_screen = 0;

  // Clear color and depth buffer
  qglClearColor(*(ima_info.step_color[(ima_info.current_step)]));
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  (this->*paint_function[(ima_info.current_step)])();
}

void ImagesDisplay::closeEvent(QCloseEvent *event)
{
  main_app->quit();
}
