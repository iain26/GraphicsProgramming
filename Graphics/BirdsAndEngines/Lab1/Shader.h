#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <intrin.h>
#include <GL\glew.h>
#include "transform.h"
#include "Cam.h"

class Shader
{
public:
	Shader();
	void InitialiseShader(const std::string& file);
	void LoadThreeShaders(const std::string& file);
	void Bind();
	GLuint ReturnProgram() {
		return ShaderProgram;
	}
	void UpdateShader(const Transform& transform, const Cam& camera);

	std::string Shader::ShaderLoad(const std::string& file);
	void Shader::ShaderErrorHandling(GLuint shader, GLuint flag, bool isProgram, const std::string& message);
	GLuint Shader::ShaderCreation(const std::string& text, unsigned int type);

	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ShaderProgram, name.c_str()), (int)value);

		if ((glGetUniformLocation(ShaderProgram, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ShaderProgram, name.c_str()), value);

		if ((glGetUniformLocation(ShaderProgram, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ShaderProgram, name.c_str()), value);

		if ((glGetUniformLocation(ShaderProgram, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string &name, const glm::vec2 &value) const
	{
		glUniform2fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, &value[0]);

		if ((glGetUniformLocation(ShaderProgram, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	void setVec2(const std::string &name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ShaderProgram, name.c_str()), x, y);

		if ((glGetUniformLocation(ShaderProgram, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string &name, const glm::vec3 &value) const
	{
		glUniform3fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, &value[0]);
		if ((glGetUniformLocation(ShaderProgram, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}

	}
	void setVec3(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ShaderProgram, name.c_str()), x, y, z);

		if ((glGetUniformLocation(ShaderProgram, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string &name, const glm::vec4 &value) const
	{
		glUniform4fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, &value[0]);

		if ((glGetUniformLocation(ShaderProgram, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	void setVec4(const std::string &name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(ShaderProgram, name.c_str()), x, y, z, w);

		if ((glGetUniformLocation(ShaderProgram, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string &name, const glm::mat2 &mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);

		if ((glGetUniformLocation(ShaderProgram, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string &name, const glm::mat3 &mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);

		if ((glGetUniformLocation(ShaderProgram, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string &name, const glm::mat4 &mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);

		if ((glGetUniformLocation(ShaderProgram, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;

			__debugbreak();
		}
	}


    ~Shader();


protected:
private:
	static const unsigned int NumShade = 3; 

	enum
	{
		TransformUnif,
		NumUnifs
	};

	GLuint ShaderProgram;
	GLuint shaders[NumShade];
	GLuint uniforms[NumUnifs];
};
