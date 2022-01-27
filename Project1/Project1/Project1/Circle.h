#ifndef CIRCLE_H
#define CIRCLE_H

#include <SDL.h>

struct Vector2f
{
	float x, y;
};

class Circle
{
private:
	int32_t m_radius;
	Vector2f m_position;

public:
	Circle(Vector2f position, int32_t radius);
	void move(float x, float y);
	
	void render(SDL_Renderer* renderer);
};

#endif // !CIRCLE_H

