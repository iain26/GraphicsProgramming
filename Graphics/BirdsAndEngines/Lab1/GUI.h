#pragma once
#include <string>
#include "Tex.h"

class GUI
{
public:
	GUI();
	~GUI();

	void Draw(float w, float h);
	void LoadTex(const std::string& file);
	void DrawQuad();

	Tex Overlay;
};

