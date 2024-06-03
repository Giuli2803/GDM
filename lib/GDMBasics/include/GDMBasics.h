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
        //Todo: Use an mate::RenderTarget instead of a _target
        std::shared_ptr<sf::RenderWindow> target{};
        std::list<ord_sprite*> printQueue;
    };

    class TriggerManager {
    private:
        std::list<Trigger*> triggers;
    public:
        void AddTrigger(Trigger *const trig) {
            triggers.push_back(trig);
        }

        void RemoveTrigger(Trigger *const trig){
            triggers.remove(trig);
        }

        void CheckTrigger(mate::Bounds, ShapeType, Element&);
        static bool RectangleToRectangleCheck(sf::Vector2f, sf::Vector2f, sf::Vector2f, sf::Vector2f);
        static bool CircleToCircleCheck(sf::Vector2f, float, sf::Vector2f, float);
        static bool CircleToRectangleCheck(sf::Vector2f, float, sf::Vector2f, sf::Vector2f);
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
        Element& AddElement();

        //Loops
        void DataLoop();
        void RenderLoop();

        //Events
        void ResizeEvent();
    };

    class Element: public mate::LocalCoords {
    private:
        std::list<std::unique_ptr<Component>> _components;
        std::list<std::unique_ptr<Element>> _elements;
        bool _destroy_flag = false;
    public:
        ///Constructors
        explicit Element(LocalCoords*);
        explicit Element(Room*);
        explicit Element(Element*);

        ///Simple methods
        [[maybe_unused]]
        unsigned long getElementsCount(){ return _elements.size(); }

        //Adds a new Component of the template type to the component list and returns it
        template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
        T& addComponent() {
            _components.push_back(std::make_unique<T>(this));
            return *dynamic_cast<T*>(_components.back().get());
        }

        template<class T, typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
        T* getComponent() const {
            for(const auto& component : _components){
                if(auto ptr = dynamic_cast<T*>(component.get()))
                    return ptr;
            }
            return nullptr;
        }

        bool ShouldDestroy() const { return _destroy_flag; }

        //Other methods definitions
        Element& AddChild();
        void Destroy();
        void Loop();
        void RenderLoop();
        void ResizeEvent();
        [[maybe_unused]]
        unsigned long getFullElementsCount();
    };

    class Component {
    protected:
        Element *_element;
    public:
        //Constructor
        explicit Component(Element &element){
            _element = &element;
        };
        virtual ~Component() = default;

        //Virtual methods
        virtual void Loop() = 0;
        virtual void RenderLoop() {}
        virtual void WindowResizeEvent() {}
    };

    class Game{
    private:
        std::list<std::shared_ptr<Room>> _rooms;
        std::shared_ptr<Room> _active_room;
        render_target _target;
        TriggerManager _trigger_manager;
        static std::shared_ptr<Game> _instance;

        //Todo: Pass window name as an argument.
        /**
         * Private constructor. Generates the window.
         */
        Game(){
            _target.target = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 400), "Game");
            _active_room = nullptr;
        };
    public:
        ///----------------non-copyable
        Game(Game &other) = delete;
        void operator=(const Game &) = delete;

        ///---------------------------------------------Simple methods----------------------------

        ///-----------------Window related stuff

        [[nodiscard]]
        std::shared_ptr<sf::RenderWindow> getWindow() const { return _target.target; }
        render_target* getWindowTarget() { return &_target; }
        [[maybe_unused]]
        void setWindowSize(int x, int y) const {
            _target.target->setSize(sf::Vector2u (x, y));
        }

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

        ///------------------Trigger related stuff
        //Todo: revision

        void AddTrigger(Trigger *const trigger){
            _trigger_manager.AddTrigger(trigger);
        }
        void RemoveTrigger(Trigger *const trigger){
            _trigger_manager.RemoveTrigger(trigger);
        }
        //Todo: Deprecate
        TriggerManager* getTriggerManager(){
            return &_trigger_manager;
        }

        ///-------------------------------Longer methods declarations-----------------------------

        ///--------------------Singleton getters

        static std::shared_ptr<Game> getGame();
        [[maybe_unused]]
        static std::shared_ptr<Game> getGame(int winWidth, int winHeight, const std::string &gameName, std::shared_ptr<Room>mainRoom);
        [[maybe_unused]]
        static std::shared_ptr<Game> getGame(int winWidth, int winHeight, const std::string &gameName, std::list<std::shared_ptr<Room>>& roomsList);

        ///-------------------Others

        [[maybe_unused]]

        //Todo: Switch rooms by using a unique ID given by the room itself.
        /**
         * Switch the active room
         * @param position of the desired room on the game rooms list
         */
        void switchRoom(int position);

        /**
         * Main game loop
         */
        void gameLoop(bool isTesting = false);
    };
}

#include "Sprite.h"
#include "InputActions.h"
#include "Camera.h"
#include "Trigger.h"

#endif //GDMATE_GDMBASICS_H
