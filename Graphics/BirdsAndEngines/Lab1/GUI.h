#pragma once
#include <string>
#include "Tex.h"

class GUI
{
public:
	GUI();
	~GUI();

	void Draw(float w, float h, float offset);
	void LoadTex(const std::string& file);
	void DrawQuad(float offset);

	Tex Overlay;
};

