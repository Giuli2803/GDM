//
// Created by elly_sparky on 01/01/24.
//

#ifndef GDMATE_GDMBASICS_H
#define GDMATE_GDMBASICS_H

#include <SFML/Graphics.hpp>
#include <utility>
#include <cmath>
#include <list>
#include <memory>
#include <iostream>
#include <string>
#include "LocalCoords.h"

namespace mate {
    class Element;
    class Component;
    class Trigger;
    class TriggerShooter;

    enum ShapeType{
        RECTANGLE,
        CIRCLE
        //Todo: TRIANGLE?
    };

    struct ord_sprite{
        sf::Sprite sprite;
        double depth = 0;

        bool operator<(ord_sprite& a) const{
            return depth < a.depth;
        }
    };

    struct render_target{
        //Todo: Use an sf::RenderTarget instead of a _target

        // The target contains the dimensions of the window where the  sprites will be printed
        std::unique_ptr<sf::RenderWindow> target{};
        // List of sprites on the scene
        std::list<std::weak_ptr<ord_sprite>> printQueue;
        const u_int id;

        render_target() : id(generate_id()) {}

        void RemoveSprite(const std::shared_ptr<ord_sprite>& sprite){
            printQueue.remove_if([&sprite](const std::weak_ptr<ord_sprite>& weak_sprite){
                return weak_sprite.lock() == sprite;
            });
        }
    private:
        static int generate_id(){
            static int unique_id = 0;
            return unique_id++;
        }
    };

    class Room: public mate::LocalCoords {
    private:
        std::list< std::shared_ptr<Element> > _elements;
    public:
        ///Simple methods
        //Constructors
        Room() = default;

        //Elements creation / destruction confirmation
        [[maybe_unused]]
        unsigned long getElementsCount(){ return _elements.size(); }

        ///Methods declarations
        //Element stuff
        [[maybe_unused]]
        void AddElement(std::shared_ptr<Element> element);
        std::shared_ptr<Element> AddElement();

        //Loops
        void DataLoop();
        void RenderLoop();

        //Events
        void ResizeEvent();
    };

    template<class T>
    concept valid_component = std::is_base_of<Component, T>::value &&
    requires(std::weak_ptr<Element> element) { T{element}; };

    class Component {
    public:
        explicit Component(std::weak_ptr<class Element> parent) : _parent(std::move(parent)) {}

        //Virtual methods
        virtual void Loop() = 0;
        virtual void RenderLoop() {};
        virtual void WindowResizeEvent() {};

    protected:
        std::weak_ptr<Element> _parent;
    };

    class Element: public mate::LocalCoords {
    private:
        // Shared pointer is used since other components might need to hold a reference, yet this should
        // be the only shared pointer to this particular components and elements, all others should be weak pointers
        // Todo: Find a way to use unique_ptr and references maybe.
        std::vector<std::shared_ptr<Component>> _components;
        std::list<std::shared_ptr<Element>> _elements;
        bool _destroy_flag = false;
    public:
        ///--------------------------- Constructors
        explicit Element(std::shared_ptr<LocalCoords> parent, sf::Vector2f position);
        Element() = default;

        [[maybe_unused]]
        unsigned long getElementsCount() const { return _elements.size(); }

        // Adds a new Component of the template type to the component list and returns it
        template<valid_component T>
        std::shared_ptr<T> addComponent() noexcept {
            auto new_component = std::make_shared<T>(std::dynamic_pointer_cast<Element>(shared_from_this()));
            _components.emplace_back(new_component);
            return new_component;
        }

        bool ShouldDestroy() const { return _destroy_flag; }

        //Other methods definitions
        std::shared_ptr<Element> AddChild();
        void Destroy();
        void Loop();
        void RenderLoop();
        void ResizeEvent();
        [[maybe_unused]]
        unsigned long getFullElementsCount();
    };

    class Trigger {
    private:
        mate::Bounds _offset = mate::Bounds();
        const std::weak_ptr<Element> _follow;
        const int id;
        bool _must_follow = false;
        bool should_remove = false;

        static int generate_id(){
            static int unique_id = 0;
            return unique_id++;
        }
    public:
        ShapeType shape = RECTANGLE;

        Trigger() : id(generate_id()){
            _offset = mate::Bounds();
        }
        explicit Trigger(const std::weak_ptr<Element>& follow, bool must_follow)
                : _follow(follow), id(generate_id()), _must_follow(must_follow) {
            _offset = mate::Bounds();
        }

        ~Trigger() = default;

        [[nodiscard]]int getID() const { return id; }

        void CheckRemove() {
            if (_must_follow && _follow.expired())
                should_remove = true;
        };

        [[nodiscard]] bool ShouldRemove() const { return should_remove; }
        void MarkForRemoval() { should_remove = true; }

        [[nodiscard]] sf::Vector2f getPosition() const {
            sf::Vector2f reference(0, 0);
            if (std::shared_ptr<Element> spt_parent = _follow.lock()){
                reference = spt_parent->getWorldPosition();
            }
            return _offset.getPositionBounds(reference);
        }

