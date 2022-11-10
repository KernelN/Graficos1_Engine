#pragma once
#include "Entity/Entity2D/Entity2D.h"
#include <string>
#include "DLLManager.h"


class DLLEXPORT Sprite : public Entity2D
{
private:
	unsigned int rendererID;
	std::string filePath;
	unsigned char* localBuffer;
	int width, height, bitsPerPixel;

public:
	Sprite(const std::string& path);
	Sprite(const std::string& path, int imgSize[2]);
	Sprite(const std::string& path, int imgSize[2], int spriteQuantity, int spriteNumber);
	~Sprite();

	void Draw();
	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
};