//
// Created by elly_sparky on 27/02/24.
//

#include "../lib/GDMBasics/GDMBasics.h"
#include <gtest/gtest.h>

///////////////////////////Game creation tests///////////////////////////////
TEST(BasicsTest, GameSingleton) {
    mate::Game *game = mate::Game::getGame();
    ASSERT_EQ(game, mate::Game::getGame());
}

TEST(BasicsTest, RoomSwitching) {
    mate::Room mainRoom;
    mate::Room secondRoom;
    mate::Room thirdRoom;

    mate::Game *game = mate::Game::getGame(400, 400, "MyGame", &mainRoom);
    game->addRoom(&secondRoom);
    game->addRoom(&thirdRoom);

    game->switchRoom(3);
    ASSERT_EQ(game->getMainRoom(), &mainRoom);
    game->switchRoom(1);
    ASSERT_EQ(game->getMainRoom(), &secondRoom);
    game->switchRoom(2);
    ASSERT_EQ(game->getMainRoom(), &thirdRoom);
    game->switchRoom(0);
    ASSERT_EQ(game->getMainRoom(), &mainRoom);
}

TEST(BasicsTest, WindowSize) {
    mate::Room mainRoom;
    mate::Game *game = mate::Game::getGame(400, 800, "MyGame", &mainRoom);
    mate::render_target *window = game->getWindowTarget();

    EXPECT_EQ(window->target->getSize().x, 400);
    EXPECT_EQ(window->target->getSize().y, 800);

    game->setWindowSize(1600, 800);

    EXPECT_EQ(window->target->getSize().x, 1600);
    EXPECT_EQ(window->target->getSize().y, 800);
}

//////////////////////////Elements tests////////////////////////////
TEST(BasicsTest, TopElementCreationAndDestruction){
    mate::Room mainRoom;
    mate::Game *game = mate::Game::getGame(400, 400, "MyGame", &mainRoom);

    //Assert if the element was properly created
    mate::Element& test_element = mainRoom.AddElement();
    ASSERT_EQ(mainRoom.getElementsCount(), 1);

    //Assert if the element was properly destroyed
    test_element.Destroy();
    game->gameLoop(true);
    EXPECT_EQ(mainRoom.getElementsCount(), 0);
}

TEST(BasicsTest, ChildElementCreationAndDestruction){
    mate::Room mainRoom;
    mate::Game *game = mate::Game::getGame(400, 400, "", &mainRoom);

    //Assert if the element was properly created
    mate::Element& parent_element = mainRoom.AddElement();
    mate::Element& child_element = parent_element.AddChild();
    mate::Element& second_child_element = parent_element.AddChild();

    ASSERT_EQ(mainRoom.getElementsCount(), 1);
    ASSERT_EQ(parent_element.getElementsCount(), 2);
    ASSERT_EQ(parent_element.getFullElementsCount(), 2);

    mate::Element& grandchild_element = child_element.AddChild();

    ASSERT_EQ(parent_element.getElementsCount(), 2);
    ASSERT_EQ(child_element.getElementsCount(), 1);
    ASSERT_EQ(parent_element.getFullElementsCount(), 3);

    child_element.Destroy();
    game->gameLoop(true);

    ASSERT_EQ(parent_element.getElementsCount(), 1);
    ASSERT_EQ(parent_element.getFullElementsCount(), 1);
}

TEST(BasicsTest, ElementTransform) {
    mate::Room room;
    mate::Element element(&room);

    element.setPosition(0, 0);
    element.setScale(1, 1);
    element.setRotation(0);

    element.move(-5, 5);
    EXPECT_EQ(element.getWorldPosition().x, -5);
    EXPECT_EQ(element.getWorldPosition().y, 5);
    EXPECT_EQ(element.getPosition().x, -5);
    EXPECT_EQ(element.getPosition().y, 5);

    element.scale(2, -2);
    EXPECT_EQ(element.getWorldScale().x, 2);
    EXPECT_EQ(element.getWorldScale().y, -2);
    EXPECT_EQ(element.getScale().x, 2);
    EXPECT_EQ(element.getScale().y, -2);

    element.rotate(50);
    EXPECT_EQ(element.getWorldRotation(), 50);
    EXPECT_EQ(element.getRotation(), 50);

    element.rotate(-150);
    EXPECT_EQ(element.getWorldRotation(), 260);
    EXPECT_EQ(element.getRotation(), 260);
}

