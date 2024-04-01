#pragma once

#include "box2d/b2_draw.h"
#include "box2d/b2_body.h"
#include "box2d/b2_world_callbacks.h"
#include <vector>

enum class EntityType
{
	Ball,
	Brick,
	Player,
	KillZone
};

struct PhysicsUserData
{
	EntityType type = EntityType::Ball;
};

class PhysicsContactListener : public b2ContactListener 
{
public:
	void BeginContact(b2Contact* contact) override;

	class Physics* physics = nullptr;
};

class Physics
{
public: 

	~Physics();

	void init();

	void update();

	void debugDraw(struct SDL_Renderer* renderer);

	void onShouldActivateBrick(b2Body* body);
	void onShouldDestroyBrick(b2Body* body);

	//Adds a brick to the world simulation and returns its physics proxy
	class b2Body* addBrick(int x, int y, int width, int height, b2BodyType bodyType, float initialRotation = 0.f);
	class b2Body* addKillZone(int x, int y, int width, int height, b2BodyType bodyType, float initialRotation = 0.f);
	class b2Body* addBall(int x, int y, float radius);

	const std::vector<b2Body*>& getBricks();

	void removeBody(b2Body* body);

private:

	class b2World* world = nullptr;
	
	PhysicsUserData ballUserData;
	PhysicsUserData brickUserData;
	PhysicsUserData playerUserData;
	PhysicsUserData killZoneUserData;

	PhysicsContactListener contactListener;

	std::vector<b2Body*> bricksToEnable;
	std::vector<b2Body*> bricksToDestroy;
	std::vector<b2Body*> bricks;
};

class Box2DDebugDraw : public b2Draw
{

public:

	void SetRenderer(struct SDL_Renderer* renderer);

private:

	/// Draw a closed polygon provided in CCW order.
	virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	/// Draw a solid closed polygon provided in CCW order.
	virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	/// Draw a circle.
	virtual void DrawCircle(const b2Vec2& center, float radius, const b2Color& color);

	/// Draw a solid circle.
	virtual void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color);

	/// Draw a line segment.
	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

	/// Draw a transform. Choose your own length scale.
	/// @param xf a transform.
	virtual void DrawTransform(const b2Transform& xf);

	/// Draw a point.
	virtual void DrawPoint(const b2Vec2& p, float size, const b2Color& color);

	struct SDL_Renderer* renderer = nullptr;

};
