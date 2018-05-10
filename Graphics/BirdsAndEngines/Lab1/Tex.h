#pragma once
#include <string>
#include <GL\glew.h>

class Tex
{
public:
	Tex();

	void LoadTextureFile(const std::string& file);

	std::string GetFileName() { return fileName; };

	void Bind(unsigned int unit); // bind upto 32 textures

	~Tex();

protected:
private:

	std::string fileName;

	GLuint handleTexture;
};

