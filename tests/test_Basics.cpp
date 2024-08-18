//
// Created by elly_sparky on 27/02/24.
//

#include "GDMBasics.h"
#include <gtest/gtest.h>

///////////////////////////Game creation tests///////////////////////////////
TEST(BasicsTest, RoomSwitching) {
    auto main_room = std::make_shared<mate::Room>();
    auto second_room = std::make_shared<mate::Room>();

    std::shared_ptr<mate::Game> game = mate::Game::getGame(400, 400, "Game", main_room);
    game->addRoom(second_room);
    auto third_room = game->addRoom();

    ASSERT_EQ(game->getActiveRoom(), main_room);
    game->switchRoom(3);
    ASSERT_EQ(game->getActiveRoom(), main_room);
    game->switchRoom(1);
    ASSERT_EQ(game->getActiveRoom(), second_room);
    game->switchRoom(2);
    ASSERT_EQ(game->getActiveRoom(), third_room);
    game->switchRoom(0);
    ASSERT_EQ(game->getActiveRoom(), main_room);
}

TEST(BasicsTest, GameSingleton) {
    std::shared_ptr<mate::Game> game = mate::Game::getGame();
    ASSERT_EQ(game, mate::Game::getGame());
}

TEST(BasicsTest, WindowSize) {
    auto main_room = std::make_shared<mate::Room>();
    auto game = mate::Game::getGame(400, 800, "MyGame", main_room);

    EXPECT_EQ(game->getWindowSize().x, 400);
    EXPECT_EQ(game->getWindowSize().y, 800);

    game->setWindowSize(1600, 800);

    EXPECT_EQ(game->getWindowSize().x, 1600);
    EXPECT_EQ(game->getWindowSize().y, 800);
}

//////////////////////////Elements tests////////////////////////////
TEST(BasicsTest, TopElementCreationAndDestruction){
    auto main_room = std::make_shared<mate::Room>();
    auto game = mate::Game::getGame(400, 400, "MyGame", main_room);

    //Assert if the element was properly created
    auto test_element = main_room->addElement();
    ASSERT_EQ(main_room->getElementsCount(), 1);

    //Assert if the element was properly destroyed
    test_element->destroy();
    game->run_single_frame();
    EXPECT_EQ(main_room->getElementsCount(), 0);
}

TEST(BasicsTest, ChildElementCreationAndDestruction){
    auto main_room = std::make_shared<mate::Room>();
    auto game = mate::Game::getGame(400, 800, "MyGame", main_room);

    //Assert if the element was properly created
    auto parent_element = main_room->addElement();
    auto child_element = parent_element->addChild();
    auto second_child_element = parent_element->addChild();

    ASSERT_EQ(main_room->getElementsCount(), 1);
    ASSERT_EQ(parent_element->getElementsCount(), 2);
    ASSERT_EQ(parent_element->getFullElementsCount(), 2);

    auto grandchild_element = child_element->addChild();

    ASSERT_EQ(parent_element->getElementsCount(), 2);
    ASSERT_EQ(child_element->getElementsCount(), 1);
    ASSERT_EQ(parent_element->getFullElementsCount(), 3);

    child_element->destroy();
    game->run_single_frame();

    ASSERT_EQ(parent_element->getElementsCount(), 1);
    ASSERT_EQ(parent_element->getFullElementsCount(), 1);
}

