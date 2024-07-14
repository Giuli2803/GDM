#include "GDMBasics.h"

// TODO: Funcking fix Input Actions fist, that's probably the biggest issue here.

//Example of trigger implementation
namespace mate{
    class ColorTrigger : public Trigger {
        Sprite *_sprite = nullptr;
    public:
        explicit ColorTrigger(std::shared_ptr<Element> parent) : Trigger(std::move(parent)) {}

        void addSprite(Sprite &sprite){
            _sprite = &sprite;
        }

        void TriggerIn(Element& shooter) override{
            if(_sprite)
                _sprite->setColor(sf::Color::Green);
        }
    };
}

void print(){
    auto win = mate::Game::getGame()->getWindow();
    win->setSize(sf::Vector2u (960, 360));
}

int main()
{
    // Todo: I don't like users having to use pointers
    auto mainRoom = std::make_shared<mate::Room>();
    auto game = mate::Game::getGame(480, 360, "MyGame", mainRoom);

    auto camElem = mainRoom->AddElement();
    //*camera component
    auto camera = camElem->addComponent<mate::Camera>();
    camera->setScaleType(mate::Camera::ScaleType::LETTERBOX);

    auto sptElem0 = mainRoom->AddElement();
    auto sptElemTest = mainRoom->AddElement();
    //*sprite component
    auto spriteTest = sptElemTest->addComponent<mate::Sprite>();
    auto sprite1 = sptElem0->addComponent<mate::Sprite>(); // Error: El sprite este se sigue viendo despues de ser destruido el elemento. No ocurre con los hijos.
    sprite1->setColor(sf::Color::Red);
    //*input action component
    auto input1 = sptElem0->addComponent<mate::InputActions>();
    input1->AddInput(sf::Keyboard::Right, &mate::Element::move, sptElem0, 5.f, 0.f);
    input1->AddInput(sf::Keyboard::Left, &mate::Element::move, sptElem0, -5.f, 0.f);
    input1->AddInput(sf::Keyboard::Up, &mate::Element::move, sptElem0, 0.f, -5.f);
    input1->AddInput(sf::Keyboard::Down, &mate::Element::move, sptElem0, 0.f, 5.f);
    //TriggerShooter
    auto shooter = sptElem0->addComponent<mate::TriggerShooter>();
    shooter->shape = mate::RECTANGLE;
    shooter->offset.rect_bounds.width = 64;
    shooter->offset.rect_bounds.height = 64;

    auto child0 = sptElem0->AddChild();
    child0->setDepth(-1);
    child0->setPosition(-20, 0);
    child0->setScale(0.5, 0.5);
    child0->addComponent<mate::Sprite>();

    auto child1 = sptElem0->AddChild();
    child1->setDepth(-1);
    child1->setPosition(55, 0);
    child1->setScale(0.5, 0.5);
    child1->addComponent<mate::Sprite>();

    auto sptElem1 = mainRoom->AddElement();
    sptElem1->setPosition(180, 0);
    sptElem1->setScale(2, 2);
    //Sprite component
    auto sprite2 = sptElem1->addComponent<mate::Sprite>();
    sprite2->setTexture("../Circle.png");
    sprite2->setColor(sf::Color::Magenta);
    sprite2->setDepth(1);
    //input1->AddInput(sf::Keyboard::W, &mate::Element::Destroy, *sptElem0);
    input1->AddInput(sf::Keyboard::W, &mate::Element::Destroy, sptElemTest);
    input1->AddInput(sf::Keyboard::A, &mate::Element::Destroy, child0);
    input1->AddInput(sf::Keyboard::D, &mate::Element::Destroy, child1);
    input1->AddInput(sf::Keyboard::S, &print);
    input1->AddInput(sf::Keyboard::Space, &mate::Sprite::setColor, sprite2, sf::Color::Magenta);
    //ColorTrigger
    auto color = sptElem1->addComponent<mate::ColorTrigger>();
    color->addSprite(*sprite2);
    color->shape = mate::CIRCLE;
    color->offset.rect_bounds.width = 64;
    color->offset.rect_bounds.height = 64;

    game->gameLoop();
}
