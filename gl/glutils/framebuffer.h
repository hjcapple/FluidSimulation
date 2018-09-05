#ifndef __GLUTILS_FRAMEBUFFER_H__
#define __GLUTILS_FRAMEBUFFER_H__

#include "gldefine.h"

namespace glutils {

    class FrameBuffer final {
    public:
        FrameBuffer();
        ~FrameBuffer();

        int glID() const;
        bool bind();
        void attachTexture(
            GLenum texTarget, GLuint texId, GLenum attachment = GL_COLOR_ATTACHMENT0, int mipLevel = 0, int zSlice = 0);
        void attachRenderBuffer(GLuint buffId, GLenum attachment = GL_COLOR_ATTACHMENT0);

        //
        // Free any resource bound to the "attachment" point of this FBO
        //
        void unattach(GLenum attachment);

        //
        // Free any resources bound to any attachment points of this FBO
        //
        void unattachAll();

        //
        // Is attached type GL_RENDERBUFFER or GL_TEXTURE?
        //
        GLenum attachedType(GLenum attachment);

        //
        // What is the Id of Renderbuffer/texture currently
        // attached to "attachment?"
        //
        GLuint attachedID(GLenum attachment);

        //
        // Which mipmap level is currently attached to "attachment?"
        //
        GLint attachedMipLevel(GLenum attachment);

        //
        // Which cube face is currently attached to "attachment?"
        //
        GLint attachedCubeFace(GLenum attachment);

        //
        // Disable all FBO rendering and return to traditional,
        // windowing-system controlled framebuffer
        //  NOTE:
        //     This is NOT an "unbind" for this specific FBO, but rather
        //     disables all FBO rendering. This call is intentionally "static"
        //     and named "disable" instead of "unbind" for this reason. The
        //     motivation for this strange semantic is performance. Providing
        //     "Unbind" would likely lead to a large number of unnecessary
        //     FBO enabling/disabling.
        //
        static void disable();

    private:
        GLuint generateFboId();
        void guardedBind();
        void guardedUnbind();
        void framebufferTextureND(GLenum attachment, GLenum texTarget, GLuint texId, int mipLevel, int zSlice);

    private:
        GLuint _fboId;
        GLint _savedFboId;
    };
} // namespace OrangeFilter

#endif