TEST(BasicsTest, ChildElementTransform) {
    mate::Room room;
    mate::Element element(&room);
    mate::Element child(&element);

    child.setPosition(0, 0);
    child.setScale(1, 1);
    child.setRotation(0);

    element.setPosition(-5, 5);
    EXPECT_EQ(child.getWorldPosition().x, -5);
    EXPECT_EQ(child.getWorldPosition().y, 5);
    EXPECT_EQ(child.getPosition().x, 0);
    EXPECT_EQ(child.getPosition().y, 0);

    child.move(-1, 1);
    EXPECT_EQ(child.getWorldPosition().x, -6);
    EXPECT_EQ(child.getWorldPosition().y, 6);
    EXPECT_EQ(child.getPosition().x, -1);
    EXPECT_EQ(child.getPosition().y, 1);


    element.setScale(2, -2);
    EXPECT_EQ(child.getWorldScale().x, 2);
    EXPECT_EQ(child.getWorldScale().y, -2);
    EXPECT_EQ(child.getScale().x, 1);
    EXPECT_EQ(child.getScale().y, 1);

    child.scale(0.5, 2);
    EXPECT_EQ(child.getWorldScale().x, 1);
    EXPECT_EQ(child.getWorldScale().y, -4);
    EXPECT_EQ(child.getScale().x, 0.5);
    EXPECT_EQ(child.getScale().y, 2);

    element.setRotation(50);
    EXPECT_EQ(child.getWorldRotation(), 50);
    EXPECT_EQ(child.getRotation(), 0);

    child.rotate(30);
    EXPECT_EQ(child.getWorldRotation(), 80);
    EXPECT_EQ(child.getRotation(), 30);

    child.rotate(-90);
    EXPECT_EQ(child.getWorldRotation(), 350);
    EXPECT_EQ(child.getRotation(), 300);
}

//////////////////////////Components creation tests////////////////////////////
TEST(BasicsTest, ComponentCreation){
    mate::Room room;
    mate::Element element(&room);
    auto& sprite = element.addComponent<mate::Sprite>();

    ASSERT_NE(element.getComponent<mate::Sprite>(), nullptr);
    ASSERT_EQ(element.getComponent<mate::Camera>(), nullptr);
    ASSERT_EQ(element.getComponent<mate::Sprite>(), &sprite);
}

//////////////////////////Sprites tests////////////////////////////
TEST(BasicsTest, SpriteDepth) {
    mate::Room room;
    mate::Element element(&room);
    auto& sprite = element.addComponent<mate::Sprite>();
    sprite.setDepth(0);

    sprite.addDepth(-1);
    EXPECT_EQ(sprite.getDepth(), 0);

    sprite.addDepth(1);
    EXPECT_EQ(sprite.getDepth(), 1);

    sprite.addDepth(5);
    EXPECT_EQ(sprite.getDepth(), 6);

    sprite.addDepth(-3);
    EXPECT_EQ(sprite.getDepth(), 3);

    sprite.setDepth(UINT_MAX);
    sprite.addDepth(1);
    EXPECT_EQ(sprite.getDepth(), UINT_MAX);
}

//////////////////////////Camera tests////////////////////////////
TEST(BasicsTest, CameraViewSize) {
    mate::Room room;
    mate::Game *game = mate::Game::getGame(100, 100, "", &room);
    mate::Element element(&room);
    auto& camera = element.addComponent<mate::Camera>();
    //This view size has an aspect ratio of 480/360 = 4/3
    camera.setSize(480, 360);

    //RESCALE should scale the view when the window size changes, maintaining the aspect ratio
    camera.setScaleType(mate::Camera::RESCALE);
    game->getWindow()->setSize(sf::Vector2u(500, 360));
    game->gameLoop(true);
    EXPECT_EQ(camera.getRatio(), 4.0f/3.0f);
    EXPECT_EQ(camera.getSize().x, 480);
    EXPECT_EQ(camera.getSize().y, 360);

    //REVEAL should rescale the view to show more of the world when the window is resized
    camera.setScaleType(mate::Camera::REVEAL);
    game->getWindow()->setSize(sf::Vector2u(960, 360));
    game->gameLoop(true);
    EXPECT_EQ(camera.getRatio(), 8.0f/3.0f);
    EXPECT_EQ(camera.getSize().x, 960);
    EXPECT_EQ(camera.getSize().y, 360);

    //LETTERBOX should maintain the aspect ratio no matter what happens to the window by adding black rectangles
    //For the propuse of testing this is the same as RESCALE since both the ratio and view size are constants
    camera.setScaleType(mate::Camera::LETTERBOX);
    game->getWindow()->setSize(sf::Vector2u(960, 720));
    game->gameLoop(true);
    EXPECT_EQ(camera.getRatio(), 3.0f/8.0f);
    EXPECT_EQ(camera.getSize().x, 960);
    EXPECT_EQ(camera.getSize().y, 360);
}