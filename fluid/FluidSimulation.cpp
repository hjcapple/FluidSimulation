#include "./FluidSimulation.h"
#include "./shaders/shaders.h"

namespace glutils {

    FluidSimulation::FluidSimulation() {
    }

    FluidSimulation::~FluidSimulation() {
        safeDelete(tmpTexture);
        safeDelete(color);
        safeDelete(divergence);
        safeDelete(velocity);
        safeDelete(pressure);
        safeDelete(offsetUV);
    }

    void FluidSimulation::initOpenGL(const TextureInfo &inTex) {
        FluidSimulation &d = *this;

        int width = 600.0;
        int height = 600.0;

        if (tmpTexture == nullptr) {
            tmpTexture = new Texture();
            tmpTexture->createFloat16(width, height);
        }

        if (velocity == nullptr) {
            velocity = new Texture();
            velocity->createFloat16(width, height);
        }

        if (pressure == nullptr) {
            pressure = new Texture();
            pressure->createFloat16(width, height);
        }

        if (offsetUV == nullptr) {
            offsetUV = new Texture();
            offsetUV->createFloat16(width, height);
        }

        if (divergence == nullptr) {
            divergence = new Texture();
            divergence->createFloat16(width, height);
        }

        if (color == nullptr) {
            color = new Texture();
            color->createFloat16(width, height);
        }

        d.drawToTexture(velocity, [&] {
            Program *pass = d.initVelocityPass();
            pass->use();
            return pass;
        });

        d.drawToTexture(pressure, [&] {
            Program *pass = d.sharedClearColorPass();
            pass->use();
            pass->setUniform4f("uColor0", 0, 0, 0, 0);

            return pass;
        });

        d.drawToTexture(offsetUV, [&] {
            Program *pass = d.sharedClearColorPass();
            pass->use();
            pass->setUniform4f("uColor0", 0, 0, 0, 0);
            return pass;
        });

        d.drawToTexture(this->color, [&] {
            Program *pass = initColorPass();
            pass->use();
            pass->setUniformTexture("uTexture0", 0, inTex.textureID);
            return pass;
        });
    }

    void FluidSimulation::updateVelocity(float deltaT) {
        FluidSimulation &d = *this;
        float rho = 1;
        d.drawToTexture(divergence, [&] {
            Program *pass = d.calcDivergencePass();
            pass->use();
            pass->setUniform1f("deltaT", deltaT);
            pass->setUniform1f("rho", rho);
            pass->setUniform1f("epsilon", 1.0 / 600.0);
            pass->setUniformTexture("velocity", 0, velocity->getTextureID());
            return pass;
        });

        for (int i = 0; i < 10; i++) {
            d.drawToTexture(tmpTexture, [&] {
                Program *pass = d.jacobiIterationPass();
                pass->use();
                pass->setUniform1f("epsilon", 1.0 / 600.0);
                pass->setUniformTexture("divergence", 0, divergence->getTextureID());
                pass->setUniformTexture("pressure", 1, pressure->getTextureID());
                return pass;
            });
            std::swap(tmpTexture, pressure);
        }

        d.drawToTexture(tmpTexture, [&] {
            Program *pass = d.subtractPressureGradientPass();
            pass->use();
            pass->setUniform1f("deltaT", deltaT);
            pass->setUniform1f("rho", rho);
            pass->setUniform1f("epsilon", 1.0 / 600.0);
            pass->setUniformTexture("velocity", 0, velocity->getTextureID());
            pass->setUniformTexture("pressure", 1, pressure->getTextureID());
            return pass;
        });
        std::swap(velocity, tmpTexture);
    }

    void FluidSimulation::stepFrameUV(float deltaT, const TextureInfo &outTex) {
        updateVelocity(deltaT);
        drawToTexture(tmpTexture, [&] {
            Program *pass = advectUVPass();
            pass->use();
            pass->setUniform1f("deltaT", deltaT);
            pass->setUniformTexture("offsetUV", 0, offsetUV->getTextureID());
            pass->setUniformTexture("velocity", 1, velocity->getTextureID());
            return pass;
        });
        std::swap(offsetUV, tmpTexture);

        drawToTexture(outTex, [&] {
            Program *pass = passthroughUVPass();
            pass->use();
            pass->setUniformTexture("uTexture0", 0, color->getTextureID());
            pass->setUniformTexture("uOffsetUV", 1, offsetUV->getTextureID());
            return pass;
        });
    }

    void FluidSimulation::stepFrameColor(float deltaT, const TextureInfo &outTex) {
        updateVelocity(deltaT);
        drawToTexture(tmpTexture, [&] {
            Program *pass = advectColorPass();
            pass->use();
            pass->setUniform1f("deltaT", deltaT);
            pass->setUniformTexture("inputTexture", 0, color->getTextureID());
            pass->setUniformTexture("velocity", 1, velocity->getTextureID());
            return pass;
        });
        std::swap(color, tmpTexture);

        drawToTexture(outTex, [&] {
            Program *pass = sharedCopyPass();
            pass->use();
            pass->setUniformTexture("uTexture0", 0, color->getTextureID());
            return pass;
        });
    } // namespace glutils

} // namespace glutils
