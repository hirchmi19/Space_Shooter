//
// Created by Michael Hirsch on 05.02.26.
//

#pragma once
#include "raylib.h"
#include "external/glfw/src/cocoa_platform.h"
#include "Systems/Assets/SpriteID.h"

class Entity {

    public:
    Entity(const Vector2& position, const Vector2& size, const SpriteID& sprite)
    : position(position), size(size), sprite(sprite) {};

    virtual ~Entity();

    void SetPosition(const Vector2& newPosition) { position = newPosition; }
    void SetSize (const Vector2& newSize) { size = newSize; }
    void SetSpeed(const float newSpeed) { speed = newSpeed; }

    Vector2& GetPosition() const { return position; }
    Vector2& GetSize() const { return size; }
    SpriteID& GetSpriteID() const { return sprite; }
    float GetSpeed() const { return speed; }

    virtual void Update() = 0;

    private:
    Vector2 position;
    Vector2 size;
    SpriteID sprite;
    float speed = 0;

};
