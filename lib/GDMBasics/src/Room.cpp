//
// Created by elly_sparky on 01/01/24.
//

#include "GDMBasics.h"

namespace mate
{
[[maybe_unused]] void Room::addElement(std::shared_ptr<Element> element)
{
    if (weakPtrIsUninitialized(element->getParent()))
        element->setParent(shared_from_this());
    _elements.push_back(std::move(element));
}

std::shared_ptr<Element> Room::addElement()
{
    _elements.push_back(std::move(std::make_shared<Element>(shared_from_this(), getPosition())));
    return _elements.back();
}

void Room::dataLoop()
{

    for (auto &element : _elements)
    {
        element->loop();
    }

    _elements.remove_if([](auto &element) { return element->shouldDestroy(); });
}

void Room::renderLoop()
{
    for (auto &element : _elements)
    {
        element->renderLoop();
    }
}

[[maybe_unused]] void Room::resizeEvent()
{
    for (auto &element : _elements)
    {
        element->resizeEvent();
    }
}
} // namespace mate
