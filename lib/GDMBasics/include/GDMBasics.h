/**
 * @brief declaration of Game, Room, Element, Component, Trigger and TriggerManager classes and
 * ord_sprite and render_target structures.
 * @file
 */

#ifndef GDMATE_GDMBASICS_H
#define GDMATE_GDMBASICS_H

#include "LocalCoords.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <utility>

namespace mate
{
class Element;
class Component;
class Trigger;
class TriggerShooter;
class TriggerManager;

/// Shapes supported for trigger detection
enum ShapeType
{
    RECTANGLE ///< Calculated from the top left corner + height/width
        ,
    CIRCLE ///< Calculated from center + radius [max dimension]
};

/**
 * @brief sf::Sprite but with an additional depth value for better ordering
 *
 * ord_sprite is a simple structure that adds a depth value to sf::Sprites. This value does not override the
 * LocalCoords depth value but it enhances it. when printing an sprite on the screen the game will first check the
 * LocalCoords depth value of the parent Element and then, if two or more Elements share the same depth value, the
 * ord_sprite depth will be used. If 2 ord_sprite have the same depth and their parent localCoords share the same depth
 * then the one that was created last is expected stay on top, but this is not ensured.
 */
struct ord_sprite
{
    sf::Sprite sprite;
    unsigned int depth = 0;

    bool operator<(ord_sprite &a) const
    {
        return depth < a.depth;
    }
};

/**
 * @brief Struct to asociate a unique ID value to all sf::RenderWindow.
 * This allows for better tracking of game windows.
 */
struct render_target
{
    // Todo: Use an sf::RenderTarget instead of an sf::RenderWindow
    // The target contains the dimensions of the window where the  sprites will be printed
    std::unique_ptr<sf::RenderWindow> target{};
    const u_int id;

    render_target() : id(generate_id())
    {
    }

  private:
    static int generate_id()
    {
        static int unique_id = 0;
        return unique_id++;
    }
};

class IDestroy
{
  protected:
    bool _destroy_flag = false;

  public:
    /**
     * When IDestroy is set for destruction the effect is not immediate, this is to avoid the destruction to occur
     * while a code thread is being executed (Like a sef-destruction case). shouldDestroy()
     * allows to now if IDestroy is going to be destroy less than a frame before the event occurs.
     *
     * @return true if IDestroy is marked for destruction.
     */
    [[nodiscard]] bool shouldDestroy() const
    {
        return _destroy_flag;
    }
    /**
     * @brief Marks the IDestroy for destruction.
     *
     * Marks the IDestroy for destruction. The removal itself is not immediate and can even not occur, the flag exists
     * to inform the object and other objects of the intention of being destroyed, but the deletion itself should be
     * managed separately.
     */
    virtual void destroy()
    {
        _destroy_flag = true;
    }
};

/**
 * @brief Interface. Defines all the "loop" and "event" methods for the building blocks of the game.
 *
 * A loop is defined as a method that is called every frame, meanwhile an event is caused by an external input like a
 * window being resized or closed.
 *
 * Note that ILoop should be inherited only by high authority objects, meaning that can only be contained by the Game
 * class and can't be destroyed. Low authority objects should use the ILowLoop interface instead.
 */
class ILoop
{
  public:
    /**
     * @brief Main loop method.
     *
     * loop() is called every frame for all active ILoops meaning that are directly or indirectly under the active room.
     * Implementations of loop() are expected to perform modifications on itself or other objects.
     */
    virtual void loop() = 0;
    /**
     * @brief Secondary loop method. For visual effects only.
     *
     * renderLoop() is called every frame after the loop() method and it's expected to not perform any modifications
     * on any object other than itself. Implementations of this method should only perform updates on visual
     * aspects using the modifications introduced by loop() as a guide.
     */
    virtual void renderLoop(){};
    /**
     * @brief Special method called when the window changes in size.
     *
     * This method is meant for visual's corrections based on window resizing.
     */
    virtual void windowResizeEvent(){};
};

/**
 * @brief Low authority ILoop object.
 * ILowLoops is an interface for objects that require the loop and event methods and are of low authority, meaning that
 * can be contained by things other than the Game class and can be destroyed.
 */
class ILowLoop : public IDestroy, public ILoop
{
};

/**
 * Verifies if a Component implementation has a constructor that takes as it's only parameter an Element.
 * @tparam T Class that inherits from Component.
 */
template <class T>
concept valid_component =
    std::is_base_of<Component, T>::value && requires(std::weak_ptr<Element> element) { T{element}; };

/**
 * @brief Abstract class for the implementation of special Element functionalities.
 */
class Component : public ILowLoop
{
  public:
    explicit Component(std::weak_ptr<class LocalCoords> parent) : _parent(std::move(parent))
    {
    }

