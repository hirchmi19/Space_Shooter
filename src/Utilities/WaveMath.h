//
// Created by Michael Hirsch on 12.02.26.
//

#pragma once

#include "raylib.h"

inline Vector2 Lerp(const Vector2& a, const Vector2& b, const float t)
{
    return {
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t
    };
}

inline Vector2 CubicBezier(const Vector2& p0,const Vector2& p1,const Vector2& p2,const Vector2& p3, const float t)
{
    const Vector2 a = Lerp(p0, p1, t);
    const Vector2 b = Lerp(p1, p2, t);
    const Vector2 c = Lerp(p2, p3, t);

    const Vector2 d = Lerp(a, b, t);
    const Vector2 e = Lerp(b, c, t);

    return Lerp(d, e, t);
}