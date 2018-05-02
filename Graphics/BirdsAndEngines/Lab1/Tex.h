#pragma once
#include <string>
#include <GL\glew.h>

class Tex
{
public:
	Tex();

	void LoadTextureFile(const std::string& file);

	void Bind(unsigned int unit); // bind upto 32 textures

	~Tex();

protected:
private:

	GLuint handleTexture;
};