  protected:
    std::weak_ptr<LocalCoords> _parent; ///< Element that contains the Component and controls it's destruction.
};

/**
 * @brief Main game entity.
 *
 * Element works mostly as a low authority implementation of LocalCoords, that is to say, it's expected to have a
 * parent LocalCoords object, yet Element is not limited to that position and can have global authority too.
 * What's really important about Element objects is their capability to hold Component objects to clearly
 * differentiate between Element instances.
 */
class Element : public mate::LocalCoords, public ILowLoop
{
  private:
    std::list<std::shared_ptr<ILowLoop>> _children;

  public:
    // Constructors
    explicit Element(const std::shared_ptr<LocalCoords> &parent) : LocalCoords(parent)
    {
    }

    explicit Element(const std::shared_ptr<LocalCoords> &parent, sf::Vector2f position) : LocalCoords(position, parent)
    {
    }

    explicit Element(const std::shared_ptr<LocalCoords> &parent, sf::Vector2f position, float rotation)
        : LocalCoords(position, rotation, parent)
    {
    }

    explicit Element(const std::shared_ptr<LocalCoords> &parent, sf::Vector2f position, sf::Vector2f scale)
        : LocalCoords(position, scale, parent)
    {
    }

    explicit Element(const std::shared_ptr<LocalCoords> &parent, sf::Vector2f position, sf::Vector2f scale,
                     float rotation)
        : LocalCoords(position, scale, rotation, parent)
    {
    }

    /**
     * Default constructor is meant for high priority Element objects, if the Element is meant to have a parent on
     * creation use one of the specific constructors for that.
     */
    Element() = default;

    [[maybe_unused]] unsigned long getElementsCount() const;

    // Adds a new Component of the template type to the component list and returns it
    /**
     * @brief Generates a new Component.
     *
     * The Component is of the specified class and the parent of the Component is set as the calling Element instance.
     * @tparam T Component implementation that complies with valid_component
     * @return Reference to the generated Component.
     */
    template <valid_component T> std::shared_ptr<T> addComponent() noexcept
    {
        auto new_component = std::make_shared<T>(std::dynamic_pointer_cast<Element>(shared_from_this()));
        _children.emplace_back(new_component);
        return new_component;
    }

    // Other methods definitions
    /**
     * Generates a new basic Element with the parent set to the calling Element instance.
     * @return A reference to the new Element.
     */
    std::shared_ptr<Element> addChild();
    /**
     * @brief Marks the Element for destruction.
     *
     * All children Element objects will also de marked for destruction. The removal itself can take up to a frame to
     * be effective, but the loop methods of the Component objects of an Element that's marked for removal are not
     * expected to be called. Furthermore, if an Element is marked for destruction by a Component of the same Element,
     * the loop() method will not be called for the remaining Component objects, so Component's with this capability
     * should be added as the first Component of an Element for efficiency. If an Element mark's it's parent for removal
     * all other children object's loop() method will be called yet those are expected to run Component and Element
     * purging only to avoid any possible unintended persistence.
     */
    void destroy() override;
    /**
     * @brief Main Element loop method.
     *
     * The loop() method of the children Elements and Component objects are called. The order is Components first
     * Elements second, so if an Element mark's itself or one of it's children for destruction via a Component, there
     * won't be unnecessary loop() calls performed. If the element is marked for removal it will destroy all of it's
     * children Elements and Component objects.
     */
    void loop() override;
    /**
     * @brief Secondary loop method, meant for visual changes only.
     *
     * renderLoop() does not perform checks for an Element destruction, therefore Element destruction should only be
     * performed on the loop() methods.
     */
    void renderLoop() override;
    /**
     * @brief Communicates to all the children and Components that a window has changed in size.
     */
    void windowResizeEvent() override;
    /**
     * @return All the related Element under it's authority (children + children's children + etc).
     */
    [[maybe_unused]] unsigned long getFullElementsCount();
};

/**
 * @brief Superposition detection.
 *
 * Trigger is an abstract class for superposition detection. Trigger implementations perform an specific action when
 * a TriggerShooter gets on top of them. Currently Trigger does not support for depth, but are expected to do so in the
 * near future.
 */
class Trigger : public LocalCoords, public IDestroy
{
  private:
    const int id;

    static int generateId()
    {
        static int unique_id = 0;
        return unique_id++;
    }

  public:
    ShapeType shape = RECTANGLE; ///< Trigger Shape

