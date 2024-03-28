#include "Physics.h"
#include "box2d/box2d.h"
#include "Globals.h"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <cmath>

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

void Physics::debugDraw(SDL_Renderer* renderer)
{
    Box2DDebugDraw debugDraw;
    debugDraw.SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_centerOfMassBit);
    debugDraw.SetRenderer(renderer);
    world->SetDebugDraw(&debugDraw);
    world->DebugDraw();
}

b2Body* Physics::addBrick(int x, int y, int width, int height, bool bIsStatic, float initialRotation)
{
    // Define body definition
    b2BodyDef bodyDef;
    bodyDef.type = bIsStatic ? b2_staticBody : b2_dynamicBody;

    bodyDef.position.Set(static_cast<float>(x), static_cast<float>(y));
    bodyDef.angle = initialRotation;

	auto newBody = world->CreateBody(&bodyDef);
    newBody->SetAwake(false);

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
    newBody->SetAngularDamping(PHYSICS_BRICK_ANGULAR_DAMPING);    

    return newBody;
}

b2Body* Physics::addBall(int x, int y, float radius)
{
    // Define body definition
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(static_cast<float>(x), static_cast<float>(y));

    auto newBody = world->CreateBody(&bodyDef);
    newBody->SetAwake(false);

    b2CircleShape circleShape;
    
    circleShape.m_radius = radius;

    // Define fixture properties (material)
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 10.f; // Density of the material
    fixtureDef.restitution = 2.f;

    // Create the fixture and attach it to the body
    newBody->CreateFixture(&fixtureDef);

    newBody->SetLinearDamping(0);
    newBody->SetLinearVelocity({ static_cast<float>((rand() % 10) - 5) , static_cast<float>((rand() % 10) - 5) });
    newBody->SetGravityScale(0);    

    return newBody;
}

void Box2DDebugDraw::SetRenderer(SDL_Renderer* renderer)
{
    this->renderer = renderer;
}

void Box2DDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    SDL_Point sdlPoints[8]; //NOTE:Max vertices per poly 8 increase this if needed

    for (int i = 0; i < vertexCount && i < 8; ++i)
    {
        sdlPoints[i].x = PHYSICS_TO_PIXEL(vertices[i].x);
        sdlPoints[i].y = PHYSICS_TO_PIXEL(vertices[i].y);
    }
    // Set the render color (example: blue)
    SDL_SetRenderDrawColor(renderer, color.r * 255, color.g * 255, color.b * 255, color.a * 255);

    SDL_RenderDrawLines(renderer, sdlPoints, vertexCount);
}

void Box2DDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    DrawPolygon(vertices, vertexCount, color);
}

void Box2DDebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
    //IMPLEMENT IF NEEDED
}

void Box2DDebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
    //IMPLEMENT IF NEEDED
}

void Box2DDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    //IMPLEMENT IF NEEDED
}

void Box2DDebugDraw::DrawTransform(const b2Transform& xf)
{
    //IMPLEMENT IF NEEDED
}

void Box2DDebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
    //IMPLEMENT IF NEEDED
}
