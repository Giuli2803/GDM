#include "GDMBasics.h"

int main()
{
    auto game = mate::start();
    game->gameLoop();
}

namespace mate
{
game_instance start()
{
    auto main_room = std::make_shared<Room>();
    auto game = Game::getGame(480, 360, "MyGame", main_room);
    auto my_element = main_room->addElement();

    // ADD YOUR CODE HERE ------------------------------------------------
    /*
     * How to add another room to the game:
     *
     * auto second_room game->addRoom();
     *
     * How to switch between rooms:
     *
     * game->switchRoom(1); // To second_room
     * game->switchRoom(0); // To main_room
     */

    /* How to add an Element into a room:
     * auto my_element = main_room->addElement();
     *
     * How to modify the new Element:
     * my_element->setPosition(x, y);
     * my_element->move(x, y);
     * my_element->setScale(x, y);
     * my_element->scale(x, y);
     * my_element->setRotation(angle);
     * my_element->rotate(angle);
     * my_element->depth = new_depth;
     *
     * NOTE: depth indicates how close to the screen an Element is, the higher the depth the closer foreground.
     *
     * How to add an Element that follows the existing Element:
     * auto child_element = my_element->addChild();
     *
     * How to destroy an Element:
     * my_element->destroy();
     *
     * NOTE: When destroying an Element, all children Elements will be destroyed too.
     */

    /*
     * How to add a Camera into an Element:
     * auto my_camera = my_element->addComponent<Camera>();
     *
     * How to modify your new Camera:
     * my_camera->setScaleType(Camera::ScaleType::[RESCALE, REVEAL, LETTERBOX]);
     * my_camera->setSize(x, y);
     *
     * NOTE: The Camera size is NOT the same as the window size, it's the area of the world captured by the camera.
     */

    /*
     * How to add a Sprite to an Element:
     * auto my_sprite = my_element->addComponent<Sprite>();
     *
     * How to set a file to the Sprite:
     * my_sprite->addTexture("image[.png, .jpg, .jpeg]");
     *
     * How to modify the Sprite:
     * my_sprite->setColor(r, g, b, a);
     * my_sprite->setColor(sf::Color::[Green, Red, Blue, etc]);
     * my_sprite->setSpriteDepth(depth);
     * my_sprite->addDepth(extra_depth);
     *
     * NOTE: Sprite depth comes second to the Element depth, if there are 2 Sprites from Elements with the same depth
     * the sprite with the highest depth will be printed on top, but if the Elements depth are different then those will
     * be the ones taken in account.
     *
     * How to make the sprite visible/invisible on the screen:
     * my_camera->addSprite(my_sprite);
     * my_camera->removeSprite(my_sprite);
     */

    /*
     * How to add keyboard functions:
     * auto my_inputs = my_element->addComponent<InputActions>();
     * my_inputs->addInput(sf::Keyboard::[Right, Left, W, A, S, D, etc], &[function], [Element, Component], [Arguments]);
     *
     * Examples:
     * Move an Element with AWSD:
     * my_inputs->addInput(sf::Keyboard::D, &Element::move, my_element, 5.f, 0.f);
     * my_inputs->addInput(sf::Keyboard::A, &Element::move, my_element, -5.f, 0.f);
     * my_inputs->addInput(sf::Keyboard::W, &Element::move, my_element, 0.f, -5.f);
     * my_inputs->addInput(sf::Keyboard::S, &Element::move, my_element, 0.f, 5.f);
     *
     * Make sprite invisible with I and visible with K
     * my_inputs->addInput(sf::Keyboard::I, &Camera::removeSprite, my_camera, my_sprite);
     * my_inputs->addInput(sf::Keyboard::K, &Camera::addSprite, my_camera, my_sprite);
     *
     * Change the color of the Sprite with J and L
     * my_inputs->addInput(sf::Keyboard::I, &Sprite::setColor, my_sprite, sf::Color::Magenta);
     * my_inputs->addInput(sf::Keyboard::K, &Sprite::setColor, my_sprite, sf::Color::Green);
     */

    // -------------------------------------------------------------------

    return game;
}
} // namespace mate