    Trigger() : id(generateId())
    {
    }
    /**
     * A Trigger can be set to follow an Element.
     * @param follow Element to follow.
     * @param must_follow Should the Trigger be destroy when the Element gets removed?
     */
    explicit Trigger(const std::shared_ptr<LocalCoords> &parent) : LocalCoords(parent), id(generateId())
    {
    }

    [[nodiscard]] int getID() const
    {
        return id;
    }

    /**
     * Abstract method. This method's implementations are called when a TriggerShooter gets inside the bounds of
     * the Trigger instance.
     */
    virtual void triggerIn() = 0;
};

/**
 * @brief Trigger superposition calculations.
 *
 * TriggerManager keeps track of all active Triggers and performs the calculations for Trigger superposition when a
 * TriggerShooter requires it.
 */
class TriggerManager
{
  private:
    std::list<std::unique_ptr<Trigger>> triggers;

  public:
    void addTrigger(std::unique_ptr<Trigger> trig)
    {
        triggers.push_back(std::move(trig));
    }

    void removeTrigger(int trigger_id);

    /**
     * Removes all Triggers that are marked for removal.
     */
    void curateTriggers()
    {
        triggers.remove_if([](const std::unique_ptr<Trigger> &trigger) { return trigger->shouldDestroy(); });
    }

    /**
     * Checks if the TriggerShooter is within any of the active Triggers.
     */
    virtual void checkTrigger(ShapeType shape, const TriggerShooter &shooter);
    static bool rectangleToRectangleCheck(sf::Vector2f rect1_pos, sf::Vector2f rect1_dim, sf::Vector2f rect2_pos,
                                          sf::Vector2f rect2_dim);
    static bool circleToCircleCheck(sf::Vector2f circ1_pos, float circ1_rad, sf::Vector2f circ2_pos, float circ2_rad);
    static bool circleToRectangleCheck(sf::Vector2f circ_pos, float radius, sf::Vector2f rect_pos,
                                       sf::Vector2f rect_dim);

#ifdef GDM_TESTING_ENABLED
    uint getListCount()
    {
        return triggers.size();
    }
    bool triggerIsContained(int trigger_id)
    {
        for (auto &trigger : triggers)
        {
            if (trigger->getID() == trigger_id)
            {
                return true;
            }
        }
        return false;
    }
#endif
};

/**
 * @brief Highest authority LocalCoords object.
 *
 * A Room works like a "folder" for all the Element objects on a Game, allowing for easy switching between radically
 * different aspects of the game by simply changing the selected Room. For example different Rooms might represent
 * different levels, scenes or menu windows, so the Game object can switch between this by simply selecting a different
 * Room that already contains all the data of the Elements involved.
 */
class Room : public mate::LocalCoords, public TriggerManager, public ILoop
{
  private:
    std::list<std::shared_ptr<ILowLoop>> _children_loops; ///< Elements within the Room.

  public:
    // Constructors
    Room() = default;

#ifdef GDM_TESTING_ENABLED
    template <class T> unsigned long getLoopTypeCount()
    {
        ulong count = 0;
        for (const auto &loop : _children_loops)
        {
            if (std::dynamic_pointer_cast<T>(loop))
            {
                count++;
            }
        }
        return count;
    }
#endif
    /**
     * @brief Adds a preexisting Element to the Room.
     *
     * This allows for an Element to move between Rooms. When doing so, the Element position coordinates must be taken
     * in account, this responsibility lays on the user.
     * @param element Element to be added
     */
    [[maybe_unused]] void addElement(std::shared_ptr<Element> element);
    /**
     * @brief Generates a new Element with the Room as it's parent and returns a reference to it.
     * @return New Element added to the Room.
     */
    std::shared_ptr<Element> addElement();

    // Loops

    /**
     * @brief Communicates to all the Element objects within the Room that they should run their main loop functions.
     *
     * After all the child Element objects have run their main loops, the Element list is purged to effectively
     * destroy any Element objects that are marked for removal.
     */
    void loop() override;

    /**
     * @brief Communicates to all the contained Element objects to run their respective renderLoop method.
     */
    void renderLoop() override;

    // Events

    /**
     * @brief Communicates to all the contained Element objects that a resize event has occur.
     *
     * This means that an sf::RenderWindow has changed in size.
     */
    void windowResizeEvent() override;
};

/**
 * @brief Main game singleton class.
 *
 * Game contains all of the Room objects from the game, runs the loop() method of the active one, tracks the window(s)
 * and keeps the TriggerManager.
 */
class Game : public TriggerManager
{
  private:
    std::list<std::shared_ptr<Room>> _rooms;
    std::shared_ptr<Room> _active_room;
    render_target _main_render_target;
    std::list<render_target> _secondary_targets;
    static std::shared_ptr<Game> _instance;

