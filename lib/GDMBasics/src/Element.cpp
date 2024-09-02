//
// Created by elly_sparky on 01/01/24.
//

#include "GDMBasics.h"

namespace mate
{
std::shared_ptr<Element> Element::addChild()
{
    auto child = std::make_shared<Element>(shared_from_this(), getPosition());
    _children.push_back(child);
    return std::move(child);
}

void Element::destroy()
{
    _destroy_flag = true;
    for (auto &child : _children)
    {
        if(auto element = std::dynamic_pointer_cast<Element>(child)){
            element->destroy();
        } else
        {
            child->destroy();
        }
    }
}

void Element::loop()
{
    if (!_destroy_flag)
    {
        for (const auto &child : _children)
        {
            child->loop();
            if (_destroy_flag)
                break;
        }
    }

    if (_destroy_flag)
    {
        _children.clear();
    }

    _children.remove_if([](auto &child) { return child->shouldDestroy(); });
}

void Element::renderLoop()
{
    for (auto &element : _children)
    {
        element->renderLoop();
    }

    for (const auto &component : _children)
    {
        component->renderLoop();
    }
}

void Element::windowResizeEvent()
{
    for (auto &element : _children)
    {
        element->windowResizeEvent();
    }

    for (const auto &component : _children)
    {
        component->windowResizeEvent();
    }
}

unsigned long Element::getElementsCount() const
{
    ulong count = 0;
    for (const auto &child : _children)
    {
        if (std::dynamic_pointer_cast<Element>(child))
        {
            ++count;
        }
    }
    return count;
}

unsigned long Element::getFullElementsCount()
{
    unsigned long count = 0;
    for (const auto &child : _children)
    {
        if (auto element = std::dynamic_pointer_cast<Element>(child))
        {
            ++count;
            count += element->getFullElementsCount();
        }
    }
    return count;
}
} // namespace mate
