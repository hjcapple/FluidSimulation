R"(

precision highp float;
varying vec2 vTexCoord;

uniform float deltaT;         // Time between steps
uniform float rho;            // Density
uniform float epsilon;        // Distance between grid units
uniform sampler2D velocity;   // Advected velocity field, u_a
uniform sampler2D pressure;   // Solved pressure field

float p(vec2 coord) {
    return texture2D(pressure, fract(coord)).x;
}

void main() {
    vec2 textureCoord = vTexCoord;
    vec2 u_a = texture2D(velocity, textureCoord).xy;
    
    float diff_p_x = (p(textureCoord + vec2(epsilon, 0.0)) -
                      p(textureCoord - vec2(epsilon, 0.0)));
    float u_x = u_a.x - deltaT/(2.0 * rho * epsilon) * diff_p_x;
    
    float diff_p_y = (p(textureCoord + vec2(0.0, epsilon)) -
                      p(textureCoord - vec2(0.0, epsilon)));
    float u_y = u_a.y - deltaT/(2.0 * rho * epsilon) * diff_p_y;
    
    gl_FragColor = vec4(u_x, u_y, 0.0, 1.0);
}

)";
