#include "TileMap.h"
#include "Utility/XML/tinyxml2.h"
#include "Utility/RendererSingleton.h"
#include "CollisionManager.h"
#include <iostream>

TileMap::TileMap()
{
	bpp = -1;

	_width = -1;
	_height = -1;

	localScale = 1;

	_tileWidth = -1;
	_tileHeight = -1;

	textureID = 0;
	rendererID = 0;

	_imageWidth = -1;
	_imageHeight = -1;
}

TileMap::~TileMap() {}

const Tile& TileMap::tile(unsigned int uiId) {
	Tile* NoTile = nullptr;

	for (int i = 0; i < tiles.size(); i++) {
		if (uiId == tiles[i].getId()) {
			return tiles[i];
		}
	}

	return *NoTile;
}

void TileMap::setMapTileId(int layer, unsigned int uiCol, unsigned int uiRow, unsigned int uiId) {
	_tileMapGrid[layer][uiCol][uiRow] = tile(uiId);
}


void TileMap::setTile(const Tile& rkTile)
{
	tiles.push_back(rkTile);
}

void TileMap::setTileDimensions(float tileWidth, float tileHeight) {
	_tileWidth = tileWidth;
	_tileHeight = tileHeight;
}

void TileMap::setDimensions(float width, float height) {
	_width = width;
	_height = height;

	//creo la grilla bidimensional para guardar la posicion de cada tile igual que en el editor
	Tile** tileMap;
	tileMap = new Tile * [_height];
	for (int i = 0; i < _height; i++) {
		tileMap[i] = new Tile[_width];
	}
	_tileMapGrid.push_back(tileMap);
}

void TileMap::setTexture(std::string path) {
	RendererSingleton::GetRenderer()->GetNewTexture(path, &_imageWidth, &_imageHeight, &bpp, &rendererID);
	textureID = rendererID - 1;
}

void TileMap::Draw() {
	Renderer* tempRenderer = RendererSingleton::GetRenderer();
	tempRenderer->BindTexture(textureID, rendererID);
	tempRenderer->SetTexture(textureID);
	//rkRenderer.setCurrentTexture(textureID);

	for (int i = 0; i < _tileMapGrid.size(); i++)
	{
		for (int y = 0; y < _height; y++)
		{
			for (int x = 0; x < _width; x++)
			{
				if (_tileMapGrid[i][y][x].getId() == NULL) continue;
				_tileMapGrid[i][y][x].Draw(false);
			}
		}
	}

	tempRenderer->UnbindTexture();
}

