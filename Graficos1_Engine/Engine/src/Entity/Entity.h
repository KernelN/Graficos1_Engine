#pragma once

#include "Renderer.h"
#include "Data Organizers/Vertex.h"
#include "DLLManager.h"

class DLLEXPORT Entity
{
public:
	Entity();
	~Entity();
protected:
	void funnyTransformationStuff();
	void funnyTransformUpdate();
	Renderer* renderer;
	unsigned int* vBuffer;
	unsigned int* iBuffer;
};