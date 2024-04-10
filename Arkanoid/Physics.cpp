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
	b2Vec2 gravity(GRAVITY); 
	world = new b2World(gravity);
	world->SetAllowSleeping(true);
    world->SetContactListener(&contactListener);
    contactListener.physics = this;

    ballUserData.type = EntityType::Ball;
    brickUserData.type = EntityType::Brick;
    playerUserData.type = EntityType::Player;
    killZoneUserData.type = EntityType::KillZone;

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
    if (!bricksToDestroy.empty())
    {
        for (auto brick : bricksToDestroy)
        {
            std::vector<b2Body*>::iterator it = std::remove(bricks.begin(), bricks.end(), brick);
            bricks.erase(it);
            removeBody(brick);
        }
        bricksToDestroy.clear();
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

void Physics::onShouldDestroyBrick(b2Body* body)
{
    bricksToDestroy.push_back(body);
}

//NOTE: To set the user data of the bodies give the address of the instantiated members of type PhysicsUserData in this class

b2Body* Physics::addBrick(float x, float y, float width, float height, b2BodyType bodyType, float initialRotation)
{
    // TODO: Implement this function
    // Hint: This function was originally implemented to create a brick in the physics world.
    // You need to define a body definition, set its type, position, and angle.
    // Then, create a body in the world using the body definition.
    // Define a box shape and set its dimensions.
    // Define fixture properties and create a fixture on the body.
    // Set various properties on the body such as damping and user data.
    // If the body is not static, add it to the list of bricks.
    return nullptr;
}

b2Body* Physics::addKillZone(float x, float y, float width, float height, b2BodyType bodyType, float initialRotation)
{
    // TODO: Implement this function
    // Hint: This function was originally implemented to create a kill zone in the physics world.
    // It's similar to creating a brick but with different user data.
    // You need to call the addBrick function and then set the user data to killZoneUserData.
    return nullptr;
}

b2Body* Physics::addBall(float x, float y, float radius)
{
    // TODO: Implement this function
    // Hint: This function was originally implemented to create a ball in the physics world.
    // You need to define a body definition, set its type and position.
    // Then, create a body in the world using the body definition.
    // Define a circle shape and set its radius.
    // Define fixture properties and create a fixture on the body.
    // Set various properties on the body such as damping, linear velocity, gravity scale, and user data.
    return nullptr;
}

b2Body* Physics::addPlayer(float x, float y, float width, float height)
{
    // TODO: Implement this function
    // Hint: This function was originally implemented to create a player in the physics world.
    // You need to define a body definition, set its type and position.
    // Then, create a body in the world using the body definition.
    // Define a box shape and set its dimensions.
    // Define fixture properties and create a fixture on the body.
    // Set various properties on the body such as gravity scale, linear damping, and user data.
    // Create a prismatic joint between the player and the kill zone.
    return nullptr;
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
    // TODO: Implement this function
    // Hint: This function was originally implemented to handle the beginning of a contact between two bodies.
    // You need to get the colliding bodies and their user data.
    // If the types of the two bodies are different, handle the collision based on the types.
    // If a brick collides with something and the brick is still kinematic, activate it, check onShouldActivateBrick.
    // If a player collides with a ball, modify the ball's linear velocity, this new velocity can be obtained from the vector
    // from the collision point to the center of the player for example
    // If a kill zone collides with a brick, destroy the brick, check onShouldDestroyBrick.

    // BONUS TODO: Implement power-ups
    // Hint: When a ball hits a brick, there could be a certain random probability of dropping a power-up.
    // The power-up could give the player the ability to shoot or make the player bigger.
    // You will need to define what each power-up does and how the player can get it.
    // You might need additional clases to handle the power ups and implement new keyboard controls depending of
    // what powerup are you implementing
}

