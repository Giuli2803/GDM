#include "GDMBasics.h"
#include <random>

int main()
{
    auto game = mate::start();
    game->gameLoop();
}

namespace mate
{
game_instance start()
{
    /*
     * The following it's an example of how the different scale type configurations of a camera work. You can build and
     * execute this example to see how each camera reacts to its window being rescaled. Comments are meant to gide you
     * into understanding how this is achieved, but you are expected to have already learned the basics of GDMBasics, if
     * you don't we recommend reading the comments on example_template folder's main.cpp file.
     */
    auto main_room = std::make_shared<Room>();
    auto game = Game::getGame(480, 360, "My Game Main Window - Rescale Mode", main_room);

    /*
     * We start by creating an element that will hold owr cameras.
     * Usually you'll probably want a different element for each camera, but for this example this will suffice.
     */
    auto my_camera_element = main_room->addElement();

    /* First, a RESCALE camera is one that will modify its aspect ratio when it's window is resized by stretching
     * everything that the camera can see, keeping the information on screen constant but deforming the player
     * perception of things in the world.
     */
    auto my_rescale_camera = my_camera_element->addComponent<Camera>();
    my_rescale_camera->setScaleType(Camera::RESCALE);
    game->setWindowSize(400, 400);
    game->setWindowPosition(500, 50);

    /* Secondly a REVEAL camera modifies its aspect ratio to match that of the window by showing more or less parts
     * of the game world. This allows to keep the form of the world objects but also reveals extra information
     * to players with bigger windows.
     */
    auto my_reveal_camera = my_camera_element->addComponent<Camera>();
    /* NOTE: we create a different window for the second and third cameras for a better visualization, but multiple
     * cameras on a single window is actually possible even with cameras with different scale types.
     */
    uint second_window_id = my_reveal_camera->useNewTarget("My Game Second Window - Reveal Mode");
    my_reveal_camera->setScaleType(Camera::REVEAL);
    game->setWindowSize(400, 400, second_window_id);
    game->setWindowPosition(0, 50, second_window_id);

    /*
     * Lastly a Letterbox camera keeps its aspect ratio constant (unless "Camera::setSize" it's called) by adding black
     * lines on the extra space on the window. This will keep both the form of the world and the information on screen
     * constant for all players.
     */
    auto my_letterbox_camera = my_camera_element->addComponent<Camera>();
    uint third_window_id = my_letterbox_camera->useNewTarget("My Game Third Window - Letterbox Mode");
    my_letterbox_camera->setScaleType(Camera::LETTERBOX);
    game->setWindowSize(400, 400, third_window_id);
    game->setWindowPosition(1000, 50, third_window_id);

    /*
     * Now we will just generate some sprites so you can better appreciate the workings of the different cameras.
     */
    auto my_sprite_element = main_room->addElement();
    for (int i = -5; i < 5; i++)
    {
        for (int j = -5; j < 5; j++)
        {
            /*
             * Most of this sprites will be out of screen for all our windows. The REVEAL camera will be the only one
             * able to show all the circles on the window if you resize it enough. All the others will only show the
             * same amount no matter what you do to the window.
             */
            auto my_sprite = my_sprite_element->addComponent<Sprite>();
            my_sprite->setTexture("resources/Circle.png");
            my_sprite->setColor(random(), random(), random(), 255);
            my_sprite->offset.rect_bounds.left = (float)i * 150 - 30;
            my_sprite->offset.rect_bounds.top = (float)j * 150 - 30;
            // We have to add the sprites to the cameras, so they are visible on all of them.
            // You can try commenting one of these lines to see how you can hide sprites to certain cameras.
            my_rescale_camera->addSprite(my_sprite);
            my_reveal_camera->addSprite(my_sprite);
            my_letterbox_camera->addSprite(my_sprite);
        }
    }

    /*
     * And we will also add a nice green background, so you can appreciate the black lines that the LETTERBOX camera
     * will add on the extra space when resizing. Note that the lines could either be on the sides of the window or
     * on it's top and bottom depending on the shape you give to the window.
     */
    auto my_background_element = main_room->addElement();
    my_background_element->setScale(200, 200);
    my_background_element->setPosition(-5000, -5000);
    my_background_element->depth = -50;

    auto my_background_sprite = my_background_element->addComponent<Sprite>();
    my_background_sprite->setTexture("resources/Square.png");
    my_background_sprite->setColor(77, 152, 79, 255);

    my_rescale_camera->addSprite(my_background_sprite);
    my_reveal_camera->addSprite(my_background_sprite);
    my_letterbox_camera->addSprite(my_background_sprite);

    return game;
}
} // namespace mate
