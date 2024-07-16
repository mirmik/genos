#ifndef RABBIT_OPENGL_OPENGL_H_
#define RABBIT_OPENGL_OPENGL_H_

#if __has_include(<GL/glew.h>)
#include <GL/glew.h>
#else
#if __has_include(<EGL/egl.h>)
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#endif
#endif

#endif