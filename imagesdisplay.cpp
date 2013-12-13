#include "imagesdisplay.h"

//Copyright: Jose Hevia jose.francisco.hevia (at) gmail
//License :GPLv2

ImagesDisplay::ImagesDisplay(QWidget *parent) :
    QGLWidget(parent)
{
    panel = new controlPanel(this);

    threshold_val = 0.5;
    connect(panel->hor_slider, SIGNAL(valueChanged(int)), this, SLOT(change_threshold_val(int)));
}

ImagesDisplay::~ImagesDisplay()
{
    deleteTexture(texture);
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
    if (!program.addShaderFromSourceFile(QGLShader::Fragment, "://shaders/fr_image_threshold2.glsl"))
    {  close();}

    // Link shader pipeline
    if (!program.link())
    {  close();}

    // Select shader as current
    if (!program.bind())
    {  close();}

    // Restore system locale
    setlocale(LC_ALL, "");
}

void ImagesDisplay::init_textures()
{
    QImage    ima_loaded;
    qreal          width;
    qreal         height;

    // Default texture for making tests, we will dynamically load them in the future
    glEnable(GL_TEXTURE_2D);
    ima_loaded = QImage("://texture_examples/H00.png");
    texture = bindTexture(ima_loaded);

    width = ima_loaded.width();
    height = ima_loaded.height();
    if (height == 0){height = 1.0;}
    image_aspect = width/height;//Aspect ratio of image

    // Set nearest filtering mode for texture minification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Set bilinear filtering mode for texture magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Make texture coordinates to repeat, not really useful as we will never get out of the vertex coords
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void ImagesDisplay::initializeGL()
{
    initializeGLFunctions();
    //qglClearColor(Qt::black);
    qglClearColor(Qt::red);
    init_shaders();
    init_textures();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling so it is more efficient
    glEnable(GL_CULL_FACE);

    kernel.init();

    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
}

void ImagesDisplay::resizeGL(int w, int h)
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

void ImagesDisplay::paintGL()
{
    QMatrix4x4 matrix;
    float         val;

    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Calculate model view transformation
    matrix.translate(0.0, 0.0, -5.0);
    //matrix.rotate(rotation);

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection *matrix);

    // Default texture unit which contains the texture from resources or file
    program.setUniformValue("texture", 0);

    val = threshold_val;
    program.setUniformValue("threshold_val",val);

    kernel.draw_image_to_screen(&program);
}