bool TileMap::importTileMap(std::string filePath) {
	tinyxml2::XMLDocument tilemap; //guarda el documento
	tinyxml2::XMLError errorHandler; //guarda el resultado de las funciones

	errorHandler = tilemap.LoadFile(filePath.c_str()); //carga el archivo XML
	if (errorHandler == tinyxml2::XML_ERROR_FILE_NOT_FOUND || errorHandler == tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED) return false;

	// Loading Map element and save Map width, heigth in tiles and width, heigth of Tiles in pixels
	tinyxml2::XMLElement* mapNode = tilemap.FirstChildElement("map");
	if (mapNode == nullptr)
		return false;

	setDimensions(mapNode->FloatAttribute("width"), mapNode->FloatAttribute("height"));				// Get width and heigth for
	setTileDimensions(mapNode->FloatAttribute("tilewidth"), mapNode->FloatAttribute("tileheight")); // the map and the tiles

	// Loading Tileset element
	tinyxml2::XMLDocument tilesheet; //guarda el documento source

	std::string tilesetPath = "res/tilemap/";
	tilesetPath += mapNode->FirstChildElement("tileset")->Attribute("source");

	errorHandler = tilesheet.LoadFile(tilesetPath.c_str()); //carga el archivo XML
	if (errorHandler == tinyxml2::XML_ERROR_FILE_NOT_FOUND || errorHandler == tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED) return false;

	tinyxml2::XMLElement* pTilesheet = tilesheet.FirstChildElement("tileset");
	if (pTilesheet == NULL)
		return false;

	_imagePath = "res/tilemap/";
	_imagePath += pTilesheet->FirstChildElement("image")->Attribute("source");			// Loading Textures
	setTexture(_imagePath); //

	// Save the Tiles in the TileMap  and set it's position
	float topLeft[2] =
	{ -(_tileWidth * _width) / 2.0f,
		(_tileHeight * _height) / 2.0f };
	float tileX = 0.0f, tileY = 0.0f;
	int _id = 0;
	for (int i = 0; i < _height; i++) {
		for (int j = 0; j < _width; j++) {
			Tile* newTile = new Tile();

			newTile->setId(_id);

			newTile->Translate(topLeft[0] + tileX, topLeft[1] - tileY);
			setTile(*newTile);
			_id++;
			tileX += _tileWidth;
		}
		tileX = 0;
		tileY += _tileHeight;
	}

	// Loading Layer element
	tinyxml2::XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if (pLayer == NULL)
		return false;

	// Loading Tileset element
	tinyxml2::XMLElement* pTilesetData = pLayer->FirstChildElement("data");
	if (pTilesetData == NULL)
		return false;

	tinyxml2::XMLElement* pTileData;

	unsigned int tileID = 0;
	int sheetTileCount = pTilesheet->IntAttribute("tilecount");
	int sheetImgSize[2] = { _imageWidth, _imageHeight };
	float tileSpriteSize[2] = { _tileWidth, _tileHeight };
	int sheetColumns = pTilesheet->IntAttribute("columns");
	float sheetRows = sheetTileCount / (float)sheetColumns;
	float tileUVSize[2] = { 1.0f / sheetColumns, 1.0f / sheetRows };
	int tileCount = pTilesheet->IntAttribute("tilecount");

	int layerCount = 0;
	while (pLayer) {
		// Loading Data element
		pTilesetData = pLayer->FirstChildElement("data");
		if (pTilesetData == NULL)
			return false;

		if (layerCount > 0) {
			Tile** tileMap;
			tileMap = new Tile * [_height];
			for (int i = 0; i < _height; i++) {
				tileMap[i] = new Tile[_width];
			}
			_tileMapGrid.push_back(tileMap);
		}


		tinyxml2::XMLElement* pTile = pTilesetData->FirstChildElement("tile");
		while (pTile)
		{
			pTileData = pTilesheet->FirstChildElement("tile");
			unsigned int gid = pTile->IntAttribute("gid") - 1; //somewhy gids are offset by 1

			int tileGetTries = 0;
			while (pTile && gid > tileCount && tileGetTries <= _width * _height)
			{
				pTile->NextSiblingElement("tile");
				gid = pTile->IntAttribute("gid") - 1;
				tileGetTries++;
			}

			if (gid > tileCount) break;

			unsigned int i = 0;
			while (pTileData && i <= gid)
			{
				i = pTileData->IntAttribute("id");
				if (i == gid)
				{
					tinyxml2::XMLElement* pProperty = pTileData->FirstChildElement("properties")->FirstChildElement("property");
					std::string propertyName = pProperty->Attribute("value");

					//Set gid
					tiles[tileID].setGid(gid);

					//set walkable
					tiles[tileID].walkability(propertyName != "false");

					//set sprite
					//tengo que:
					//gettear imgSize
					//gettear spriteSize (tileWidth y tileHeight)
					//gettear gid
					//ver donde esta en X e Y
					//(lease, dividir gid por tilesheet columns para Y y restarle columns...
					//(por alguna razon lee de abajo para arriba)
					int y = sheetColumns - std::ceil(gid / sheetColumns); //round up
					//... y modulo gid por columns para X)
					int x = gid % sheetColumns;
					//calcular uv left bot (x/columnas, y/filas)
					float uv[2] = { x / (float)sheetColumns, y / sheetRows };
					tiles[tileID].SetSprite(textureID, sheetImgSize,
						tileSpriteSize, uv, tileUVSize);
					break;
				}
				else
				{
					pTileData = pTileData->NextSiblingElement("tile");
					continue;
				}
			}

			pTile = pTile->NextSiblingElement("tile");
			tileID++;
		}

		while (pTilesetData)
		{
			int id = 0; //Don't forget, tile 0 is top left, last one is bottom right
			for (int x = 0; x < _width; x++)
			{
				for (int y = 0; y < _height; y++)
				{
					setMapTileId(layerCount, x, y, id);
					id++;
				}
			}

			pTilesetData = pTilesetData->NextSiblingElement("data");
		}

		layerCount++;
		pLayer = pLayer->NextSiblingElement("layer");
	}

	return true;
}

