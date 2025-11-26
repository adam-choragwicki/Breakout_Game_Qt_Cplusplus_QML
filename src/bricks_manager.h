#pragma once

#include "entities/brick.h"
#include "brick_layouts/brick_layout_library.h"
#include <vector>
#include <QDebug>
#include <QSize>

class BricksManager
{
public:
    BricksManager() = default;

    void reset()
    {
        bricks_.clear();
    }

    void createBricksFromLayout(const std::vector<BrickLayoutEntry>& layout, const QSize& gameArenaSize)
    {
        bricks_.clear();

        // qDebug() << "Game arena size: " << gameArenaSize.width() << "x" << gameArenaSize.height();

        for (const auto& [xRel, yRel, wRel, hRel, color]: layout)
        {
            bricks_.emplace_back(gameArenaSize.width() * xRel,
                                 gameArenaSize.height() * yRel,
                                 gameArenaSize.width() * wRel,
                                 gameArenaSize.height() * hRel,
                                 color);
        }
    }

    [[nodiscard]] const std::vector<Brick>& getBricks() const
    {
        return bricks_;
    }

    std::vector<Brick>& getBricks()
    {
        return bricks_;
    }

    [[nodiscard]] int getAliveBricksCount() const
    {
        return static_cast<int>(std::ranges::count_if(bricks_,
                                                      [](const Brick& brick)
                                                      {
                                                          return brick.isAlive();
                                                      }));
    }

private:
    std::vector<Brick> bricks_;
};
