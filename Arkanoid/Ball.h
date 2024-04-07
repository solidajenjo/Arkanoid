#pragma once
class Ball
{
public:

	bool init(struct SDL_Renderer* renderer, class Physics& physics);
	void update(struct SDL_Renderer* renderer, Physics& physics);

private:

	struct SDL_Texture* texture = nullptr;

	class b2Body* ballPhysics = nullptr;
};

