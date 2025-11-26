#pragma once

#include "entities/ball.h"
#include "entities/brick.h"
#include "collision_common.h"

class BrickCollisionDetector
{
public:
    struct BrickCollisionInfo
    {
        const HitSide hitSide;
        const int brickIndex; // index of collided brick

        explicit BrickCollisionInfo(const HitSide side, const int brickIndex) : hitSide(side), brickIndex(brickIndex)
        {
            // this constructor prevents creating an object without a meaningful hitSide and brick index
        }
    };

    static std::optional<BrickCollisionInfo> checkBallCollisionWithBricks(const Ball& ball, const std::vector<Brick>& bricks);
};