TEST(BasicsTest, ElementCoords) {
    auto room = std::make_shared<mate::Room>();
    mate::Element element(room);

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

TEST(BasicsTest, ChildElementCoords) {
    auto room = std::make_shared<mate::Room>();
    auto element = room->addElement();
    auto child = element->addChild();

    child->setPosition(0, 0);
    child->setScale(1, 1);
    child->setRotation(0);

    element->setPosition(-5, 5);
    EXPECT_EQ(child->getWorldPosition().x, -5);
    EXPECT_EQ(child->getWorldPosition().y, 5);
    EXPECT_EQ(child->getPosition().x, 0);
    EXPECT_EQ(child->getPosition().y, 0);

    child->move(-1, 1);
    EXPECT_EQ(child->getWorldPosition().x, -6);
    EXPECT_EQ(child->getWorldPosition().y, 6);
    EXPECT_EQ(child->getPosition().x, -1);
    EXPECT_EQ(child->getPosition().y, 1);


    element->setScale(2, -2);
    EXPECT_EQ(child->getWorldScale().x, 2);
    EXPECT_EQ(child->getWorldScale().y, -2);
    EXPECT_EQ(child->getScale().x, 1);
    EXPECT_EQ(child->getScale().y, 1);

    child->scale(0.5, 2);
    EXPECT_EQ(child->getWorldScale().x, 1);
    EXPECT_EQ(child->getWorldScale().y, -4);
    EXPECT_EQ(child->getScale().x, 0.5);
    EXPECT_EQ(child->getScale().y, 2);

    element->setRotation(50);
    EXPECT_EQ(child->getWorldRotation(), 50);
    EXPECT_EQ(child->getRotation(), 0);

    child->rotate(30);
    EXPECT_EQ(child->getWorldRotation(), 80);
    EXPECT_EQ(child->getRotation(), 30);

    child->rotate(-90);
    EXPECT_EQ(child->getWorldRotation(), 350);
    EXPECT_EQ(child->getRotation(), 300);
}

//////////////////////////Components creation tests////////////////////////////
/*TEST(BasicsTest, ComponentCreation){
    auto room = std::make_shared<mate::Room>();
    mate::Element element(room);
    auto sprite = element.addComponent<mate::Sprite>();

    ASSERT_NE(element.getComponent<mate::Sprite>(), nullptr);
    ASSERT_EQ(element.getComponent<mate::Camera>(), nullptr);
    ASSERT_EQ(element.getComponent<mate::Sprite>(), sprite);
}*/

//////////////////////////Sprites tests////////////////////////////
TEST(BasicsTest, SpriteDepth) {
    auto room = std::make_shared<mate::Room>();
    auto element = room->addElement();
    auto sprite = element->addComponent<mate::Sprite>();
    sprite->setSpriteDepth(0);

    sprite->addDepth(-1);
    EXPECT_EQ(sprite->getSpriteDepth(), 0);
    EXPECT_EQ(sprite->getElementDepth(), 0);

    sprite->addDepth(1);
    EXPECT_EQ(sprite->getSpriteDepth(), 1);
    EXPECT_EQ(sprite->getElementDepth(), 0);

    sprite->addDepth(5);
    EXPECT_EQ(sprite->getSpriteDepth(), 6);

    sprite->addDepth(-3);
    EXPECT_EQ(sprite->getSpriteDepth(), 3);

    sprite->setSpriteDepth(UINT_MAX);
    sprite->addDepth(1);
    EXPECT_EQ(sprite->getSpriteDepth(), UINT_MAX);

    element->depth -= 2;
    EXPECT_EQ(sprite->getSpriteDepth(), UINT_MAX);
    EXPECT_EQ(sprite->getElementDepth(), -2);

}

//////////////////////////Camera tests////////////////////////////
/*TEST(BasicsTest, CameraViewSize) {
    auto room = std::make_shared<mate::Room>();
    auto game = mate::Game::getGame(400, 400, "MyGame", room);
    auto element = room->addElement();
    auto camera = element->addComponent<mate::Camera>();
    //This view size has an aspect ratio of 480/360 = 4/3
    camera->setSize(480, 360);

    //RESCALE should scale the view when the window size changes, maintaining the aspect ratio
    camera->setScaleType(mate::Camera::RESCALE);
    game->setWindowSize(500, 360);
    game->run_single_frame();
    EXPECT_EQ(camera->getRatio(), 4.0f/3.0f);
    EXPECT_EQ(camera->getSize().x, 480);
    EXPECT_EQ(camera->getSize().y, 360);

    //REVEAL should rescale the view to show more of the world when the window is resized
    camera->setScaleType(mate::Camera::REVEAL);
    game->setWindowSize(960, 360);
    game->run_single_frame();
    EXPECT_EQ(camera->getRatio(), 8.0f/3.0f);
    EXPECT_EQ(camera->getSize().x, 960);
    EXPECT_EQ(camera->getSize().y, 360);

    //LETTERBOX should maintain the aspect ratio no matter what happens to the window by adding black rectangles
    //For the propuse of testing this is the same as RESCALE since both the ratio and view size are constants
    camera->setScaleType(mate::Camera::LETTERBOX);
    game->setWindowSize(960, 720);
    game->run_single_frame();
    EXPECT_EQ(camera->getRatio(), 3.0f/8.0f);
    EXPECT_EQ(camera->getSize().x, 960);
    EXPECT_EQ(camera->getSize().y, 360);
}*/
