#ifdef GL_ES
//Copyright: Jose Hevia jose.francisco.hevia (at) gmail
//License :GPLv2
// Set default precision to medium
  precision mediump   int;
  precision mediump float;
#endif

uniform sampler2D   texture;
uniform float threshold_val;

varying vec2 v_texcoord;

void main()
{
    vec4  tex_color;
    float intensity;

    // Fragment color to texture
    tex_color = texture2D(texture, v_texcoord);
    intensity = tex_color.r + tex_color.g + tex_color.b;
    intensity /= 3.0;
    if(intensity < threshold_val)
    {discard;}
    else
    {
        gl_FragColor.r = 1.0;
        gl_FragColor.g = 1.0;
        gl_FragColor.b = 1.0;
    }
}
