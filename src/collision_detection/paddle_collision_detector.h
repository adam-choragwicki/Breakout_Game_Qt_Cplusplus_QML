#pragma once

#include "entities/ball.h"
#include "entities/paddle.h"
#include "collision_common.h"
#include <optional>

class PaddleCollisionDetector
{
public:
    struct PaddleCollisionInfo
    {
        const HitSide hitSide;

        explicit PaddleCollisionInfo(const HitSide side) : hitSide(side)
        {
            // this constructor prevents creating an object without meaningful hitSide defined
        }
    };

    static std::optional<PaddleCollisionInfo> checkBallCollisionWithPaddle(const Ball& ball, const Paddle& paddle);
};
