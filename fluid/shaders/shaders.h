#ifndef __GLUTILS_SHADERS_H__
#define __GLUTILS_SHADERS_H__

static const char* pressure_gradient_fsh =
#include "./velocity/pressure_gradient_fsh.glsl"

static const char* jacobi_fsh =
#include "./velocity/jacobi_fsh.glsl"

static const char* divergence_fsh =
#include "./velocity/divergence_fsh.glsl"

static const char* init_velocity_fsh =
#include "./init/init_velocity_fsh.glsl"

static const char* init_color_fsh =
#include "./init/init_color_fsh.glsl"

static const char* advect_uv_fsh =
#include "./uv/advect_uv_fsh.glsl"

static const char* passthrough_uv_fsh =
#include "./uv/passthrough_uv_fsh.glsl"

static const char* advect_color_fsh =
#include "./color/advect_color_fsh.glsl"

#endif
