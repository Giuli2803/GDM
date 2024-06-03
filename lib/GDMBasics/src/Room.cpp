//
// Created by elly_sparky on 01/01/24.
//

#include "GDMBasics.h"

namespace mate {
    [[maybe_unused]]
    void Room::AddElement(std::shared_ptr<Element> element) {
        if(element->getParent())
            element->setParent(this);
        _elements.push_back(std::move(element));
    }

    Element& Room::AddElement() {
        _elements.push_back(std::make_shared<Element>(this));
        return *_elements.back();
    }

    void Room::DataLoop() {
        for(auto &element : _elements)
        {
            element->Loop();
        }

        _elements.remove_if([](auto& element){
            return element->ShouldDestroy();
        });
    }

    void Room::RenderLoop() {
        for(auto &element : _elements)
        {
            element->RenderLoop();
        }
    }

    [[maybe_unused]]
    void Room::ResizeEvent() {
        for(auto &element : _elements)
        {
            element->ResizeEvent();
        }
    }
} // mate