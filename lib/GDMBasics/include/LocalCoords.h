//
// Created by elly_sparky on 02/01/24.
//

#ifndef GDMATE_LOCALCOORDS_H
#define GDMATE_LOCALCOORDS_H

#include <SFML/Graphics.hpp>

namespace mate {
class LocalCoords : public sf::Transformable {
    private:
        LocalCoords *_parent;
        int _depth = 0;
    public:
        [[maybe_unused]]
        LocalCoords();
        explicit LocalCoords(LocalCoords*);
        [[maybe_unused]]
        explicit LocalCoords(sf::Vector2f, LocalCoords*);
        [[maybe_unused]]
        explicit LocalCoords(sf::Vector2f, float, LocalCoords*);
        [[maybe_unused]]
        explicit LocalCoords(sf::Vector2f, sf::Vector2f, float, LocalCoords*);

        sf::Vector2f getWorldPosition();
        sf::Vector2f getWorldScale();
        float getWorldRotation() const;

        //Simple methods
        void setDepth(int depth) { _depth = depth; }
        int getDepth() const { return _depth; }
        void setParent(LocalCoords* parent) { this->_parent = parent; }
        LocalCoords* getParent() { return _parent; }
    };

    class Bounds {
        LocalCoords *_parent;
    public:
        sf::Rect<float> rect_bounds;

        explicit Bounds(LocalCoords *parent){
            rect_bounds.left = 0;
            rect_bounds.top = 0;
            rect_bounds.width = 1;
            rect_bounds.height = 1;
            _parent = parent;
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
