//
// Created by elly_sparky on 01/01/24.
//

#include "GDMBasics.h"

namespace mate
{
std::shared_ptr<Element> Element::addChild()
{
    auto child = std::make_shared<Element>(shared_from_this(), getPosition());
    _elements.push_back(child);
    return std::move(child);
}

void Element::destroy()
{
    _destroy_flag = true;
    for (auto &element : _elements)
    {
        element->destroy();
    }
}

void Element::loop()
{
    _components.remove_if([](auto &component) { return component->shouldDestroy(); });

    if (!_destroy_flag)
    {
        for (const auto &component : _components)
        {
            component->loop();
            if (_destroy_flag)
                break;
        }
    }

    for (auto &element : _elements)
    {
        element->loop();
    }

    if (_destroy_flag)
    {
        _components.clear();
        _elements.clear();
    }

    _elements.remove_if([](auto &element) { return element->shouldDestroy(); });
}

void Element::renderLoop()
{
    for (auto &element : _elements)
    {
        element->renderLoop();
    }

    for (const auto &component : _components)
    {
        component->renderLoop();
    }
}

void Element::windowResizeEvent()
{
    for (auto &element : _elements)
    {
        element->windowResizeEvent();
    }

    for (const auto &component : _components)
    {
        component->windowResizeEvent();
    }
}

unsigned long Element::getFullElementsCount()
{
    unsigned long count = _elements.size();
    for (auto &element : _elements)
    {
        count += element->getFullElementsCount();
    }
    return count;
}
} // namespace mate
