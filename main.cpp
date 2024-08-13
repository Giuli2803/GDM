#include "GDMBasics.h"

void print()
{
    auto game = mate::Game::getGame();
    game->setWindowSize(960, 340);
}

int main()
{
    auto game = mate::start();
    game->gameLoop();
}

namespace mate
{
// Example of trigger implementation
class ColorTrigger : public Trigger
{
    std::weak_ptr<Sprite> _sprite;

  public:
    explicit ColorTrigger(const std::weak_ptr<Element> &parent) : Trigger(parent, true)
    {
    }

    void addSprite(const std::shared_ptr<Sprite> &sprite)
    {
        _sprite = sprite;
    }

    void triggerIn() override
    {
        if (auto spt_sprite = _sprite.lock())
            spt_sprite->setColor(sf::Color::Green);
    }
};

game_instance start()
{
    auto mainRoom = std::make_shared<mate::Room>();
    auto game = mate::Game::getGame(480, 360, "MyGame", mainRoom);

    auto camElem = mainRoom->addElement();
    //*camera component
    auto camera = camElem->addComponent<mate::Camera>();
    camera->setScaleType(mate::Camera::ScaleType::REVEAL);

    auto camElem2 = mainRoom->addElement();
    //*camera component
    auto camera2 = camElem2->addComponent<mate::Camera>();
    camera2->useNewTarget();

    auto sptElem0 = mainRoom->addElement();
    //*sprite component
    auto sprite1 = sptElem0->addComponent<mate::Sprite>();
    camera->addSprite(sprite1);
    sprite1->setColor(sf::Color::Red);
    //*input action component
    auto input1 = sptElem0->addComponent<mate::InputActions>();
    input1->addInput(sf::Keyboard::Right, &mate::Element::move, sptElem0, 5.f, 0.f);
    input1->addInput(sf::Keyboard::Left, &mate::Element::move, sptElem0, -5.f, 0.f);
    input1->addInput(sf::Keyboard::Up, &mate::Element::move, sptElem0, 0.f, -5.f);
    input1->addInput(sf::Keyboard::Down, &mate::Element::move, sptElem0, 0.f, 5.f);
    // TriggerShooter
    auto shooter = sptElem0->addComponent<mate::TriggerShooter>();
    shooter->shape = mate::RECTANGLE;
    shooter->setDimensionOffset(64, 64);

    auto child0 = sptElem0->addChild();
    child0->depth = -1;
    child0->setPosition(-20, 0);
    child0->setScale(0.5, 0.5);
    camera->addSprite((child0->addComponent<mate::Sprite>()));

    auto child1 = sptElem0->addChild();
    child1->depth = -1;
    child1->setPosition(55, 0);
    child1->setScale(0.5, 0.5);
    camera->addSprite((child1->addComponent<mate::Sprite>()));

    auto sptElem1 = mainRoom->addElement();
    sptElem1->setPosition(0, 0);
    sptElem1->setScale(2, 2);
    sptElem1->depth = -2;
    // Sprite component
    auto sprite2 = sptElem1->addComponent<mate::Sprite>();
    sprite2->setTexture("../Circle.png");
    sprite2->setColor(sf::Color::Magenta);
    camera2->addSprite(sprite2);
    // sprite2->setDepth(-10);
    // ColorTrigger
    auto color = std::make_unique<ColorTrigger>(sptElem1);
    // input1->AddInput(sf::Keyboard::W, &mate::Game::RemoveTrigger, game,
    // color->getID());
    color->addSprite(sprite2);
    color->shape = mate::CIRCLE;
    color->setDimensionOffset(64, 64);
    game->addTrigger(std::move(color));

    input1->addInput(sf::Keyboard::W, &mate::Element::destroy, sptElem1);
    input1->addInput(sf::Keyboard::A, &mate::Element::destroy, child0);
    input1->addInput(sf::Keyboard::D, &mate::Element::destroy, child1);
    input1->addInput(sf::Keyboard::S, &print);
    input1->addInput(sf::Keyboard::Space, &mate::Sprite::setColor, sprite2, sf::Color::Magenta);

    return game;
}
} // namespace mate
