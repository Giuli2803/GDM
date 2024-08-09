//
// Created by elly_sparky on 02/01/24.
//

#ifndef GDMATE_SPRITE_H
#define GDMATE_SPRITE_H

#include <string>
#include "GDMBasics.h"

//Todo:
// * If print


namespace mate{
    class Sprite : public Component {
    private:
        sf::Texture _texture;
        std::shared_ptr<ord_sprite> _sprite;
        std::weak_ptr<Game> _game_manager;

        bool _actualize = true;
        unsigned int _depth = 0;
    public:
        //Constructor
        explicit Sprite(const std::weak_ptr<Element>&  parent);
        ~Sprite();

        //Simple methods
        void setTexture(const std::string &filename){
            _texture.loadFromFile(filename);
            ///This might be necessary if the new sprite has a different size, but I'm not sure of that
            //_sprite.sprite.setTexture(_texture, true);
        }

        [[maybe_unused]]
        void setColor(sf::Color color){
            _sprite->sprite.setColor(color);
        }

        std::shared_ptr<const ord_sprite> getSprite(){
            return _sprite;
        }

        [[maybe_unused]]
        void setColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha){
            _sprite->sprite.setColor(sf::Color(red, green, blue, alpha));
        }

        [[maybe_unused]]
        void setDepth(unsigned int depth){
            _depth = depth;
        }

        [[maybe_unused]]
        void addDepth(int depth){
            unsigned int stored = _depth;
            _depth += depth;
            if(depth < 0 && _depth > stored){
                _depth = 0;
            } else if(depth > 0 && _depth < stored){
                _depth = UINT_MAX;
            }
        }

        [[maybe_unused]]
        unsigned int getDepth() const { return _depth; }

        [[maybe_unused]]
        void doActualize(bool actualize){
            _actualize = actualize;
        }

        //Other methods declarations
        void Loop() override;
    };
}

#endif //GDMATE_SPRITE_H
