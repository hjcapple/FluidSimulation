#ifndef __GLUTILS_QUADRENDER_H__
#define __GLUTILS_QUADRENDER_H__

#include "gldefine.h"

namespace glutils {

    class Program;
    class QuadRender final {
    public:
        QuadRender();
        ~QuadRender();

        void draw(Program *program, bool bVFlip = false);

    private:
        typedef struct {
            float x;
            float y;
        } Vec2f;
        
        typedef struct {
            Vec2f position;
            Vec2f texCoord;
        } Vertex;

        GLuint _vertexBuffer;
        GLuint _vertexBufferVFlip;
        GLuint _indexBuffer;
    };
} // namespace glutils

#endif
