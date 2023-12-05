#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include <vector>
#define MAX_FRAMES 50

class Animation
{
public:
	float speed = 1.0f;
	std::vector<SDL_Rect> frames;  // Direct instance of vector, not a pointer
	bool loop = true;
	// Allows the animation to keep going back and forth
	bool pingpong = false;
	/*bool Finished() const;*/
	


	

private:
	float currentFrame = 0.0f;
	int totalFrames = 0;
	int loopCount = 0;
	int pingpongDirection = 1;

public:

	void Animation::PushBack(const SDL_Rect& rect)
	{
		frames.push_back(rect);
	}

	void Reset()
	{
		currentFrame = 0;
	
	}
	
	bool HasFinished()
	{
		return !loop && !pingpong && loopCount > 0;
	}

	void Update()
	{
		currentFrame += speed;
		if (currentFrame >= totalFrames)
		{
			currentFrame = (loop || pingpong) ? 0.0f : totalFrames - 1;
			++loopCount;

			if (pingpong)
				pingpongDirection = -pingpongDirection;
		}
	}

	SDL_Rect& Animation::GetCurrentFrame(float dt)
	{
		currentFrame += speed * dt;
		if (currentFrame >= frames.size())
		{
			currentFrame = (loop) ? 0.0f : frames.size() - 1;
		}
		return frames[(int)currentFrame];
	}

};


#endif