#pragma once

class Physics
{
public: 

	~Physics();

	void init();

	void update();

	//Adds a brick to the world simulation and returns its physics proxy
	class b2Body* addBrick(int x, int y, int width, int height, bool awake, bool bDynamic);

private:

	class b2World* world = nullptr;
};

