#include "Sprite.h"
#include "Utility/RendererSingleton.h"
#include <iostream>

Sprite::Sprite() {}

Sprite::Sprite(const std::string& path)
{
	rendererID = 0;
	filePath = path;
	imgWidth = 0;
	imgHeight = 0;
	bitsPerPixel = 0;
	
	RendererSingleton::GetRenderer()->GetNewTexture(path, &imgWidth, &imgHeight, &bitsPerPixel, &rendererID);

	imageID = rendererID - 1;

	float vertexPos[4][2] =
	{
		{-1, -1},
		{1, -1},
		{1, 1},
		{-1, 1}
	};
	float uvPos[4][2] =
	{
		{0, 0}, //bot left
		{1, 0}, //bot right
		{1, 1}, //top right
		{0, 1}  //top left
	};
	unsigned int indices[6] =
	{
		0, 1, 2,
		2, 3, 0
	};

	for (unsigned short i = 0; i < 4; i++)
	{
		for (unsigned short j = 0; j < 2; j++)
		{
			vertices[i][j] = vertexPos[i][j];
		}
		for (unsigned short j = 2; j < 4; j++)
		{
			vertices[i][j] = uvPos[i][j - 2];
		}
	}

	*vBuffer = RendererSingleton::GetRenderer()->GetNewVertexBuffer(vertices, 4 * (sizeof(float) * 2 + sizeof(float) * 2));
	*iBuffer = RendererSingleton::GetRenderer()->GetNewIndexBuffer(indices, 6);

	Bind();
}

Sprite::Sprite(const std::string& path, int spriteQuantity, int spriteNumber)
{
 	rendererID = 0;
	filePath = path;
	imgWidth = 0;
	imgHeight = 0;
	bitsPerPixel = 0;
	spriteQty = spriteQuantity;

	Renderer* tempRenderer = RendererSingleton::GetRenderer();
	tempRenderer->GetNewTexture(path, &imgWidth, &imgHeight, &bitsPerPixel, &rendererID);
	
	imageID = rendererID - 1;

	Scale(GetWidth(), GetHeight());

	float vertexPos[4][2] =
	{
		{-1, -1},
		{1, -1},
		{1, 1},
		{-1, 1}
	};
	
	float leftX = (float)spriteNumber / spriteQuantity;
	float rightX = (float)(spriteNumber+1) / spriteQuantity;

	float uvPos[4][2] =
	{
		{leftX, 0}, //bot left
		{rightX, 0}, //bot right
		{rightX, 1}, //top right
		{leftX, 1}  //top left
	};
	unsigned int indices[6] =
	{
		0, 1, 2,
		2, 3, 0
	};

	for (unsigned short i = 0; i < 4; i++)
	{
		for (unsigned short j = 0; j < 2; j++)
		{
			vertices[i][j] = vertexPos[i][j];
		}
		for (unsigned short j = 2; j < 4; j++)
		{
			vertices[i][j] = uvPos[i][j - 2];
		}
	}

	*vBuffer = tempRenderer->GetNewVertexBuffer(vertices, 4 * (sizeof(float) * 2 + sizeof(float) * 2));
	*iBuffer = tempRenderer->GetNewIndexBuffer(indices, 6);

	Bind();
}

Sprite::Sprite(unsigned int _imageID, int imgSize[2], int spriteQuantity, int spriteNumber)
{
	rendererID = _imageID+1;
	imageID = _imageID;
	imgWidth = imgSize[0];
	imgHeight = imgSize[1];
	bitsPerPixel = 0;
	spriteQty = spriteQuantity;

	Scale(GetWidth(), GetHeight());

	float vertexPos[4][2] =
	{
		{-1, -1},
		{1, -1},
		{1, 1},
		{-1, 1}
	};
	
	float leftX = (float)spriteNumber / spriteQuantity;
	float rightX = (float)(spriteNumber+1) / spriteQuantity;

	float uvPos[4][2] =
	{
		{leftX, 0}, //bot left
		{rightX, 0}, //bot right
		{rightX, 1}, //top right
		{leftX, 1}  //top left
	};
	unsigned int indices[6] =
	{
		0, 1, 2,
		2, 3, 0
	};

	for (unsigned short i = 0; i < 4; i++)
	{
		for (unsigned short j = 0; j < 2; j++)
		{
			vertices[i][j] = vertexPos[i][j];
		}
		for (unsigned short j = 2; j < 4; j++)
		{
			vertices[i][j] = uvPos[i][j - 2];
		}
	}

	Renderer* tempRenderer = RendererSingleton::GetRenderer();
	*vBuffer = tempRenderer->GetNewVertexBuffer(vertices, 4 * (sizeof(float) * 2 + sizeof(float) * 2));
	*iBuffer = tempRenderer->GetNewIndexBuffer(indices, 6);

	Bind();
}

Sprite::~Sprite()
{
	RendererSingleton::GetRenderer()->DeleteTexture(&rendererID);

	if (anim)
		delete anim;
}

