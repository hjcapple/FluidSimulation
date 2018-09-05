#include "quadrender.h"
#include "program.h"

namespace glutils {

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

    //////////////////////////////////////////////////////////////////////////
    // Class QuadRender.
    //////////////////////////////////////////////////////////////////////////
    QuadRender::QuadRender() : _vertexBuffer(0), _vertexBufferVFlip(0), _indexBuffer(0) {

        QuadRender::Vertex vertices[4];

        //
        // 0 bottom left.
        //
        vertices[0].position.x = -1.0f;
        vertices[0].position.y = -1.0f;
        vertices[0].texCoord.x = 0.0f;
        vertices[0].texCoord.y = 0.0f;

        //
        // 1 bottom right.
        //
        vertices[1].position.x = 1.0f;
        vertices[1].position.y = -1.0f;
        vertices[1].texCoord.x = 1.0f;
        vertices[1].texCoord.y = 0.0f;

        //
        // 2 top left.
        //
        vertices[2].position.x = -1.0f;
        vertices[2].position.y = 1.0f;
        vertices[2].texCoord.x = 0.0f;
        vertices[2].texCoord.y = 1.0f;

        //
        // 3 top right.
        //
        vertices[3].position.x = 1.0f;
        vertices[3].position.y = 1.0f;
        vertices[3].texCoord.x = 1.0f;
        vertices[3].texCoord.y = 1.0f;

        QuadRender::Vertex verticesFliped[4];

        //
        // 0 bottom left.
        //
        verticesFliped[0].position.x = -1.0f;
        verticesFliped[0].position.y = -1.0f;
        verticesFliped[0].texCoord.x = 0.0f;
        verticesFliped[0].texCoord.y = 1.0f;

        //
        // 1 bottom right.
        //
        verticesFliped[1].position.x = 1.0f;
        verticesFliped[1].position.y = -1.0f;
        verticesFliped[1].texCoord.x = 1.0f;
        verticesFliped[1].texCoord.y = 1.0f;

        //
        // 2 top left.
        //
        verticesFliped[2].position.x = -1.0f;
        verticesFliped[2].position.y = 1.0f;
        verticesFliped[2].texCoord.x = 0.0f;
        verticesFliped[2].texCoord.y = 0.0f;

        //
        // 3 top right.
        //
        verticesFliped[3].position.x = 1.0f;
        verticesFliped[3].position.y = 1.0f;
        verticesFliped[3].texCoord.x = 1.0f;
        verticesFliped[3].texCoord.y = 0.0f;

        glGenBuffers(1, &_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &_vertexBufferVFlip);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferVFlip);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesFliped), verticesFliped, GL_STATIC_DRAW);

        GLushort indices[] = {0, 1, 2, 3};
        glGenBuffers(1, &_indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    QuadRender::~QuadRender() {
        if (_vertexBuffer != 0) {
            glDeleteBuffers(1, &_vertexBuffer);
            _vertexBuffer = 0;
        }

        if (_vertexBufferVFlip != 0) {
            glDeleteBuffers(1, &_vertexBufferVFlip);
            _vertexBufferVFlip = 0;
        }

        if (_indexBuffer != 0) {
            glDeleteBuffers(1, &_indexBuffer);
            _indexBuffer = 0;
        }
    }

    void QuadRender::draw(Program *program, bool bVFlip /*= false*/) {
        glBindBuffer(GL_ARRAY_BUFFER, bVFlip ? _vertexBufferVFlip : _vertexBuffer);
        program->setVertexAttribPointer("aPosition", 2, GL_FLOAT, GL_FALSE, sizeof(QuadRender::Vertex), 0);
        program->setVertexAttribPointer(
            "aTextureCoord", 2, GL_FLOAT, GL_FALSE, sizeof(QuadRender::Vertex), BUFFER_OFFSET(sizeof(float) * 2));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
        program->disableVertexAttrib("aPosition");
        program->disableVertexAttrib("aTextureCoord");

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
