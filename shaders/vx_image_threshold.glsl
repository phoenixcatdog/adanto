#ifdef GL_ES
// Set default precision to medium
precision mediump   int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;

attribute vec4    vertex_coords;
attribute vec2 vertex_uv_coords;

varying vec2 v_texcoord;

void main()
{
    // Vertex geometric transformation to screen coords
    gl_Position = mvp_matrix * vertex_coords;

    //Output texture coordinates to fragment input without transformation 
    v_texcoord = vertex_uv_coords;
}
