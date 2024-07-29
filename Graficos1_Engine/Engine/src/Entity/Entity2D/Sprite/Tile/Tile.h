#pragma once
#include "DLLManager.h"
#include "Entity/Entity2D/Sprite/Sprite.h"

class DLLEXPORT Tile : public Sprite
{
public:
	Tile();
	Tile(unsigned int _imageID, int imgSize[2], int spriteQuantity, int spriteNumber);
	~Tile();

	bool isWalkable();
	void walkability(bool bWalkable);

	unsigned int getId();
	void setId(unsigned int id);

	unsigned int getGid();
	void setGid(unsigned int gid);

private:
	unsigned int _gid;
	unsigned int _id;
	bool _walkable;
};