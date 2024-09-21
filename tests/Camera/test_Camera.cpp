#include "GDMBasics.h"
#include <gtest/gtest.h>

TEST(CameraTest, CameraViewSize)
{
    auto room = std::make_shared<mate::Room>();
    auto game = mate::Game::getGame(400, 400, "MyGame", room);
    auto element = room->addElement();
    auto camera = element->addComponent<mate::Camera>();
    // This view size has an aspect ratio of 480/360 = 4/3
    camera->setSize(480, 360);

    // RESCALE should scale the view when the window size changes, maintaining the aspect ratio
    camera->setScaleType(mate::Camera::RESCALE);
    EXPECT_EQ(camera->getScaleType(), mate::Camera::RESCALE);

    game->setWindowSize(500, 360, camera->target_id);
    // We run multiple frames because the window resizing event can take a few frames to be triggered
    for (int i = 0; i < 10; ++i)
    {
        game->runSingleFrame();
    }
    EXPECT_EQ(camera->getRatio(), 480.0f / 360.0f);
    EXPECT_EQ(camera->getSize().x, 480);
    EXPECT_EQ(camera->getSize().y, 360);

    // LETTERBOX should maintain the aspect ratio no matter what happens to the window by adding black rectangles
    // For the propuse of testing this is the same as RESCALE since both the ratio and view size are constants
    camera->setScaleType(mate::Camera::LETTERBOX);
    EXPECT_EQ(camera->getScaleType(), mate::Camera::LETTERBOX);

    game->setWindowSize(960, 720, camera->target_id);
    // We run multiple frames because the window resizing event can take a few frames to be triggered
    for (int i = 0; i < 10; ++i)
    {
        game->runSingleFrame();
    }
    EXPECT_EQ(camera->getRatio(), 480.0f / 360.0f);
    EXPECT_EQ(camera->getSize().x, 480);
    EXPECT_EQ(camera->getSize().y, 360);

    // Testing that Letterbox works with to & bottom black lines too.
    game->setWindowSize(400, 800, camera->target_id);
    // We run multiple frames because the window resizing event can take a few frames to be triggered
    for (int i = 0; i < 10; ++i)
    {
        game->runSingleFrame();
    }
    EXPECT_EQ(camera->getRatio(), 480.0f / 360.0f);
    EXPECT_EQ(camera->getSize().x, 480);
    EXPECT_EQ(camera->getSize().y, 360);

    // REVEAL should rescale the view to show more of the world when the window is resized
    camera->setScaleType(mate::Camera::REVEAL);
    EXPECT_EQ(camera->getScaleType(), mate::Camera::REVEAL);

    game->setWindowSize(960, 360, camera->target_id);
    // We run multiple frames because the window resizing event can take a few frames to be triggered
    for (int i = 0; i < 10; ++i)
    {
        game->runSingleFrame();
    }
    EXPECT_EQ(camera->getRatio(), 960.0f / 360.0f);
    EXPECT_EQ(camera->getSize().x, 960);
    EXPECT_EQ(camera->getSize().y, 360);
}

TEST(CameraTest, CameraRatioTest)
{
    auto element = std::make_shared<mate::Element>();
    auto camera = element->addComponent<mate::Camera>();

    camera->setSize(400, 400);
    auto ratio = camera->getRatio();
    EXPECT_EQ(ratio, 1.0f);

    camera->setSize(500, 200);
    ratio = camera->getRatio();
    EXPECT_EQ(ratio, 500.0f / 200.0f);
}

TEST(CameraTest, CameraTarget)
{
    auto room = std::make_shared<mate::Room>();
    auto game = mate::Game::getGame(400, 400, "", room);
    auto element = room->addElement();
    auto camera = element->addComponent<mate::Camera>();

    EXPECT_EQ(camera->target_id, 0);

    auto target_id = camera->useNewTarget("");
    EXPECT_NE(camera->target_id, 0);
    EXPECT_EQ(target_id, camera->target_id);

    camera->setSize(500, 500);
    element->renderLoop();
    EXPECT_EQ((game->getView(target_id)).getViewport(), (camera->getView()).getViewport());
}

TEST(CameraTest, SpriteSorting)
{
    auto room = std::make_shared<mate::Room>();
    auto game = mate::Game::getGame(400, 400, "MyGame", room);
    auto element = room->addElement();
    auto camera = element->addComponent<mate::Camera>();

    auto element1 = room->addElement();
    auto sprite1 = element1->addComponent<mate::Sprite>();

    auto element2 = room->addElement();
    auto sprite2 = element2->addComponent<mate::Sprite>();

    auto element3 = room->addElement();
    auto sprite3 = element3->addComponent<mate::Sprite>();

    element1->depth = 0;
    element2->depth = 0;
    element3->depth = 0;

    sprite1->setSpriteDepth(1);
    sprite2->setSpriteDepth(2);
    sprite3->setSpriteDepth(3);

    camera->addSprite(sprite1);
    camera->addSprite(sprite2);
    camera->addSprite(sprite3);
    camera->renderLoop();
    // Lower depth sprites go to the front of the list (which prints it on the back).
    EXPECT_EQ(camera->getTopSprite().lock(), sprite1);
    EXPECT_EQ(camera->getBottomSprite().lock(), sprite3);

    element1->depth = 1;
    camera->renderLoop();
    // Element depth takes priority over sprite depth.
    EXPECT_EQ(camera->getTopSprite().lock(), sprite2);
    EXPECT_EQ(camera->getBottomSprite().lock(), sprite1);
}
