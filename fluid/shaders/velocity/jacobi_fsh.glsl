R"(

precision highp float;
varying vec2 vTexCoord;

uniform float epsilon;        // Distance between grid units
uniform sampler2D divergence; // Divergence field of advected velocity, d
uniform sampler2D pressure;   // Pressure field from previous iteration, p^(k-1)
varying vec2 textureCoord;

float d(vec2 coord) {
    return texture2D(divergence, fract(coord)).x;
}

float p(vec2 coord) {
    return texture2D(pressure, fract(coord)).x;
}

void main() {
    vec2 textureCoord = vTexCoord;
    gl_FragColor = vec4(0.25 * (
                                d(textureCoord)
                                + p(textureCoord + vec2(2.0 * epsilon, 0.0))
                                + p(textureCoord - vec2(2.0 * epsilon, 0.0))
                                + p(textureCoord + vec2(0.0, 2.0 * epsilon))
                                + p(textureCoord - vec2(0.0, 2.0 * epsilon))
                                ), 0.0, 0.0, 1.0);
}

)";
