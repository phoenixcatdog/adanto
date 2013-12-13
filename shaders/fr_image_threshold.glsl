#ifdef GL_ES
// Set default precision to medium
  precision mediump int;
  precision mediump float;
#endif

uniform sampler2D texture;

varying vec2 v_texcoord;

void main()
{
    // Fragment color to texture
    gl_FragColor = texture2D(texture, v_texcoord);
}
