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
    auto child_element = std::make_shared<Element>(shared_from_this(), getPosition());
    _elements.push_back(child_element);
    return std::move(child_element);
}

void Room::loop()
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

[[maybe_unused]] void Room::windowResizeEvent()
{
    for (auto &element : _elements)
    {
        element->windowResizeEvent();
    }
}
} // namespace mate
