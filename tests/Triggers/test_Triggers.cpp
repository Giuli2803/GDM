#include "GDMBasics.h"
#include <gtest/gtest.h>

TEST(TriggersTest, TriggerShooterCoords)
{
    std::shared_ptr<mate::TriggerShooter> trigger_shooter;
    {
        auto room = std::make_shared<mate::Room>();
        room->setPosition(0, 0);
        room->setScale(1, 1);

        auto element = room->addElement();
        element->setPosition(0, 0);
        element->setScale(1, 1);

        trigger_shooter = element->addComponent<mate::TriggerShooter>();
        trigger_shooter->setPositionOffset(5, -5);
        trigger_shooter->setDimensionOffset(2, 0.5f);

        EXPECT_EQ(trigger_shooter->getPosition(), sf::Vector2f (5, -5));
        EXPECT_EQ(trigger_shooter->getDimensions(), sf::Vector2f (2, 0.5f));

        element->setPosition(-6, 5);
        element->setScale(0.1f, 2);

        EXPECT_EQ(trigger_shooter->getPosition(), sf::Vector2f (-1, 0));
        EXPECT_EQ(trigger_shooter->getDimensions(), sf::Vector2f (0.2f, 1));
    }
    EXPECT_EQ(trigger_shooter->getPosition(), sf::Vector2f (0, 0));
    EXPECT_EQ(trigger_shooter->getDimensions(), sf::Vector2f (0, 0));
}

namespace mate{
    class TestTrigger : public Trigger{
  public:
    TestTrigger() = default;
    explicit TestTrigger(const std::shared_ptr<LocalCoords> &parent) : Trigger(parent){}
    static int count;
    void triggerIn() override{
        count++;
    }
    };
    int TestTrigger::count = 0;
};

TEST(TriggersTest, TriggerCoords){
    auto room = std::make_shared<mate::Room>();
    auto element = room->addElement();
    auto trigger_c = std::make_unique<mate::TestTrigger>(element);
    trigger_c->setScale(2, 0.5f);
    element->setScale(4, 2);
    trigger_c->setPosition(2, -2);
    element->setPosition(-5, -5);

    EXPECT_EQ(trigger_c->getWorldPosition(), sf::Vector2f (-3, -7));
    EXPECT_EQ(trigger_c->getWorldScale(), sf::Vector2f (8, 1));
}

TEST(TriggersTest, TriggersTrackingOnManager){
    mate::TriggerManager manager;
    EXPECT_EQ(manager.getListCount(), 0);

    auto trigger_a = std::make_unique<mate::TestTrigger>();
    int id_a = trigger_a->getID();

    auto trigger_b = std::make_unique<mate::TestTrigger>();
    int id_b = trigger_b->getID();

    manager.addTrigger(std::move(trigger_a));
    EXPECT_EQ(manager.getListCount(), 1);
    EXPECT_EQ(manager.triggerIsContained(id_a), true);
    EXPECT_EQ(manager.triggerIsContained(id_b), false);

    manager.addTrigger(std::move(trigger_b));
    EXPECT_EQ(manager.getListCount(), 2);
    EXPECT_EQ(manager.triggerIsContained(id_b), true);

    manager.removeTrigger(id_a);
    manager.curateTriggers();
    EXPECT_EQ(manager.getListCount(), 1);
    EXPECT_EQ(manager.triggerIsContained(id_a), false);
    EXPECT_EQ(manager.triggerIsContained(id_b), true);


    auto trigger_c = std::make_unique<mate::TestTrigger>();
    int id_c = trigger_c->getID();
    trigger_c->destroy();
    EXPECT_EQ(trigger_c->shouldDestroy(), true);

    manager.addTrigger(std::move(trigger_c));
    EXPECT_EQ(manager.getListCount(), 2);
    EXPECT_EQ(manager.triggerIsContained(id_c), true);
    manager.curateTriggers();
    EXPECT_EQ(manager.getListCount(), 1);
    EXPECT_EQ(manager.triggerIsContained(id_c), false);
}

TEST(TriggersTest, TriggerActivation){
    int starting_count = mate::TestTrigger::count;

    auto room = std::make_shared<mate::Room>();
    auto game = mate::Game::getGame(400, 400, "", room);
    room->setScale(1, 1);
    room->setPosition(0, 0);

    auto trigger_a = std::make_unique<mate::TestTrigger>(room);
    trigger_a->setScale(2, 2);
    trigger_a->setPosition(5, 5);
    trigger_a->shape = mate::ShapeType::CIRCLE;

    auto trigger_b = std::make_unique<mate::TestTrigger>(room);
    trigger_b->setScale(2, 2);
    trigger_b->setPosition(-5, 5);
    trigger_b->shape = mate::ShapeType::RECTANGLE;

    ASSERT_EQ(mate::TestTrigger::count, starting_count);

    auto element = room->addElement();
    auto trigger_c = std::make_unique<mate::TestTrigger>(element);
    trigger_c->setScale(2, 2);
    element->setScale(1, 1);
    trigger_c->setPosition(0, 0);
    element->setPosition(-5, -5);
    trigger_c->shape = mate::ShapeType::CIRCLE;

    room->addTrigger(std::move(trigger_a));
    room->addTrigger(std::move(trigger_b));
    room->addTrigger(std::move(trigger_c));

    auto element_b = room->addElement();
    element_b->setPosition(5, -5);
    element_b->setScale(2, 2);
    auto shooter = element_b->addComponent<mate::TriggerShooter>();
    shooter->setDimensionOffset(1, 1);
    shooter->setPositionOffset(0, 0);
    shooter->shape = mate::ShapeType::RECTANGLE;

    room->loop();
    EXPECT_EQ(mate::TestTrigger::count, starting_count);

    // RECTANGLE moving shooter to CIRCLE  static trigger
    element_b->setPosition(5, 5);
    room->loop();
    EXPECT_EQ(mate::TestTrigger::count, starting_count+1);

    element_b->setPosition(4, 4);
    room->loop();
    EXPECT_EQ(mate::TestTrigger::count, starting_count+2);

    element_b->setPosition(3.001, 3.001);
    room->loop();
    EXPECT_EQ(mate::TestTrigger::count, starting_count+2);

    // RECTANGLE moving shooter to RECTANGLE static trigger
    element_b->setPosition(-5, 5);
    room->loop();
    EXPECT_EQ(mate::TestTrigger::count, starting_count+3);

    element_b->setPosition(-6.999, 3.001);
    room->loop();
    EXPECT_EQ(mate::TestTrigger::count, starting_count+4);

    // RECTANGLE moving shooter to CIRCLE moving trigger
    element_b->setPosition(0, 0);
    element->setPosition(0, 0);

    room->loop();
    EXPECT_EQ(mate::TestTrigger::count, starting_count+5);
    room->loop();
    EXPECT_EQ(mate::TestTrigger::count, starting_count+6);

    // CIRCLE moving trigger to CIRCLE static trigger
    shooter->shape = mate::ShapeType::CIRCLE;

    element_b->setPosition(5, 5);
    room->loop();
    EXPECT_EQ(mate::TestTrigger::count, starting_count+7);

    element_b->setPosition(4, 4);
    room->loop();
    EXPECT_EQ(mate::TestTrigger::count, starting_count+8);

    element_b->setPosition(3.001, 3.001);
    room->loop();
    EXPECT_EQ(mate::TestTrigger::count, starting_count+8);
}

