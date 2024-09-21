#include "GDMBasics.h"
#include <gtest/gtest.h>


bool test_bool = false;

void test_function(){
    test_bool = true;
}

TEST(InputActionsTest, FunctionActionCalls){
    auto room = std::make_shared<mate::Room>();
    auto element = room->addElement();
    auto input = element->addComponent<mate::InputActions>();

    input->addInput(sf::Keyboard::Key::W, &test_function);

    mate::isKeyPressed = [](sf::Keyboard::Key key){
        return false;
    };
    room->loop();
    ASSERT_EQ(test_bool, false);

    mate::isKeyPressed = [](sf::Keyboard::Key key){
        if (key == sf::Keyboard::Key::W)
            return false;
        return true;
    };
    room->loop();
    ASSERT_EQ(test_bool, false);


    mate::isKeyPressed = [](sf::Keyboard::Key key){
        if (key == sf::Keyboard::Key::W)
            return true;
        return false;
    };
    room->loop();
    ASSERT_EQ(test_bool, true);
}

TEST(InputActionsTest, MethodFunctionCalls){
    auto room = std::make_shared<mate::Room>();
    auto element = room->addElement();
    auto input = element->addComponent<mate::InputActions>();
    {
        auto element_b = room->addElement();
        input->addInput(sf::Keyboard::Key::F, &mate::Element::move, element_b, 5.0f, -2.0f);
        EXPECT_EQ(input->getActionsCount(), 1);

        mate::isKeyPressed = [](sf::Keyboard::Key key){
            return false;
        };

        room->loop();
        EXPECT_EQ(element_b->getWorldPosition(), sf::Vector2f (0.0f, 0.0f));

        mate::isKeyPressed = [](sf::Keyboard::Key key){
            return true;
        };
        room->loop();
        EXPECT_EQ(element_b->getWorldPosition(), sf::Vector2f (5.0f, -2.0f));
        element_b->destroy();
    }

    EXPECT_EQ(input->getActionsCount(), 1);
    room->loop();
    room->loop();
    EXPECT_EQ(input->getActionsCount(), 0);
}

