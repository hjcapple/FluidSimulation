R"(

attribute vec4 aPosition;
attribute vec4 aTextureCoord;
varying vec2 vTexCoord;

void main()
{
    gl_Position = aPosition;
    vTexCoord = aTextureCoord.xy;
}

)";