        [[nodiscard]] sf::Vector2f getDimensions() const {
            sf::Vector2f reference(1, 1);
            if (std::shared_ptr<Element> spt_parent = _follow.lock()){
                reference = spt_parent->getWorldScale();
            }
            return _offset.getDimensionBounds(reference);
        }

        void setPositionOffset(float left, float top) {
            _offset.rect_bounds.left = left;
            _offset.rect_bounds.top = top;
        }

        void setDimensionOffset(float width, float height){
            _offset.rect_bounds.width = width;
            _offset.rect_bounds.height = height;
        }

        virtual void TriggerIn() = 0;
    };

    class TriggerManager {
    private:
        std::list<std::unique_ptr<Trigger>> triggers;
    public:
        void AddTrigger(std::unique_ptr<Trigger> trig) {
            triggers.push_back(std::move(trig));
        }

        void RemoveTrigger(int trigger_id){
            for (const std::unique_ptr<Trigger>& trigger : triggers){
                if (trigger->getID() == trigger_id){
                    trigger->MarkForRemoval();
                }
            }
        }

        void Curate(){
            triggers.remove_if([](const std::unique_ptr<Trigger>& trigger) {
                return trigger->ShouldRemove();
            });
        }

        void CheckTrigger(ShapeType, const TriggerShooter&);
        static bool RectangleToRectangleCheck(sf::Vector2f, sf::Vector2f, sf::Vector2f, sf::Vector2f);
        static bool CircleToCircleCheck(sf::Vector2f, float, sf::Vector2f, float);
        static bool CircleToRectangleCheck(sf::Vector2f, float, sf::Vector2f, sf::Vector2f);
    };

    /**
     * Main game singleton class.
     */
    class Game{
    private:
        std::list<std::shared_ptr<Room>> _rooms;
        std::shared_ptr<Room> _active_room;
        render_target _main_render_target;
        std::list<render_target> _secondary_targets;
        TriggerManager _trigger_manager;
        static std::shared_ptr<Game> _instance;

        /**
         * Private constructor. Generates the window.
         */
        Game(){
            _main_render_target.target = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 400), "Game");
            _active_room = nullptr;
        };
    public:
        ///----------------non-copyable
        Game(Game &other) = delete;
        void operator=(const Game &) = delete;

        ///---------------------------------------------Simple methods----------------------------

        ///-----------------Window related stuff
        void setWindowView(sf::View view) const { _main_render_target.target->setView(view); }
        void setWindowView() const { _main_render_target.target->setView(_main_render_target.target->getDefaultView()); }
        sf::Vector2u getWindowSize() const { return _main_render_target.target->getSize(); }
        void setWindowSize(int x, int y) const {
            _main_render_target.target->setSize(sf::Vector2u (x, y));
        }

        ///-----------------Render Targets related stuff
        [[nodiscard]]
        u_int AddSecondaryTarget(sf::View view) {
            render_target new_target;
            u_int id = new_target.id;
            new_target.target = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 400), "Game: Second Window");
            new_target.target->setView(view);
            _secondary_targets.push_back(std::move(new_target));
            return id;
        }
        //void setSecondaryView()

        ///-----------------Rooms related stuff
        /**
         * Creates a shared pointer to store the new room on the room list
         * @param room Room to be added
         */
        void addRoom(std::shared_ptr<Room> room) {
            _rooms.push_back(std::move(room));
        }

        [[maybe_unused]]
        std::shared_ptr<Room> getMainRoom() {
            return _active_room;
        }

        ///------------------Sprites related stuff
        void AddSprite(const std::shared_ptr<ord_sprite>& sprite){
            _main_render_target.printQueue.push_back(sprite);
        }
        void RemoveSprite(const std::shared_ptr<ord_sprite>& sprite){
            _main_render_target.RemoveSprite(sprite);
        }

        ///------------------Trigger related stuff
        void AddTrigger(std::unique_ptr<Trigger> trigger){
            _trigger_manager.AddTrigger(std::move(trigger));
        }
        void RemoveTrigger(int trigger_id){
            _trigger_manager.RemoveTrigger(trigger_id);
        }
        void CheckTrigger(ShapeType shape, const TriggerShooter& shooter){
            _trigger_manager.CheckTrigger(shape, shooter);
        }

        ///-------------------------------Longer methods declarations-----------------------------

        ///--------------------Singleton getters

        static std::shared_ptr<Game> getGame();
        [[maybe_unused]]
        static std::shared_ptr<Game> getGame(int winWidth, int winHeight, const std::string &gameName, std::shared_ptr<Room>mainRoom);
        [[maybe_unused]]
        static std::shared_ptr<Game> getGame(int winWidth, int winHeight, const std::string &gameName, std::list<std::shared_ptr<Room>>& roomsList);

        ///-------------------Others

        //Todo: Switch rooms by using a unique ID given by the room itself.
        /**
         * Switch the active room
         * @param position of the desired room on the game rooms list
         */
        void switchRoom(int position);

        /**
         * Main game loop
         */
        [[noreturn]]
        void gameLoop(bool isTesting = false);
    };

    using game_instance = std::shared_ptr<Game>;

    game_instance Start();
}

#include "Sprite.h"
#include "InputActions.h"
#include "Camera.h"
#include "TriggerShooter.h"
#endif //GDMATE_GDMBASICS_H
