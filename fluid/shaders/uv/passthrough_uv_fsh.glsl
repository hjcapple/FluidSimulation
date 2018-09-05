R"(

precision highp float;
varying vec2 vTexCoord;
uniform sampler2D uTexture0;
uniform sampler2D uOffsetUV;

void main()
{
    vec2 offset = texture2D(uOffsetUV, vTexCoord).xy;
    gl_FragColor = texture2D(uTexture0, fract(vTexCoord + offset));
}

)";
