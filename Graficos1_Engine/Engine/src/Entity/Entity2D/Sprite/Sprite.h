#pragma once
#include "Entity/Entity2D/Entity2D.h"
#include <string>
#include "DLLManager.h"
#include "Animation/Animation.h"


class DLLEXPORT Sprite : public Entity2D
{
private:
	unsigned int rendererID;
	unsigned int imageID;
	std::string filePath;
	int imgWidth, imgHeight, bitsPerPixel;
	float width, height;
	float vertices[4][4];
	Animation* anim;
	unsigned int spriteQty;

	void ChangeSprite(float leftU, float rightU);
	void Bind();
	void UnBind();

public:
	Sprite();
	Sprite(const std::string& path);
	Sprite(const std::string& path, int spriteQuantity, int spriteNumber);
	Sprite(unsigned int _imageID, int imgSize[2], int spriteQuantity, int spriteNumber);
	~Sprite();

	void SetSprite(unsigned int _imageID, int imgSize[2], float spriteSize[2], float uv[2], float uvSize[2]);
	void ChangeSprite(int spriteQuantity, int spriteNumber);
	void SetAnim(Animation* _anim);
	void UpdateFrame();
	void Draw(bool bind = true);
	unsigned int GetImageID();
	inline int GetSpriteQty() const { return spriteQty; }
	inline int GetImgWidth() const { return imgWidth; }
	inline int GetImgHeight() const { return imgHeight; }
	inline int GetWidth() const { return imgWidth / spriteQty; }
	inline int GetHeight() const { return imgHeight; }
};