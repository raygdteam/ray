#pragma once

class Texture
{
public:
    Texture(pcstr path);

    void Bind();
    void Unbind();

private:
    pcstr texturePath;
	GLuint texture;
};