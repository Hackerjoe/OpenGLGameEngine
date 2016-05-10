uniform sampler2D tex;
uniform vec4 diffuseColor;

void main()
{
    vec4 color = texture2D(tex,gl_TexCoord[0].st);
    vec4 diffuseColor = vec4(1.0,0,0.0,1.0);
    gl_FragColor = color * diffuseColor;
}