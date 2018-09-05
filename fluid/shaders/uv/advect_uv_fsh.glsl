R"(

precision highp float;
varying vec2 vTexCoord;

uniform float deltaT;
uniform sampler2D offsetUV;
uniform sampler2D velocity;

void main() {
    vec2 offset = texture2D(offsetUV, vTexCoord).xy;
    vec2 u = texture2D(velocity, fract(vTexCoord + offset)).xy;
    vec2 result = offset + deltaT * u;
    gl_FragColor = vec4(result.x, result.y, 0.0, 0.0);
}

)";
