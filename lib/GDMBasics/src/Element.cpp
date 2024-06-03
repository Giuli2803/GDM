//
// Created by elly_sparky on 01/01/24.
//

#include "GDMBasics.h"

namespace mate {
    Element::Element(LocalCoords* pParent)
    : LocalCoords(pParent) {}

    Element::Element(mate::Room* pParent)
    : LocalCoords(pParent) {}

    Element::Element(mate::Element* pParent)
    : LocalCoords(pParent) {}

    Element& Element::AddChild() {
        _elements.push_back(std::make_unique<Element>(this));
        return *_elements.back();
    }

    void Element::Destroy(){
        _destroy_flag = true;
        for(auto& element : _elements){
            element->Destroy();
        }
    }

    void Element::Loop() {
        if(!_destroy_flag){
            for(auto &component: _components){
                if(_destroy_flag)
                    break;
                component->Loop();
            }
        }

        for(auto &element : _elements){
            element->Loop();
        }

        if(_destroy_flag){
            std::cout << _components.size() << _elements.size() << std::endl;
            _components.clear();
            _elements.clear();
        } else {
            _elements.remove_if([](auto& element) {
                return element->ShouldDestroy();
            });
        }
    }

    void Element::RenderLoop(){
        for(auto &element : _elements){
            element->RenderLoop();
        }

        for(auto &component : _components){
            component->RenderLoop();
        }
    }

    void Element::ResizeEvent() {
        for(auto &element : _elements){
            element->ResizeEvent();
        }

        for(auto &component : _components){
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