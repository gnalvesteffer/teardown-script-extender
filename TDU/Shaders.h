#pragma once
#include <GL/glew.h>
#include <gl/GL.h>

typedef void*	(*tSetupShader)						(void* pData, void* a2, small_string* shader, small_string* includes, small_string* params);
typedef void	(*tSetShaderSampler2D)				(void* program, const GLchar *name, GLint* value);
typedef void	(*tSetShaderVec)					(GLuint program, GLint location, const GLfloat *value);
typedef void	(*tSetShaderFloat)					(GLuint program, GLint location, float value);
typedef GLint	(*tglGetUniformLocation)			(GLuint program, const GLchar *name);

namespace Teardown
{
	namespace Shaders
	{
		extern tglGetUniformLocation tdglGetUniformLocation;
		extern tSetShaderFloat SetShaderFloat;
		extern tSetShaderVec SetShaderVec4;
		extern tSetShaderVec SetShaderVec3;
		extern tSetShaderVec SetShaderVec2;
		extern tSetupShader SetupShader;
		extern tSetShaderSampler2D SetShaderSampler2D;

		void getFunctionAddresses();
	}
}