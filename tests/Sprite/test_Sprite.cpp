#include "GDMBasics.h"
#include <gtest/gtest.h>

TEST(SpriteTest, SpriteCoordsTest)
{
    auto element = std::make_shared<mate::Element>();
    auto sprite = element->addComponent<mate::Sprite>();

    sprite->offset.rect_bounds.left = 0;
    sprite->offset.rect_bounds.top = 0;
    sprite->offset.rect_bounds.width = 1;
    sprite->offset.rect_bounds.height = 1;
    auto ptr_sprite = sprite->getSprite();

    element->move(2, -2);
    sprite->loop();
    EXPECT_EQ(ptr_sprite->sprite.getPosition().x, 2);
    EXPECT_EQ(ptr_sprite->sprite.getPosition().y, -2);

    sprite->offset.rect_bounds.left = 3;
    sprite->offset.rect_bounds.top = 2;
    sprite->loop();
    EXPECT_EQ(ptr_sprite->sprite.getPosition().x, 5);
    EXPECT_EQ(ptr_sprite->sprite.getPosition().y, 0);

    element->scale(5, 2);
    sprite->loop();
    EXPECT_EQ(ptr_sprite->sprite.getScale().x, 5);
    EXPECT_EQ(ptr_sprite->sprite.getScale().y, 2);

    sprite->offset.rect_bounds.width = 2;
    sprite->offset.rect_bounds.height = 0.5f;
    sprite->loop();
    EXPECT_EQ(ptr_sprite->sprite.getScale().x, 10);
    EXPECT_EQ(ptr_sprite->sprite.getScale().y, 1);

    element->rotate(90);
    sprite->loop();
    EXPECT_EQ(ptr_sprite->sprite.getRotation(), 90);
}

TEST(SpriteTest, SpriteDepth)
{
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

TEST(SpriteTest, SpriteColor)
{
    auto element = std::make_shared<mate::Element>();
    auto sprite = element->addComponent<mate::Sprite>();
    auto ptr_sprite = sprite->getSprite();

    sprite->setColor(sf::Color::Green);
    EXPECT_EQ(ptr_sprite->sprite.getColor(), sf::Color::Green);

    sprite->setColor(sf::Color::Blue);
    EXPECT_EQ(ptr_sprite->sprite.getColor(), sf::Color::Blue);

    sf::Color color(15, 20, 20, 1);
    sprite->setColor(15, 20, 20, 1);
    EXPECT_EQ(ptr_sprite->sprite.getColor(), color);
}

TEST(SpriteTest, SpriteNoParent)
{
    std::shared_ptr<mate::Sprite> sprite;
    {
        auto element = std::make_shared<mate::Element>();
        sprite = element->addComponent<mate::Sprite>();
    }

    EXPECT_EQ(sprite->getElementDepth(), INT_MIN);
}
