#include "GDMBasics.h"
#include <gtest/gtest.h>
#include <cmath>

// Game creation tests

TEST(BasicsTest, RoomSwitching)
{
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

TEST(BasicsTest, GameSingleton)
{
    std::shared_ptr<mate::Game> game = mate::Game::getGame();
    ASSERT_EQ(game, mate::Game::getGame());

    auto game_b = mate::Game::getGame(400, 400, "", std::move(std::make_shared<mate::Room>()));
    ASSERT_EQ(game_b, game);

    std::list<std::shared_ptr<mate::Room>> rooms;
    rooms.push_back(std::move(std::make_shared<mate::Room>()));
    auto game_c = mate::Game::getGame(400, 400, "", rooms);
    ASSERT_EQ(game, game_c);
    ASSERT_EQ(game_b, game_c);
}

TEST(BasicsTest, WindowSize)
{
    auto main_room = std::make_shared<mate::Room>();
    auto game = mate::Game::getGame(400, 800, "MyGame", main_room);

    EXPECT_EQ(game->getWindowSize().x, 400);
    EXPECT_EQ(game->getWindowSize().y, 800);

    game->setWindowSize(1600, 800);

    EXPECT_EQ(game->getWindowSize().x, 1600);
    EXPECT_EQ(game->getWindowSize().y, 800);
}

// Element tests

TEST(BasicsTest, ElementParent){
    auto room = std::make_shared<mate::Room>();
    auto element = room->addElement();

    std::weak_ptr<mate::LocalCoords> weak_room = room;
    EXPECT_EQ(element->getParent().lock(), weak_room.lock());

    auto element_b = std::make_shared<mate::Element>();
    EXPECT_EQ(element_b->getParent().lock(), nullptr);

    element_b->setParent(room);
    EXPECT_EQ(element_b->getParent().lock(), weak_room.lock());
}

TEST(BasicsTest, TopElementCreationAndDestruction)
{
    auto main_room = std::make_shared<mate::Room>();
    auto game = mate::Game::getGame(400, 400, "MyGame", main_room);

    // Assert if the element was properly created
    auto test_element = main_room->addElement();
    ASSERT_EQ(main_room->getLoopTypeCount<mate::Element>(), 1);

    // Assert if the element was properly destroyed
    test_element->destroy();
    game->runSingleFrame();
    EXPECT_EQ(main_room->getLoopTypeCount<mate::Element>(), 0);

    auto element_b = std::make_shared<mate::Element>();
    main_room->addElement(element_b);
    ASSERT_EQ(main_room->getLoopTypeCount<mate::Element>(), 1);

    element_b->destroy();
    game->runSingleFrame();
    EXPECT_EQ(main_room->getLoopTypeCount<mate::Element>(), 0);
}

float getAngle(float angle){
    angle = std::fmod(angle, 360.0f);
    if (angle < 0){
        angle += 360.0f;
    }
    return angle;
}

TEST(BasicsTest, ElementConstructors)
{
    std::shared_ptr<mate::Room> room = std::make_shared<mate::Room>();
    auto element_a = std::make_shared<mate::Element>(room);
    auto element_b = std::make_shared<mate::Element>(room, sf::Vector2f(20, -20));
    auto element_c = std::make_shared<mate::Element>(room, sf::Vector2f(2, -2), sf::Vector2f(2, 3));
    auto element_d = std::make_shared<mate::Element>(room, sf::Vector2f(-2, 0), 90);
    auto element_e = std::make_shared<mate::Element>(room, sf::Vector2f(0, 2), sf::Vector2f(1, 0.5), -90);

    EXPECT_EQ(element_a->getPosition().x, room->getPosition().x);
    EXPECT_EQ(element_a->getPosition().y, room->getPosition().y);
    EXPECT_EQ(element_a->getScale().x, room->getScale().x);
    EXPECT_EQ(element_a->getScale().y, room->getScale().y);
    EXPECT_EQ(element_a->getRotation(), room->getRotation());

    EXPECT_EQ(element_b->getPosition().x, room->getPosition().x + 20);
    EXPECT_EQ(element_b->getPosition().y, room->getPosition().y - 20);
    EXPECT_EQ(element_b->getScale().x, room->getScale().x);
    EXPECT_EQ(element_b->getScale().y, room->getScale().y);
    EXPECT_EQ(element_b->getRotation(), room->getRotation());

    EXPECT_EQ(element_c->getPosition().x, room->getPosition().x + 2);
    EXPECT_EQ(element_c->getPosition().y, room->getPosition().y - 2);
    EXPECT_EQ(element_c->getScale().x, room->getScale().x*2);
    EXPECT_EQ(element_c->getScale().y, room->getScale().y*3);
    EXPECT_EQ(element_c->getRotation(), room->getRotation());

    EXPECT_EQ(element_d->getPosition().x, room->getPosition().x - 2);
    EXPECT_EQ(element_d->getPosition().y, room->getPosition().y);
    EXPECT_EQ(element_d->getScale().x, room->getScale().x);
    EXPECT_EQ(element_d->getScale().y, room->getScale().y);
    EXPECT_EQ(element_d->getRotation(), getAngle(room->getRotation()+90));

    EXPECT_EQ(element_e->getPosition().x, room->getPosition().x);
    EXPECT_EQ(element_e->getPosition().y, room->getPosition().y + 2);
    EXPECT_EQ(element_e->getScale().x, room->getScale().x);
    EXPECT_EQ(element_e->getScale().y, room->getScale().y*0.5f);
    EXPECT_EQ(element_e->getRotation(), getAngle(room->getRotation()-90));
}

TEST(BasicsTest, ChildElementCreationAndDestruction)
{
    auto main_room = std::make_shared<mate::Room>();
    auto game = mate::Game::getGame(400, 800, "MyGame", main_room);

    // Assert if the element was properly created
    auto parent_element = main_room->addElement();
    auto child_element = parent_element->addChild();
    auto second_child_element = parent_element->addChild();

    ASSERT_EQ(main_room->getLoopTypeCount<mate::Element>(), 1);
    ASSERT_EQ(parent_element->getElementsCount(), 2);
    ASSERT_EQ(parent_element->getFullElementsCount(), 2);

    auto grandchild_element = child_element->addChild();

    ASSERT_EQ(parent_element->getElementsCount(), 2);
    ASSERT_EQ(child_element->getElementsCount(), 1);
    ASSERT_EQ(parent_element->getFullElementsCount(), 3);

    child_element->destroy();
    main_room->loop();

    ASSERT_EQ(parent_element->getElementsCount(), 1);
    ASSERT_EQ(parent_element->getFullElementsCount(), 1);
}

TEST(BasicsTest, ElementCoords)
{
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

TEST(BasicsTest, ChildElementCoords)
{
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

TEST(BasicsTest, WeakPointerIsUninitialized){
    std::weak_ptr<mate::Room> room;
    std::weak_ptr<mate::Element> element;

    EXPECT_EQ(mate::weakPtrIsUninitialized(room), true);
    EXPECT_EQ(mate::weakPtrIsUninitialized(element), true);

    {
        auto shared_room = std::make_shared<mate::Room>();
        auto shared_element = shared_room->addElement();
        room = shared_room;
        element = shared_element;

        EXPECT_EQ(mate::weakPtrIsUninitialized(room), false);
        EXPECT_EQ(mate::weakPtrIsUninitialized(element), false);
    }
    // Out of scope, shared_room & shared_element are expired

    EXPECT_EQ(room.expired(), true);
    EXPECT_EQ(element.expired(), true);
    EXPECT_EQ(mate::weakPtrIsUninitialized(room), false);
    EXPECT_EQ(mate::weakPtrIsUninitialized(element), false);
}

