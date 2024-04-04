#pragma once
class Player
{
public:	

	bool init(struct SDL_Renderer* renderer, class Physics& physics);
	void draw(struct SDL_Renderer* renderer);
	void update(int xDir);

private:

	struct SDL_Texture* texture = nullptr;

	class b2Body* playerPhysics = nullptr;

};

