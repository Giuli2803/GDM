//
// Created by elly_sparky on 01/01/24.
//

#include "GDMBasics.h"

namespace mate {
    Element::Element(std::shared_ptr<LocalCoords>parent, sf::Vector2f position)
    : LocalCoords(position, std::move(parent)) {}

    std::shared_ptr<Element> Element::AddChild() {
        auto child = std::make_shared<Element>(shared_from_this(), getPosition());
        _elements.push_back(child);
        return child;
    }

    void Element::Destroy(){
        _destroy_flag = true;
        for(auto& element : _elements){
            element->Destroy();
        }
    }

    void Element::Loop() {
        if(!_destroy_flag){
            for(const auto &component: _components){
                if(_destroy_flag)
                    break;
                component->Loop();
            }
        }

        for(auto &element : _elements){
            element->Loop();
        }

        if(_destroy_flag){
            _components.clear();
            _elements.clear();
        }

        _elements.remove_if([](auto& element) {
            return element->ShouldDestroy();
        });
    }

    void Element::RenderLoop(){
        for(auto &element : _elements){
            element->RenderLoop();
        }

        for(const auto &component : _components){
            component->RenderLoop();
        }
    }

    void Element::ResizeEvent() {
        for(auto &element : _elements){
            element->ResizeEvent();
        }

        for(const auto &component : _components){
            component->WindowResizeEvent();
        }
    }

    unsigned long Element::getFullElementsCount(){
        unsigned long count = _elements.size();
        for(auto &element : _elements){
            count += element->getFullElementsCount();
        }
        return count;
    }
} // mate
