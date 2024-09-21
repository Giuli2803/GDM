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
    _children_loops.push_back(std::move(element));
}

std::shared_ptr<Element> Room::addElement()
{
    auto child_element = std::make_shared<Element>(shared_from_this(), getPosition());
    _children_loops.push_back(child_element);
    return std::move(child_element);
}

void Room::loop()
{
    for (const auto &child : _children_loops)
    {
        child->loop();
    }
    _children_loops.remove_if([](auto &child) { return child->shouldDestroy(); });
}

void Room::renderLoop()
{
    for (auto &element : _children_loops)
    {
        element->renderLoop();
    }
}

[[maybe_unused]] void Room::windowResizeEvent()
{
    for (auto &element : _children_loops)
    {
        element->windowResizeEvent();
    }
}
} // namespace mate
