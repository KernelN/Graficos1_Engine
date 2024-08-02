#pragma once
#include<vector>
#include "DLLManager.h"
#include "Data Organizers/Vector2.h"

class DLLEXPORT Animation
{
public:
	Animation(float animLength, unsigned int framesQty);
	Animation(float animLength, unsigned int framesQty, unsigned int firstFrame, unsigned int lastFrame);
	Animation(float animLength, unsigned int framesQty,unsigned int firstFrame, unsigned int lastFrame, unsigned int framesRow);
	~Animation();
	void Update();
	void AddFrame(Vector2 _uCoords);
	void SetDuration(float _length);
	Vector2 GetCurrentFrame();
	Vector2 GetFrame(int frame);
	int GetRow() { return row; };
private:
	std::vector<Vector2> uCoords; //use only left U and right U
	float timer;
	float length;
	int currentFrame;
	int row;
};