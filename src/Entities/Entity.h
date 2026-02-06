//
// Created by Michael Hirsch on 05.02.26.
//

#pragma once
#include "EntityID.h"
#include "raylib.h"
#include "../Systems/Assets/Sprite.h"



class Entity {

    public:
    Entity( const Vector2 position, const Vector2 size, const Sprite sprite, const EntityID id )
    : position(position), size(size), sprite(sprite), id(id) {};

    virtual ~Entity();

    void SetPosition( const Vector2& newPosition) { position = newPosition; }
    void SetSize ( const Vector2& newSize) { size = newSize; }
    void SetSpeed( float newSpeed) { speed = newSpeed; }

    const Vector2& GetPosition() const { return position; }
    const Vector2& GetSize() const  { return size; }
    const Sprite& GetSprite() const { return sprite; }
    float GetSpeed() const { return speed; }
    const EntityID GetEntityID() const { return id; }
    bool IsAlive() const { return isAlive; }

    virtual void Update() = 0;

    private:
    Vector2 position;
    Vector2 size;
    Sprite sprite;
    EntityID id;
    float speed = 0;
    bool isAlive = true;
};
