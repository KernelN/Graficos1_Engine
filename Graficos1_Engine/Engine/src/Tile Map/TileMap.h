#pragma once
#include "DLLManager.h"
#include "Entity/Entity2D/Sprite/Tile/Tile.h"
#include <string>

class DLLEXPORT TileMap 
{
public:
	TileMap();
	~TileMap();

	const Tile& tile(unsigned int uiId);
	void setTile(const Tile& rkTile);
	void setMapTileId(int layer, unsigned int uiCol, unsigned int uiRow, unsigned int uiId);

	void setDimensions(float width, float heigth);
	void setTileDimensions(float tileWidth, float tileHeigth);

	void setTexture(std::string path);
	void Draw();

	bool importTileMap(std::string filePath);
	void checkCollision(Entity2D* object);

	void scaleTiles(float factor);
	void translateTiles(float x, float y);
	Vector2 getConvertedPos(float x, float y);
private:
	std::vector<Tile> tiles;
	std::vector<Tile**> _tileMapGrid;

	int bpp;
	
	int _width;
	int _height;

	float localScale;
	
	float _tileWidth;
	float _tileHeight;
	
	float _scaledTileWidth;
	float _scaledTileHeight;

	unsigned int textureID;
	unsigned int rendererID;

	int _imageWidth;
	int _imageHeight;

	std::string _imagePath;
};