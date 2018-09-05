#ifndef __GLUTILS_GLPLAYGROUND_H__
#define __GLUTILS_GLPLAYGROUND_H__

#include <map>
#include "./glutils/program.h"
#include "./glutils/texture.h"

namespace glutils {

    typedef struct {
        int textureID;
        int target;
        int format;
        int width;
        int height;
    } TextureInfo;

    static inline TextureInfo toTextureInfo(Texture *ptr) {
        TextureInfo info;
        info.width = ptr->getWidth();
        info.height = ptr->getHeight();
        info.format = ptr->getFormat();
        info.target = ptr->getTarget();
        info.textureID = ptr->getTextureID();
        return info;
    }

    template <typename T>
    static inline void safeDelete(T *&ptr) {
        if (ptr) {
            delete ptr;
            ptr = nullptr;
        }
    }

    class FrameBuffer;
    class QuadRender;
    class GLPlayground {
    public:
        GLPlayground();
        ~GLPlayground();

        Program *getSharedProgram(int key, const char *vsh, const char *fsh);
        Program *getSharedProgram(int key, const char *fsh);

        void drawTexture(const TextureInfo &inTex, bool isVFlip = false);
        void drawToTexture(const TextureInfo &outTex, bool isVFlip, const std::function<Program *()> &fun);

        void drawToTexture(const TextureInfo &outTex, const std::function<Program *()> &fun) {
            drawToTexture(outTex, false, fun);
        }

        void drawToTexture(Texture *texture, const std::function<Program *()> &fun) {
            drawToTexture(toTextureInfo(texture), false, fun);
        }

        Program *sharedCopyPass();
        Program *sharedClearColorPass();

    private:
        void bindFBO(const TextureInfo &texture);
        QuadRender *sharedQuadRender();
        FrameBuffer *sharedFrameBuffer();

    private:
        std::map<int, Program *> _sharedPrograms;
        QuadRender *_sharedQuardRender;
        FrameBuffer *_sharedFrameBuffer;
    };
} // namespace glutils

#endif
