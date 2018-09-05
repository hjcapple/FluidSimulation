#include "program.h"
#include "logutils.h"

namespace glutils {
    Program::Program() : _programID(-1) {
    }

    Program::~Program() {
        if (_programID != -1) {
            glDeleteProgram(_programID);
            _programID = -1;
        }
    }

    bool Program::init(const std::string &vsSource, const std::string &fsSource) {
        GLuint vertShader = 0;
        GLuint fragShader = 0;

        std::string vs = vsSource.c_str();
        std::string fs = fsSource.c_str();

        //
        // Create shader program.
        //
        _programID = glCreateProgram();
        if (0 >= _programID) {
            GLenum ret = glGetError();
            GLUTILS_LOGE("Failed to create program. error id: %d", ret);
            return false;
        }

        //
        // Create and compile vertex shader.
        //
        if (!compileShader(&vertShader, GL_VERTEX_SHADER, vs.c_str())) {
            GLUTILS_LOGE("Failed to compile vertex shader");
            return false;
        }

        //
        // Create and compile fragment shader.
        //
        if (!compileShader(&fragShader, GL_FRAGMENT_SHADER, fs.c_str())) {
            GLUTILS_LOGE("Failed to compile fragment shader");
            return false;
        }

        //
        // Attach vertex shader to program.
        //
        glAttachShader(_programID, vertShader);

        //
        // Attach fragment shader to program.
        //
        glAttachShader(_programID, fragShader);

        //
        // Link program.
        //
        if (!linkProgram()) {
            GLUTILS_LOGE("Failed to link program: %d", _programID);

            glDeleteShader(vertShader);
            vertShader = 0;

            glDeleteShader(fragShader);
            fragShader = 0;

            glDeleteProgram(_programID);
            _programID = 0;

            return true;
        }

        //
        // Release vertex and fragment shaders.
        //
        glDetachShader(_programID, vertShader);
        glDeleteShader(vertShader);

        glDetachShader(_programID, fragShader);
        glDeleteShader(fragShader);

        return true;
    }

    void Program::use() {
        glUseProgram(_programID);
    }

    void Program::setUniformTexture(const std::string &name, GLint x, GLuint textureID, GLenum target) {
        int location = getHandle(name);
        if (-1 != location) {
            glUniform1i(location, x);
            glActiveTexture(GL_TEXTURE0 + x);
            glBindTexture(target, textureID);
        }
    }

    void Program::setUniform1f(const std::string &name, GLfloat x) {
        GLint location = getHandle(name);
        if (-1 != location) {
            glUniform1f(location, x);
        }
    }

    void Program::setUniform1i(const std::string &name, GLint x) {
        GLint location = getHandle(name);
        if (-1 != location) {
            glUniform1i(location, x);
        }
    }

    void Program::setUniform4f(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
        GLint location = getHandle(name);
        if (-1 != location) {
            glUniform4f(location, x, y, z, w);
        }
    }

    void Program::disableVertexAttrib(const std::string &name) {
        GLint index = this->getHandle(name);
        if (-1 != index) {
            glDisableVertexAttribArray(index);
        }
    }

    void Program::setVertexAttribPointer(
        const std::string &name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, void *ptr) {
        GLint index = this->getHandle(name);
        if (-1 != index) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, size, type, normalized, stride, ptr);
        }
    }

    int Program::getHandle(const std::string &name, bool errorNotExist) {
        auto it = _shaderHandleMap.find(name);
        if (it != _shaderHandleMap.end()) {
            return it->second;
        }

        int handle = glGetAttribLocation(_programID, name.c_str());
        if (handle == -1) {
            handle = glGetUniformLocation(_programID, name.c_str());
        }

        if (handle == -1 && errorNotExist) {
            GLUTILS_LOGW("Could not get attrib location for %s", name.c_str());
        } else {
            _shaderHandleMap.insert(std::map<std::string, int>::value_type(name, handle));
        }
        return handle;
    }

    bool Program::compileShader(GLuint *shader, GLenum type, const char *source) {
        const GLchar *sources[] = {source};

        *shader = glCreateShader(type);
        glShaderSource(*shader, sizeof(sources) / sizeof(*sources), sources, NULL);
        glCompileShader(*shader);

        GLint logLength;
        glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            char *log = (char *)malloc(logLength);
            glGetShaderInfoLog(*shader, logLength, &logLength, log);
            GLUTILS_LOGW("Shader compile log:%s", log);
            free(log);
        }

        GLint status;
        glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
        if (status == 0) {
            glDeleteShader(*shader);
            *shader = 0;
            return false;
        }

        return true;
    }

    bool Program::linkProgram() {
        GLint status;
        glLinkProgram(_programID);

        GLint logLength;
        glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            char *log = (char *)malloc(logLength);
            glGetProgramInfoLog(_programID, logLength, &logLength, log);
            GLUTILS_LOGD("Program link log:%s", log);
            free(log);
        }

        glGetProgramiv(_programID, GL_LINK_STATUS, &status);
        if (status == 0) {
            return false;
        }
        return true;
    }

} // namespace glutils
