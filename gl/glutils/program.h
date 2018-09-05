#ifndef __GLUTILS_PROGRAM_H___
#define __GLUTILS_PROGRAM_H___

#include <string>
#include <map>
#include "./gldefine.h"

namespace glutils {
    class Program final {
    public:
        Program();
        ~Program();

        bool init(const std::string &vsSource, const std::string &fsSource);

        void use();
        void setUniformTexture(const std::string &name, GLint x, GLuint textureID, GLenum target = GL_TEXTURE_2D);
        void setUniform1f(const std::string &name, GLfloat x);
        void setUniform4f(const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
        void setUniform1i(const std::string &name, GLint x);

        void setVertexAttribPointer(const std::string &name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, void *ptr);
        void disableVertexAttrib(const std::string &name);

    private:
        int getHandle(const std::string &name, bool errorNotExist = true);
        bool compileShader(GLuint *shader, GLenum type, const char *source);
        bool linkProgram();

    private:
        std::map<std::string, int> _shaderHandleMap;
        int _programID;
    };
} // namespace glutils

#endif
