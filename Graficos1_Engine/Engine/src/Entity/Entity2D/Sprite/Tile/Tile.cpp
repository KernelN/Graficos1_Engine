#include "Tile.h"

Tile::Tile() 
{
	_id = 0;
	_walkable = false;
}

Tile::Tile(unsigned _imageID, int imgSize[2], int spriteQuantity, int spriteNumber)
			: Sprite(_imageID, imgSize, spriteQuantity, spriteNumber)
{
	_id = 0;
	_walkable = false;	
}

Tile::~Tile() { }

bool Tile::isWalkable() 
{
	if (!_walkable)
		return false;

	else
		return true;
}
void Tile::walkability(bool bWalkable) 
{
	_walkable = bWalkable;
}
unsigned int Tile::getId() 
{
	return _id;
}
void Tile::setId(unsigned int id) 
{
	_id = id;
}
unsigned int Tile::getGid()
{
	return _gid;
}
void Tile::setGid(unsigned int gid)
{
	_gid = gid;
}