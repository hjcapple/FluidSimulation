R"(

precision highp float;
varying vec2 vTexCoord;
uniform sampler2D uTexture0;

void main()
{
   gl_FragColor = texture2D(uTexture0, vTexCoord);
}

)";
