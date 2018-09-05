R"(

precision highp float;
varying vec2 vTexCoord;

uniform float deltaT;
uniform sampler2D inputTexture;
uniform sampler2D velocity;

void main() {
    
    vec2 textureCoord = vTexCoord;
    vec2 u = texture2D(velocity, textureCoord).xy;
    vec2 pastCoord = fract(textureCoord - (0.5 * deltaT * u));
    gl_FragColor = texture2D(inputTexture, pastCoord);
}

)";
