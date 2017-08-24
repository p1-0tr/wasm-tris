attribute vec4 vPosition;

varying vec2 Texcoord;

uniform mat4 projection;

void main()
{
    Texcoord = vPosition.zw;
    gl_Position = projection * vec4(vPosition.xy, 0.0, 1.0);
}
