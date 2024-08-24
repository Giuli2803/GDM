#include "GDMBasics.h"
#include <gtest/gtest.h>

TEST(CameraTest, CameraViewSize) {
    auto room = std::make_shared<mate::Room>();
    auto game = mate::Game::getGame(400, 400, "MyGame", room);
    auto element = room->addElement();
    auto camera = element->addComponent<mate::Camera>();
    //This view size has an aspect ratio of 480/360 = 4/3
    camera->setSize(480, 360);

    //RESCALE should scale the view when the window size changes, maintaining the aspect ratio
    camera->setScaleType(mate::Camera::RESCALE);
    EXPECT_EQ(camera->getScaleType(), mate::Camera::RESCALE);

    game->setWindowSize(500, 360, camera->target_id);
    // We run multiple frames because the window resizing event can take a few frames to be triggered
    for (int i = 0; i < 10; ++i)
    {
        game->runSingleFrame();
    }
    EXPECT_EQ(camera->getRatio(), 480.0f/360.0f);
    EXPECT_EQ(camera->getSize().x, 480);
    EXPECT_EQ(camera->getSize().y, 360);

    //LETTERBOX should maintain the aspect ratio no matter what happens to the window by adding black rectangles
    //For the propuse of testing this is the same as RESCALE since both the ratio and view size are constants
    camera->setScaleType(mate::Camera::LETTERBOX);
    EXPECT_EQ(camera->getScaleType(), mate::Camera::LETTERBOX);

    game->setWindowSize(960, 720, camera->target_id);
    // We run multiple frames because the window resizing event can take a few frames to be triggered
    for (int i = 0; i < 10; ++i)
    {
        game->runSingleFrame();
    }
    EXPECT_EQ(camera->getRatio(), 480.0f/360.0f);
    EXPECT_EQ(camera->getSize().x, 480);
    EXPECT_EQ(camera->getSize().y, 360);

    //REVEAL should rescale the view to show more of the world when the window is resized
    camera->setScaleType(mate::Camera::REVEAL);
    EXPECT_EQ(camera->getScaleType(), mate::Camera::REVEAL);

    game->setWindowSize(960, 360, camera->target_id);
    // We run multiple frames because the window resizing event can take a few frames to be triggered
    for (int i = 0; i < 10; ++i)
    {
        game->runSingleFrame();
    }
    EXPECT_EQ(camera->getRatio(), 960.0f/360.0f);
    EXPECT_EQ(camera->getSize().x, 960);
    EXPECT_EQ(camera->getSize().y, 360);
}
