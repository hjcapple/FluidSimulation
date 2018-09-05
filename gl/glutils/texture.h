#ifndef __GLUTILS_TEXTURE_H__
#define __GLUTILS_TEXTURE_H__

#include "gldefine.h"

namespace glutils {
    class Texture final {
    public:
        Texture(GLenum target = GL_TEXTURE_2D);
        ~Texture();

        void createFloat16(int width, int height, int filterType = GL_LINEAR, int wrapType = GL_CLAMP_TO_EDGE);
        void create(int width,
                    int height,
                    int format,
                    const void *data,
                    int filterType = GL_LINEAR,
                    int wrapType = GL_CLAMP_TO_EDGE);

        void update(int xoffset, int yoffset, int width, int height, int format, const void *data);

        GLuint getTextureID() const {
            return _textureID;
        }

        int getWidth() const {
            return _width;
        }

        int getHeight() const {
            return _height;
        }

        int getFormat() const {
            return _format;
        }

        GLenum getTarget() const {
            return _target;
        }

    private:
        int _width;
        int _height;
        int _format;
        GLenum _target;
        GLuint _textureID;
    };
} // namespace glutils

#endif
