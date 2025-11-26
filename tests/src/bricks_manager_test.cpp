#include <gtest/gtest.h>
#include "bricks_manager.h"
#include "entities/brick.h"
#include <QColor>

class BricksManagerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        layout = {
            {0.1, 0.1, 0.2, 0.1, Qt::red},
            {0.4, 0.2, 0.2, 0.1, Qt::blue}
        };
    }

    std::vector<BrickLayoutEntry> layout;
    BricksManager manager;
};

TEST_F(BricksManagerTest, CreateBricksFromLayout)
{
    constexpr QSize gameArenaSize(1000, 500);

    manager.createBricksFromLayout(layout, gameArenaSize);

    ASSERT_EQ(manager.getBricks().size(), 2);

    const Brick& b0 = manager.getBricks()[0];
    EXPECT_DOUBLE_EQ(b0.x(), 100); // 1000 * 0.1
    EXPECT_DOUBLE_EQ(b0.y(), 50); // 500 * 0.1
    EXPECT_DOUBLE_EQ(b0.width(), 200);
    EXPECT_DOUBLE_EQ(b0.height(), 50);
    EXPECT_EQ(b0.color(), Qt::red);
    EXPECT_TRUE(b0.isAlive());

    const Brick& b1 = manager.getBricks()[1];
    EXPECT_DOUBLE_EQ(b1.x(), 400);
    EXPECT_DOUBLE_EQ(b1.y(), 100);
    EXPECT_EQ(b1.color(), Qt::blue);
}

TEST_F(BricksManagerTest, AliveBricksCount)
{
    constexpr QSize gameArenaSize(1000, 500);

    manager.createBricksFromLayout(layout, gameArenaSize);

    EXPECT_EQ(manager.getAliveBricksCount(), 2);

    manager.getBricks()[0].destroy();
    EXPECT_EQ(manager.getAliveBricksCount(), 1);

    manager.getBricks()[1].setAlive(false);
    EXPECT_EQ(manager.getAliveBricksCount(), 0);
}

TEST_F(BricksManagerTest, ResetClearsBricks)
{
    constexpr QSize gameArenaSize(1000, 500);

    manager.createBricksFromLayout(layout, gameArenaSize);

    EXPECT_EQ(manager.getBricks().size(), 2);

    manager.reset();
    EXPECT_EQ(manager.getBricks().size(), 0);
}
