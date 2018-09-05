R"(

precision highp float;
varying vec2 vTexCoord;

void main()
{
    float x = 2.0 * vTexCoord.x - 1.0;
    float y = 2.0 * vTexCoord.y - 1.0;
    gl_FragColor = vec4(sin(2.0 * 3.1415 * y), sin(2.0 * 3.1415 * x), 0.0, 1.0);
  //  gl_FragColor = vec4(0.1, 0, 0.0, 1.0);
}

)";
