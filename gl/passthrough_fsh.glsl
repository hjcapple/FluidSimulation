R"(

precision mediump float;
varying vec2 vTexCoord;
uniform sampler2D uTexture0;

void main()
{
    vec4 color = texture2D(uTexture0, vTexCoord);
    gl_FragColor = color;
}

)";
