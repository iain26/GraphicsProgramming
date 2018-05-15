#include "GUI.h"



GUI::GUI()
{
}

void GUI::Draw(float w, float h, float offset) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, w, h, 0.0, -1.0, 1.0);

	DrawQuad(offset);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void GUI::LoadTex(const std::string& file) {
	Overlay.LoadTextureFile(file);
}

void GUI::DrawQuad(float offset) {
	Overlay.Bind(0);
	glDisable(GL_LIGHTING);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Overlay.GetTexHandler());
	glColor3f(1, 1, 1);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0.05f + offset, -0.95f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(0.15f + offset, -0.95f);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(0.15f + offset, -0.85f);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0.05f + offset, -0.85f);
	glEnd();
}

GUI::~GUI()
{
}
