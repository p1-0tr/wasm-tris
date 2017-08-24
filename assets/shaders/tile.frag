precision mediump float;

varying vec2 Texcoord;

uniform sampler2D tex;
uniform vec4 color;

void main()
{
    gl_FragColor = texture2D(tex, Texcoord) + color;
}
