#include "Physics.h"
#include "box2d/box2d.h"
#include "Globals.h"

Physics::~Physics()
{
	delete world;
}

void Physics::init()
{
	b2Vec2 gravity(0.0f, 10.f); // Set gravity (adjust as needed)
	world = new b2World(gravity);
	world->SetAllowSleeping(true);
}

void Physics::update()
{
	world->Step(PHYSICS_TIMESTEP, PHYSICS_VELOCITY_ITERATIONS, PHYSICS_POSITION_ITERATIONS);
}

b2Body* Physics::addBrick(int x, int y, int width, int height, bool awake, bool bDynamic)
{
    // Define body definition
    b2BodyDef bodyDef;
    bodyDef.type = bDynamic ? b2_dynamicBody : b2_staticBody; // Dynamic body for movement
    bodyDef.position.Set(static_cast<float>(x), static_cast<float>(y));

	auto newBody = world->CreateBody(&bodyDef);
    newBody->SetAwake(awake);

    // Define fixture shape (box in this case)
    b2PolygonShape boxShape;
    boxShape.SetAsBox(static_cast<float>(width) *.5f, static_cast<float>(height) * .5f); // Width and height of the box

    // Define fixture properties (material)
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 1.0f; // Density of the material

    // Create the fixture and attach it to the body
    newBody->CreateFixture(&fixtureDef);

    newBody->SetLinearDamping(PHYSICS_BRICK_LINEAR_DAMPING);

    return newBody;
}
