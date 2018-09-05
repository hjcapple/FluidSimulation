#include "GLPlayground.h"
#include "./glutils/quadrender.h"
#include "./glutils/framebuffer.h"

static const char *passthrough_vsh =
#include "./passthrough_vsh.glsl"

static const char *passthrough_fsh =
#include "./passthrough_fsh.glsl"

static const char* clearcolor_fsh =
#include "./clearcolor_fsh.glsl"

namespace glutils {
    GLPlayground::GLPlayground() {
        _sharedQuardRender = nullptr;
        _sharedFrameBuffer = nullptr;
    }

    Program *GLPlayground::getSharedProgram(int key, const char *vsh, const char *fsh) {
        auto iter = _sharedPrograms.find(key);
        if (iter != _sharedPrograms.end()) {
            return iter->second;
        }
        Program *program = new Program();
        if (program->init(vsh, fsh)) {
            _sharedPrograms[key] = program;
            return program;
        } else {
            assert(false);
            delete program;
            return nullptr;
        }
    }

    Program *GLPlayground::getSharedProgram(int key, const char *fsh) {
        return getSharedProgram(key, passthrough_vsh, fsh);
    }

    GLPlayground::~GLPlayground() {
        for (auto &iter : _sharedPrograms) {
            safeDelete(iter.second);
        }
        safeDelete(_sharedQuardRender);
        safeDelete(_sharedFrameBuffer);
    }

    QuadRender *GLPlayground::sharedQuadRender() {
        if (_sharedQuardRender == nullptr) {
            _sharedQuardRender = new QuadRender();
        }
        return _sharedQuardRender;
    }

    FrameBuffer *GLPlayground::sharedFrameBuffer() {
        if (_sharedFrameBuffer == nullptr) {
            _sharedFrameBuffer = new FrameBuffer();
        }
        return _sharedFrameBuffer;
    }

    Program *GLPlayground::sharedCopyPass() {
        return getSharedProgram(65536, passthrough_vsh, passthrough_fsh);
    }

    Program *GLPlayground::sharedClearColorPass() {
        return getSharedProgram(65537, passthrough_vsh, clearcolor_fsh);
    }

    void GLPlayground::drawTexture(const TextureInfo &inTex, bool isVFlip) {
        GLPlayground &d = *this;
        glDisable(GL_BLEND);
        QuadRender *quadRender = d.sharedQuadRender();
        Program *copyPass = d.sharedCopyPass();
        copyPass->use();
        copyPass->setUniformTexture("uTexture0", 0, inTex.textureID);
        quadRender->draw(copyPass, isVFlip);
    }

    void GLPlayground::drawToTexture(const TextureInfo &outTex, bool isVFlip, const std::function<Program *()> &fun) {
        GLPlayground &d = *this;
        glDisable(GL_BLEND);
        QuadRender *quadRender = d.sharedQuadRender();
        d.bindFBO(outTex);

        Program *pass = fun();

        glViewport(0, 0, outTex.width, outTex.height);
        quadRender->draw(pass, isVFlip);
    }

    void GLPlayground::bindFBO(const TextureInfo &texture) {
        glBindTexture(texture.target, texture.textureID);
        FrameBuffer *frameBuffer = sharedFrameBuffer();
        frameBuffer->attachTexture(texture.target, texture.textureID, GL_COLOR_ATTACHMENT0);
        frameBuffer->bind();
    }
} // namespace glutils
