#pragma once

#include "DLLManager.h"
#include "Data Organizers/Vertex.h"
#include "Data Organizers/Vector2.h"
#include "Data Organizers/Buffer/Buffer.h"

class DLLEXPORT Entity
{
public:
	Entity();
	~Entity();
	void SetTranslation(float x, float y);

	void Translate(float x, float y);
	void Rotate(float angle);
	void Scale(float x, float y);

	Vector2 GetTranslation();
	float GetRotation();
	Vector2 GetScale();

	void UndoTranslation();
	void UndoRotation();
	void UndoScale();

protected:
	void UpdateModel(bool isModelCreated);

	unsigned int* vBuffer;
	unsigned int* iBuffer;
	unsigned int modelID;

	Vector2 lastTranslation;
	float lastRotation;
	Vector2 lastScale;

	Vector2 translation;
	float rotation;
	Vector2 scale;

	
};