    /**
     * Private constructor. Generates the window.
     */
    Game()
    {
        _main_render_target.target = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 400), "Game");
        _active_room = nullptr;
    };

  public:
    Game(Game &other) = delete;
    void operator=(const Game &) = delete;

    // Simple methods

    // Window related stuff
    void setWindowView(sf::View view_, u_int id_) const;
    /**
     * Sets the main window's view to the default view.
     */
    void setWindowView() const
    {
        _main_render_target.target->setView(_main_render_target.target->getDefaultView());
    }

#ifdef GDM_TESTING_ENABLED
    [[nodiscard]] sf::View getView(u_int id_) const
    {
        if (id_ == 0)
        {
            return _main_render_target.target->getView();
        }
        for (auto &target : _secondary_targets)
        {
            if (target.id == id_)
            {
                return target.target->getView();
            }
        }
        return {};
    }
#endif

    [[nodiscard]] sf::Vector2i getWindowPosition(uint id_ = 0) const;
    void setWindowPosition(int x, int y, uint id_ = 0) const;
    [[nodiscard]] sf::Vector2u getWindowSize(uint id_ = 0) const;
    void setWindowSize(int x, int y, uint id_ = 0) const;

    /**
     * Prints a sprite on a selected render target (window).
     * @param id_ id value of the render_target to be used.
     */
    void draw(const std::shared_ptr<const ord_sprite> &sprite_, u_int id_);

    // Render Targets related stuff
    /**
     * Generates a new window with the desired view.
     * @param view_ sf::View of the new window.
     * @return id value of the new window.
     */
    [[nodiscard]] u_int addSecondaryTarget(sf::View view_, const std::string &title);

    // Rooms related stuff
    [[maybe_unused]] void addRoom(std::shared_ptr<Room> room)
    {
        _rooms.push_back(std::move(room));
    }

    [[maybe_unused]] std::shared_ptr<Room> addRoom()
    {
        auto room = std::make_shared<Room>();
        _rooms.push_back(room);
        return std::move(room);
    }

    [[maybe_unused]] std::shared_ptr<Room> getActiveRoom()
    {
        return _active_room;
    }

    // Trigger related stuff
    void addRoomTrigger(std::unique_ptr<Trigger> trigger)
    {
        _active_room->addTrigger(std::move(trigger));
    }
    void removeRoomTrigger(int trigger_id)
    {
        _active_room->removeTrigger(trigger_id);
    }
    void checkTrigger(ShapeType shape, const TriggerShooter &shooter) override
    {
        TriggerManager::checkTrigger(shape, shooter);
        _active_room->checkTrigger(shape, shooter);
    }

    // Longer methods declarations

    // Singleton getters

    static std::shared_ptr<Game> getGame();
    /**
     * Generates a new Game object with the desired parameters only if there isn't an already existing Game.
     * @param win_width_ width of the main game window.
     * @param win_height_ height of the main game window.
     * @param game_name_ title to be displayed on the main game window.
     * @param main_room_ pre-existing main Room.
     * @return Game object.
     */
    [[maybe_unused]] static std::shared_ptr<Game> getGame(int win_width_, int win_height_,
                                                          const std::string &game_name_,
                                                          std::shared_ptr<Room> main_room_);
    /**
     * Generates a new Game object with the desired parameters only if there isn't an already existing Game.
     * @param win_width_ width of the main game window.
     * @param win_height_ height of the main game window.
     * @param game_name_ title to be displayed on the main game window.
     * @param rooms_list_ list of pre-existing rooms.
     * @return Game object.
     */
    [[maybe_unused]] static std::shared_ptr<Game> getGame(int win_width_, int win_height_,
                                                          const std::string &game_name_,
                                                          std::list<std::shared_ptr<Room>> &rooms_list_);

    // Others

    // Todo: Switch rooms by using a unique ID given by the room itself.
    /**
     * Switch the active room
     * @param position_ of the desired room on the game rooms list
     */
    void switchRoom(int position_);

    /**
     * Main game loop
     */
    [[noreturn]] void gameLoop();

    void runSingleFrame();
};

using game_instance = std::shared_ptr<Game>;

/**
 * Implement your game declaration/generation on this function, Then call this function from your main function and call
 * gameLoop on the return value to start the game.
 * @return Game object with the desired content.
 */
game_instance start();
} // namespace mate

#include "Camera.h"
#include "InputActions.h"
#include "Sprite.h"
#include "TriggerShooter.h"
#endif // GDMATE_GDMBASICS_H
