#include "texture.h"

namespace glutils {
    Texture::Texture(GLenum target) : _target(target), _width(0), _height(0), _format(GL_RGBA), _textureID(0) {
    }

    Texture::~Texture() {
        if (_textureID != 0) {
            glDeleteTextures(1, &_textureID);
            _textureID = 0;
        }
    }

    void Texture::createFloat16(int width, int height, int filterType, int wrapType) {
        GLuint textureHandles[1] = {0};
        glGenTextures(1, textureHandles);
        _textureID = textureHandles[0];

        glBindTexture(_target, _textureID);
        glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, filterType);
        glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, filterType);
        glTexParameteri(_target, GL_TEXTURE_WRAP_S, wrapType);
        glTexParameteri(_target, GL_TEXTURE_WRAP_T, wrapType);
        glTexStorage2D(_target, 1, GL_RGBA16F, width, height);

        _width = width;
        _height = height;
        _format = GL_RGBA;
    }

    void Texture::create(int width, int height, int format, const void *data, int filterType, int wrapType) {
        GLuint textureHandles[1] = {0};
        glGenTextures(1, textureHandles);
        _textureID = textureHandles[0];

        glBindTexture(_target, _textureID);

        glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, filterType);
        glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, filterType);
        glTexParameteri(_target, GL_TEXTURE_WRAP_S, wrapType);
        glTexParameteri(_target, GL_TEXTURE_WRAP_T, wrapType);

        glPixelStorei(GL_PACK_ALIGNMENT, 4);
        glTexImage2D(_target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        _width = width;
        _height = height;
        _format = format;
    }

    void Texture::update(int xoffset, int yoffset, int width, int height, int format, const void *data) {
        glPixelStorei(GL_PACK_ALIGNMENT, 4);
        glBindTexture(_target, _textureID);
        glTexSubImage2D(_target, 0, xoffset, yoffset, width, height, format, GL_UNSIGNED_BYTE, data);
    }
} // namespace glutils
