#include "images_kernel_gl.h"

//Copyright: Jose Hevia jose.francisco.hevia (at) gmail
//License :GPLv2

#include <QVector2D>
#include <QVector3D>

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

    // Generate 2 VBOs
    glGenBuffers(2, vbo_handles);

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

void     Images_kernel_gl::update_image_on_screen(image_info *info)
{

}
