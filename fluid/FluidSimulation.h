#ifndef __GLUTILS_PROCESSOR_H__
#define __GLUTILS_PROCESSOR_H__

#include "GLPlayground.h"
#include "./shaders/shaders.h"

namespace glutils {

    class FluidSimulation : public GLPlayground {
    public:
        FluidSimulation();
        ~FluidSimulation();

        void initOpenGL(const TextureInfo &inTex);
        void stepFrameUV(float dt, const TextureInfo &outTex);
        void stepFrameColor(float dt, const TextureInfo &outTex);

        FluidSimulation &operator=(const FluidSimulation &rhs) = delete;
        FluidSimulation(const FluidSimulation &rhs) = delete;

    private:
        void updateVelocity(float dt);

        Program *initVelocityPass() {
            return this->getSharedProgram(1, init_velocity_fsh);
        }

        Program *initColorPass() {
            return this->getSharedProgram(2, init_color_fsh);
        }

        Program *calcDivergencePass() {
            return this->getSharedProgram(3, divergence_fsh);
        }

        Program *jacobiIterationPass() {
            return this->getSharedProgram(4, jacobi_fsh);
        }

        Program *subtractPressureGradientPass() {
            return this->getSharedProgram(5, pressure_gradient_fsh);
        }

        Program *advectUVPass() {
            return this->getSharedProgram(6, advect_uv_fsh);
        }

        Program *advectColorPass() {
            return this->getSharedProgram(7, advect_color_fsh);
        }

        Program *passthroughUVPass() {
            return this->getSharedProgram(8, passthrough_uv_fsh);
        }

    private:
        Texture *tmpTexture = nullptr;
        Texture *color = nullptr;
        Texture *divergence = nullptr;
        Texture *velocity = nullptr;
        Texture *pressure = nullptr;
        Texture *offsetUV = nullptr;
    };
} // namespace glutils

#endif