void Sprite::SetSprite(unsigned int _imageID, int imgSize[2], float spriteSize[2], float uv[2], float uvSize[2])
{
	rendererID = _imageID+1;
	imageID = _imageID;
	imgWidth = imgSize[0];
	imgHeight = imgSize[1];
	width = spriteSize[0];
	height = spriteSize[1];

	Scale(width, height);

	float vertexPos[4][2] =
	{
		{-1, -1},
		{1, -1},
		{1, 1},
		{-1, 1}
	};
	float uvPos[4][2] =
	{
		{uv[0], uv[1]}, //bot left
		{uv[0] + uvSize[0], uv[1]}, //bot right
		{uv[0] + uvSize[0], uv[1] + uvSize[1]}, //top right
		{uv[0], uv[1] + uvSize[1]}  //top left
	};
	unsigned int indices[6] =
	{
		0, 1, 2,
		2, 3, 0
	};

	for (unsigned short i = 0; i < 4; i++)
	{
		for (unsigned short j = 0; j < 2; j++)
		{
			vertices[i][j] = vertexPos[i][j];
		}
		for (unsigned short j = 2; j < 4; j++)
		{
			vertices[i][j] = uvPos[i][j - 2];
		}
	}

	*vBuffer = RendererSingleton::GetRenderer()->GetNewVertexBuffer(vertices, 4 * (sizeof(float) * 2 + sizeof(float) * 2));
	*iBuffer = RendererSingleton::GetRenderer()->GetNewIndexBuffer(indices, 6);

	Bind();
}

void Sprite::ChangeSprite(int spriteQuantity, int spriteNumber)
{
	float widthHeightRatio = imgWidth / imgHeight;
	float adjustedX = 0.5f * widthHeightRatio;

	float vertexPos[4][2] =
	{
		{-adjustedX, -1},
		{adjustedX, -1},
		{adjustedX, 1},
		{-adjustedX, 1}
	};

	float leftX = (float)spriteNumber / spriteQuantity;
	float rightX = (float)(spriteNumber + 1) / spriteQuantity;

	float uvPos[4][2] =
	{
		{leftX, 0}, //bot left
		{rightX, 0}, //bot right
		{rightX, 1}, //top right
		{leftX, 1}  //top left
	};

	//float tempVertices[4][4];
	for (unsigned short i = 0; i < 4; i++)
	{
		//for (unsigned short j = 0; j < 2; j++)
		//{
		//	vertices[i][j] = vertices[i][j];
		//}
		for (unsigned short j = 2; j < 4; j++)
		{
			vertices[i][j] = uvPos[i][j - 2];
		}
	}

	//std::cout << "Frame Left:" << uvPos[0][0] << "	Right:" << uvPos[1][0] << std::endl;

	Bind();

	RendererSingleton::GetRenderer()->SetVertexBuffer(*vBuffer, vertices, 4 * (sizeof(float) * 2 + sizeof(float) * 2));
	//Singleton::GetRenderer()->SetNewIndexBuffer(indices, 6);

	UnBind();
}

void Sprite::SetAnim(Animation* _anim)
{
	anim = _anim;
}

void Sprite::UpdateFrame()
{
	//Get old coords
	Vector2 oldCoords = anim->GetCurrentFrame();

	//Update animation timer
	anim->Update();
	
	//Get new coords
	Vector2 uCoords = anim->GetCurrentFrame();

	//If frame didn't change, exit
	if (oldCoords.x == uCoords.x) return;
		
	//std::cout << "Frame Left:" << uCoords.x << "	Right:" << uCoords.y << std::endl;

	//If it's in new frame, update sprite
	ChangeSprite(uCoords.x, uCoords.y);
}

void Sprite::ChangeSprite(float leftU, float rightU)
{
	float uvPos[4][2] =
	{
		{leftU, 0}, //bot left
		{rightU, 0}, //bot right
		{rightU, 1}, //top right
		{leftU, 1}  //top left
	};

	//ONLY CHANGES TEX COORD FROM EACH VERTEX
	for (unsigned short i = 0; i < 4; i++)
	{
		//for (unsigned short j = 0; j < 2; j++)
		//{
		//	vertices[i][j] = vertices[i][j];
		//}
		for (unsigned short j = 2; j < 4; j++)
		{
			vertices[i][j] = uvPos[i][j - 2];
		}
	}

	//std::cout << "Frame Left:" << uvPos[0][0] << "	Right:" << uvPos[1][0] << std::endl;
	
	RendererSingleton::GetRenderer()->SetVertexBuffer(*vBuffer, vertices, 4 * (sizeof(float) * 2 + sizeof(float) * 2));
	
	Bind();

	//UnBind();
}

void Sprite::Bind()
{
	RendererSingleton::GetRenderer()->BindTexture(imageID, rendererID);
	RendererSingleton::GetRenderer()->SetTexture(imageID);
}

void Sprite::UnBind()
{
	RendererSingleton::GetRenderer()->UnbindTexture();
}

void Sprite::Draw(bool bind)
{
	Bind();
	RendererSingleton::GetRenderer()->Draw(*vBuffer, *iBuffer, modelID);
	UnBind();
}

unsigned int Sprite::GetImageID()
{
	return imageID;
}