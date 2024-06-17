//
// Created by elly_sparky on 02/01/24.
//

#ifndef GDMATE_LOCALCOORDS_H
#define GDMATE_LOCALCOORDS_H

#include <SFML/Graphics.hpp>
#include <memory>

namespace mate {
class LocalCoords : public sf::Transformable, public std::enable_shared_from_this<LocalCoords> {
    private:
        std::shared_ptr<LocalCoords> _parent;
        int _depth = 0;
    public:
        ///----------------------------- Constructors
        [[maybe_unused]]
        LocalCoords();

        [[maybe_unused]]
        explicit LocalCoords(sf::Vector2f position, std::shared_ptr<LocalCoords> parent);
        [[maybe_unused]]
        explicit LocalCoords(sf::Vector2f, float, std::shared_ptr<LocalCoords> parent);
        [[maybe_unused]]
        explicit LocalCoords(sf::Vector2f, sf::Vector2f, float, std::shared_ptr<LocalCoords> parent);

        ///---------------------------------- Getters
        sf::Vector2f getWorldPosition();
        sf::Vector2f getWorldScale();
        float getWorldRotation() const;
        std::shared_ptr<LocalCoords> getParent() const { return _parent; }
        int getDepth() const { return _depth; }

        ///----------------------------------- Setters
        void setDepth(int depth) { _depth = depth; }
        void setParent(std::shared_ptr<LocalCoords> parent) { _parent = std::move(parent); }
    };

    class Bounds {
        std::shared_ptr<LocalCoords> _parent;
    public:
        sf::Rect<float> rect_bounds;

        Bounds() = default;

        explicit Bounds(std::shared_ptr<LocalCoords> parent){
            rect_bounds.left = 0;
            rect_bounds.top = 0;
            rect_bounds.width = 1;
            rect_bounds.height = 1;
            _parent = std::move(parent);
        }

        sf::Vector2f getPositionBounds(){
            sf::Vector2f pos = _parent->getWorldPosition();
            pos.x += rect_bounds.left;
            pos.y += rect_bounds.top;
            return pos;
        }

        sf::Vector2f getDimensionBounds(){
            sf::Vector2f dim = _parent->getWorldScale();
            dim.x *= rect_bounds.width;
            dim.y *= rect_bounds.height;
            return dim;
        }
    };

} // mate

#endif //GDMATE_LOCALCOORDS_H
