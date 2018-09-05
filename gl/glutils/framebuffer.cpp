#include "framebuffer.h"
#include "logutils.h"

namespace glutils {

    FrameBuffer::FrameBuffer() : _fboId(generateFboId()), _savedFboId(0) {
    }

    FrameBuffer::~FrameBuffer() {
        if (_fboId != 0) {
            glDeleteFramebuffers(1, &_fboId);
            _fboId = 0;
        }
    }

    bool FrameBuffer::bind() {
        if (_fboId == 0) {
            GLUTILS_LOGE("FrameBuffer::bind(), d._fboId == 0");
            return false;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
        return true;
    }

    void FrameBuffer::attachTexture(GLenum texTarget,
                                    GLuint texId,
                                    GLenum attachment /*= GL_COLOR_ATTACHMENT0*/,
                                    int mipLevel /*= 0*/,
                                    int zSlice /*= 0*/) {
        guardedBind();
        framebufferTextureND(attachment, texTarget, texId, mipLevel, zSlice);
        guardedUnbind();
    }

    void FrameBuffer::attachRenderBuffer(GLuint buffId, GLenum attachment /*= GL_COLOR_ATTACHMENT0*/) {
        guardedBind();
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, buffId);
        guardedUnbind();
    }

    void FrameBuffer::unattach(GLenum attachment) {
        guardedBind();
        GLenum type = attachedType(attachment);

        switch (type) {
            case GL_NONE:
                break;

            case GL_RENDERBUFFER:
                attachRenderBuffer(0, attachment);
                break;

            case GL_TEXTURE:
                attachTexture(GL_TEXTURE_2D, 0, attachment);
                break;

            default:
                GLUTILS_LOGE("FrameBuffer::unattach, ERROR: Unknown attached resource type.");
        }

        guardedUnbind();
    }

    GLenum FrameBuffer::attachedType(GLenum attachment) {
        guardedBind();
        GLint type = 0;
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &type);
        guardedUnbind();
        return GLenum(type);
    }

    GLuint FrameBuffer::attachedID(GLenum attachment) {
        guardedBind();
        GLint id = 0;
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &id);
        guardedUnbind();
        return GLuint(id);
    }

    GLint FrameBuffer::attachedMipLevel(GLenum attachment) {
        guardedBind();
        GLint level = 0;
        glGetFramebufferAttachmentParameteriv(
            GL_FRAMEBUFFER, attachment, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL, &level);
        guardedUnbind();
        return level;
    }

    GLint FrameBuffer::attachedCubeFace(GLenum attachment) {
        guardedBind();
        GLint level = 0;
        glGetFramebufferAttachmentParameteriv(
            GL_FRAMEBUFFER, attachment, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE, &level);
        guardedUnbind();
        return level;
    }

    int FrameBuffer::glID() const {
        return _fboId;
    }

    void FrameBuffer::disable() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLuint FrameBuffer::generateFboId() {
        GLuint id = 0;
        glGenFramebuffers(1, &id);
        return id;
    }

    void FrameBuffer::guardedBind() {
        //
        // Only binds if _fboId is different than the currently bound FBO.
        //
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &_savedFboId);

        if (_fboId != _savedFboId) {
            glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
        }
    }

    void FrameBuffer::guardedUnbind() {
        //
        // Returns FBO binding to the previously enabled FBO.
        //
        if (_fboId != _savedFboId) {
            glBindFramebuffer(GL_FRAMEBUFFER, _savedFboId);
        }
    }

    void
    FrameBuffer::framebufferTextureND(GLenum attachment, GLenum texTarget, GLuint texId, int mipLevel, int zSlice) {
        //
        // Default is GL_TEXTURE_2D, GL_TEXTURE_RECTANGLE, or cube faces
        //
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, texTarget, texId, mipLevel);
    }

} // namespace glutils
