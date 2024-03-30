#include "Physics.h"
#include "box2d/box2d.h"
#include "Globals.h"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <cmath>
#include <iostream>

Physics::~Physics()
{
	delete world;
}

void Physics::init()
{
	b2Vec2 gravity(0.0f, 10.f); // Set gravity (adjust as needed)
	world = new b2World(gravity);
	world->SetAllowSleeping(true);
    world->SetContactListener(&contactListener);
    contactListener.physics = this;

    ballUserData.type = EntityType::Ball;
    brickUserData.type = EntityType::Brick;
    playerUserData.type = EntityType::Player;

}

void Physics::update()
{
    world->Step(PHYSICS_TIMESTEP, PHYSICS_VELOCITY_ITERATIONS, PHYSICS_POSITION_ITERATIONS);

    if (!bricksToEnable.empty())
    {
        for (auto brick : bricksToEnable)
        {
            std::vector<b2Body*>::iterator it = std::remove(bricks.begin(), bricks.end(), brick);
            bricks.erase(it);
            auto pos = brick->GetPosition();
            auto newBrick = addBrick(pos.x, pos.y, PIXEL_TO_PHYSICS(BLOCK_WIDTH), PIXEL_TO_PHYSICS(BLOCK_HEIGHT), b2_dynamicBody);
            newBrick->SetAwake(true);
            removeBody(brick);
        }
        bricksToEnable.clear();
    }
}

void Physics::debugDraw(SDL_Renderer* renderer)
{
    Box2DDebugDraw debugDraw;
    debugDraw.SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_centerOfMassBit);
    debugDraw.SetRenderer(renderer);
    world->SetDebugDraw(&debugDraw);
    world->DebugDraw();
}

void Physics::onShouldActivateBrick(b2Body* body)
{
    bricksToEnable.push_back(body);
}

b2Body* Physics::addBrick(int x, int y, int width, int height, b2BodyType bodyType, float initialRotation)
{
    // Define body definition
    b2BodyDef bodyDef;
    bodyDef.type = bodyType;

    bodyDef.position.Set(static_cast<float>(x), static_cast<float>(y));
    bodyDef.angle = initialRotation;

	auto newBody = world->CreateBody(&bodyDef);
    newBody->SetAwake(false);

    // Define fixture shape (box in this case)
    b2PolygonShape boxShape;
    // Width and height of the box. Slightly reduced to prevent collision detection with nearby bricks
    boxShape.SetAsBox(static_cast<float>(width) *.49f, static_cast<float>(height) * .5f);
    
    // Define fixture properties (material)
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = BRICK_DENSITY;

    // Create the fixture and attach it to the body
    newBody->CreateFixture(&fixtureDef);

    newBody->SetLinearDamping(PHYSICS_BRICK_LINEAR_DAMPING);    
    newBody->SetAngularDamping(PHYSICS_BRICK_ANGULAR_DAMPING);    
    newBody->SetUserData(&brickUserData);
    if (bodyType != b2_staticBody) //ignore static margins
    {
        bricks.push_back(newBody);
    }
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
    fixtureDef.density = BALL_DENSITY;
    fixtureDef.restitution = BALL_RESTITUTION;

    // Create the fixture and attach it to the body
    newBody->CreateFixture(&fixtureDef);

    newBody->SetLinearDamping(0);
    newBody->SetLinearVelocity({ static_cast<float>((rand() % 10) - 5) , static_cast<float>((rand() % 10) - 5) });
    newBody->SetGravityScale(0);    

    newBody->SetUserData(&ballUserData);

    return newBody;
}

const std::vector<b2Body*>& Physics::getBricks()
{
    return bricks;
}

void Physics::removeBody(b2Body* body)
{
    world->DestroyBody(body);
}

void Box2DDebugDraw::SetRenderer(SDL_Renderer* renderer)
{
    this->renderer = renderer;
}

void Box2DDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    SDL_Point sdlPoints[36]; //NOTE:Max vertices per poly 36 increase this if needed

    for (int i = 0; i < vertexCount && i < vertexCount; ++i)
    {
        sdlPoints[i].x = PHYSICS_TO_PIXEL(vertices[i].x);
        sdlPoints[i].y = PHYSICS_TO_PIXEL(vertices[i].y);
    }
    
    SDL_SetRenderDrawColor(renderer, color.r * 255, color.g * 255, color.b * 255, color.a * 255);

    SDL_RenderDrawLines(renderer, sdlPoints, vertexCount);
}

void Box2DDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    DrawPolygon(vertices, vertexCount, color);
}

void Box2DDebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
    b2Vec2 p = center + b2Vec2(0.f, radius);
    constexpr float rotPerSector = 10.f;
    constexpr int vertexCount = static_cast<int>(360.f / rotPerSector);

    b2Vec2 vertices[vertexCount + 1];
    float theta = 0.f;
    int i = 0;

    for (; theta <= 360.f; theta += rotPerSector, ++i)
    {
        vertices[i] = p - center;
        b2Vec2& rotP = vertices[i];
        rotP.x = cos(DEGTORAD(theta)) * rotP.x - sin(DEGTORAD(theta)) * rotP.y;
        rotP.y = sin(DEGTORAD(theta)) * rotP.x + cos(DEGTORAD(theta)) * rotP.y; 
        vertices[i].Normalize();
        vertices[i] *= radius;
        vertices[i] += center;
    }
    DrawPolygon(vertices, vertexCount, color);
}

void Box2DDebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
    DrawCircle(center, radius, color);
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

void PhysicsContactListener::BeginContact(b2Contact* contact)
{
    // Get the colliding bodies
    b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();

    auto userDataA = (PhysicsUserData*)bodyA->GetUserData().pointer;
    auto userDataB = (PhysicsUserData*)bodyB->GetUserData().pointer;
    if (userDataA->type != userDataB->type)
    {
        if (userDataA->type == EntityType::Brick && bodyA->GetType() == b2_kinematicBody)
        {
            physics->onShouldActivateBrick(bodyA);
        }
        if (userDataB->type == EntityType::Brick && bodyB->GetType() == b2_kinematicBody)
        {
            physics->onShouldActivateBrick(bodyB);
        }
    }
}
