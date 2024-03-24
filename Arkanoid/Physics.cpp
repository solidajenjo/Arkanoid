#include "Physics.h"
#include "box2d/box2d.h"
#include "Globals.h"

Physics::~Physics()
{
	delete world;
}

void Physics::init()
{
	b2Vec2 gravity(0.0f, 100.f); // Set gravity (adjust as needed)
	world = new b2World(gravity);
	world->SetAllowSleeping(true);
}

void Physics::update()
{
	world->Step(PHYSICS_TIMESTEP, PHYSICS_VELOCITY_ITERATIONS, PHYSICS_POSITION_ITERATIONS);
}

b2Body* Physics::addBrick(int x, int y, int width, int height)
{
    // Define body definition
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; 
    bodyDef.position.Set(static_cast<float>(x), static_cast<float>(y));

	auto newBody = world->CreateBody(&bodyDef);
    newBody->SetAwake(false);
    newBody->SetGravityScale(0);

    // Define fixture shape (box in this case)
    b2PolygonShape boxShape;
    // Width and height of the box. Slightly reduced to prevent collision detection with nearby bricks
    boxShape.SetAsBox(static_cast<float>(width) *.49f, static_cast<float>(height) * .49f); 

    // Define fixture properties (material)
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 1.0f; // Density of the material

    // Create the fixture and attach it to the body
    newBody->CreateFixture(&fixtureDef);

    newBody->SetLinearDamping(PHYSICS_BRICK_LINEAR_DAMPING);

    return newBody;
}
