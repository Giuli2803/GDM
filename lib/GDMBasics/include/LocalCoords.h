//
// Created by elly_sparky on 02/01/24.
//

#ifndef GDMATE_LOCALCOORDS_H
#define GDMATE_LOCALCOORDS_H

#include <SFML/Graphics.hpp>
#include <memory>

namespace mate
{
template <typename T> bool weakPtrIsUninitialized(std::weak_ptr<T> const &weak)
{
    using wt = std::weak_ptr<T>;
    return !weak.owner_before(wt{}) && !wt{}.owner_before(weak);
}

class Bounds
{
  public:
    sf::Rect<float> rect_bounds;

    explicit Bounds()
    {
        rect_bounds.left = 0;
        rect_bounds.top = 0;
        rect_bounds.width = 1;
        rect_bounds.height = 1;
    }

    [[nodiscard]] sf::Vector2f getPositionBounds(sf::Vector2f pos) const
    {
        pos.x += rect_bounds.left;
        pos.y += rect_bounds.top;
        return pos;
    }

    [[nodiscard]] sf::Vector2f getDimensionBounds(sf::Vector2f scale) const
    {
        scale.x *= rect_bounds.width;
        scale.y *= rect_bounds.height;
        return scale;
    }
};

class LocalCoords : public sf::Transformable, public std::enable_shared_from_this<LocalCoords>
{
  private:
    std::weak_ptr<LocalCoords> _parent;
    int _depth = 0;

  public:
    ///----------------------------- Constructors
    [[maybe_unused]] LocalCoords();

    [[maybe_unused]] explicit LocalCoords(sf::Vector2f position, const std::shared_ptr<LocalCoords> &parent);
    [[maybe_unused]] explicit LocalCoords(sf::Vector2f, float, const std::shared_ptr<LocalCoords> &parent);
    [[maybe_unused]] explicit LocalCoords(sf::Vector2f, sf::Vector2f, float,
                                          const std::shared_ptr<LocalCoords> &parent);

    ///---------------------------------- Getters
    sf::Vector2f getWorldPosition();
    sf::Vector2f getWorldScale();
    float getWorldRotation() const;
    std::weak_ptr<LocalCoords> getParent() const
    {
        return _parent;
    }
    int getDepth() const
    {
        return _depth;
    }

    ///----------------------------------- Setters
    void setDepth(int depth)
    {
        _depth = depth;
    }
    void setParent(const std::weak_ptr<LocalCoords> &parent)
    {
        _parent = parent;
    }
};
} // namespace mate

#endif // GDMATE_LOCALCOORDS_H
