//
// Created by Michael Hirsch on 05.02.26.
//

#pragma once
#include "raylib.h"
#include "../Systems/Assets/Sprite.h"



class Entity {

    public:
    Entity( Vector2 position, Vector2 size, Sprite sprite)
    : position(position), size(size), sprite(sprite) {};

    virtual ~Entity();

    void SetPosition( Vector2 newPosition) { position = newPosition; }
    void SetSize ( Vector2 newSize) { size = newSize; }
    void SetSpeed( float newSpeed) { speed = newSpeed; }

    Vector2 GetPosition() const { return position; }
    Vector2 GetSize() const  { return size; }
    Sprite GetSprite() const { return sprite; }
    float GetSpeed() const { return speed; }

    virtual void Update() = 0;

    private:
    Vector2 position;
    Vector2 size;
    Sprite sprite;
    float speed = 0;

};
