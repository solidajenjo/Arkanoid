#pragma once
class Ball
{
public:

	void init(class Physics& physics);
	void update(struct SDL_Renderer* renderer, Physics& physics);

private:

	class b2Body* ballPhysics = nullptr;
};

