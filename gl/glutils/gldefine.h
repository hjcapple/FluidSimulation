#ifndef __GLUTILS_GLDEFINE_H__
#define __GLUTILS_GLDEFINE_H__

#ifdef FLOWIMAGE_ANDROID

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#else

#include "TargetConditionals.h"
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>

#endif

#endif