void TileMap::checkCollision(Entity2D* object) {

	Vector2 convertedPos = getConvertedPos(object->GetTranslation().x, object->GetTranslation().y);

	int left_tile = (convertedPos.x - object->GetScale().x) / _scaledTileWidth;
	int right_tile = (convertedPos.x + object->GetScale().x) / _scaledTileWidth;

	int top_tile = (convertedPos.y - object->GetScale().y) / _scaledTileHeight;
	int bottom_tile = (convertedPos.y + object->GetScale().y) / _scaledTileHeight;

	if (left_tile < 0)
	{
		//left_tile = 0;
		object->UndoTranslation();
		return;
	}

	if (right_tile >= _width)
	{
		//right_tile = _width - 1;
		object->UndoTranslation();
		return;
	}

	if (top_tile < 0)
	{
		//top_tile = 0;
		object->UndoTranslation();
		return;
	}

	if (bottom_tile >= _height)
	{
		//bottom_tile = _height - 1;
		object->UndoTranslation();
		return;
	}

	/*
	cout << "converted X: " << convertedPosX << endl;
	cout << "converted Y: " << convertedPosY << endl;

	cout << "left: " <<left_tile << endl;
	cout << "right: "<<right_tile << endl;
	cout << "top: " << top_tile << endl;
	cout << "bottom: "<<bottom_tile << endl;
	*/

	bool tileColliding;
	Tile* tile;

	//tile 0 is top left, last one is bottom right, never forgetti
	for (int i = left_tile; i <= right_tile; i++)
	{
		for (int j = top_tile; j <= bottom_tile; j++)
		{
			for (int k = 0; k < _tileMapGrid.size(); k++)
			{
				//cout << "caminable " << "[" << k << "]" << "[" << j << "]" << "[" << i << "] : "<< _tileMapGrid[k][j][i].isWalkable() << endl; // true == 1  ; false == 0
				//cout << true << endl;
				tile = &_tileMapGrid[k][j][i];

				if (tile == nullptr)
					continue;
				if (tile->isWalkable()) continue;

				tileColliding = CollisionManager::CheckCollision(tile, object);

				if (!tileColliding) continue;

				object->UndoTranslation();
			}
		}
	}
}

void TileMap::scaleTiles(float factor)
{
	localScale *= factor;

	float scaledTileWidth = _tileWidth * localScale;
	float scaledTileHeight = _tileHeight * localScale;
	float topLeft[2] = { -(scaledTileWidth * _width) / 2.0f,
								(scaledTileHeight * _height) / 2.0f };
	float tileX = 0.0f, tileY = 0.0f;

	for (int i = 0; i < _tileMapGrid.size(); i++)
	{
		for (int y = 0; y < _height; y++)
		{
			for (int x = 0; x < _width; x++)
			{
				_tileMapGrid[i][y][x].SetTranslation(topLeft[0] + tileX,
					topLeft[1] - tileY);
				_tileMapGrid[i][y][x].Scale(factor, factor);
				tileX += scaledTileWidth;
			}
			tileX = 0;
			tileY += scaledTileHeight;
		}
		tileY = 0;
	}

	//Update real distance in X Y between tiles
	Vector2 tile1Pos = _tileMapGrid[0][0][0].GetTranslation();
	Vector2 tile2Pos = _tileMapGrid[0][1][1].GetTranslation();
	_scaledTileWidth = tile1Pos.x - tile2Pos.x;
	_scaledTileHeight = tile1Pos.y - tile2Pos.y;

	if (_scaledTileWidth < 0) _scaledTileWidth *= -1;
	if (_scaledTileHeight < 0) _scaledTileHeight *= -1;
}

void TileMap::translateTiles(float x, float y)
{
	for (int i = 0; i < _tileMapGrid.size(); i++) {
		for (int j = 0; j < _height; j++) {
			for (int k = 0; k < _width; k++) {
				_tileMapGrid[i][j][k].Translate(x, y);
			}
		}
	}
}

Vector2 TileMap::getConvertedPos(float x, float y)
{
	//Reminder, tile 0 is top left, last one is bottom right
	Vector2 pos;

	//0,0 is top left, so offset pos by half grid
	pos.x = (_width / 2.0f) * _scaledTileWidth + x;
	pos.y = (_height / 2.0f) * _scaledTileHeight - y; //y is inverted in the tilemap
	return pos;
}