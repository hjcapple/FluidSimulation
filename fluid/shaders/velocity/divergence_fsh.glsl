R"(

precision highp float;
varying vec2 vTexCoord;

uniform float deltaT;         // Time between steps
uniform float rho;            // Density
uniform float epsilon;        // Distance between grid units
uniform sampler2D velocity;   // Advected velocity field, u_a

vec2 u(vec2 coord) {
    return texture2D(velocity, fract(coord)).xy;
}

void main() {
    vec2 textureCoord = vTexCoord;
    gl_FragColor = vec4((-2.0 * epsilon * rho / deltaT) * (
                                                           (u(textureCoord + vec2(epsilon, 0)).x -
                                                            u(textureCoord - vec2(epsilon, 0)).x)
                                                           +
                                                           (u(textureCoord + vec2(0, epsilon)).y -
                                                            u(textureCoord - vec2(0, epsilon)).y)
                                                           ), 0.0, 0.0, 1.0);
}

)";
