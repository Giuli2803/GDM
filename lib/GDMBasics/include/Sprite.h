/**
 * @brief Sprite class declaration
 * @file
 */

#ifndef GDMATE_SPRITE_H
#define GDMATE_SPRITE_H

#include "GDMBasics.h"
#include <string>

namespace mate
{
/**
 * @brief Visual component.
 *
 * Sprites are just that, the Component holds the image to be displayed on the screen on the coordinates of the
 * associated Element.
 */
class Sprite : public Component
{
  private:
    sf::Texture _texture;
    std::shared_ptr<ord_sprite> _sprite;
    std::weak_ptr<Game> _game_manager;

    bool _actualize = true;

  public:
    Bounds offset;
    // Constructor
    explicit Sprite(const std::weak_ptr<Element> &parent);
    ~Sprite() = default;

    // Simple methods
    /**
     * Loads an image from a file and sets it as the displayed image of the Sprite.
     * @param filename relative path of the file.
     */
    void setTexture(const std::string &filename)
    {
        _texture.loadFromFile(filename);
        _sprite->sprite.setTexture(_texture, true);
    }

    [[maybe_unused]] void setColor(sf::Color color)
    {
        _sprite->sprite.setColor(color);
    }

    /**
     * Color setter with RGBA values.
     */
    [[maybe_unused]] void setColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
    {
        _sprite->sprite.setColor(sf::Color(red, green, blue, alpha));
    }

    std::shared_ptr<const ord_sprite> getSprite() const
    {
        return _sprite;
    }

    /**
     * Sprite depth comes secondary to the associated Element's depth, this means that the Sprite depth will only be
     * taken in account when multiple Sprites have the same Element depth.
     */
    [[maybe_unused]] void setSpriteDepth(unsigned int depth)
    {
        _sprite->depth = depth;
    }

    /**
     * @return In case of the associated Element not existing anymore (A problem that shouldn't really occur) the
     * returned depth will be the minimum possible.
     */
    [[maybe_unused]] int getElementDepth() const
    {
        if (auto spt_parent = _parent.lock())
        {
            return spt_parent->depth;
        }
        return INT_MIN;
    }

    [[maybe_unused]] unsigned int getSpriteDepth() const
    {
        return _sprite->depth;
    }

    /**
     * @param actualize if false the Sprite will not run it's loop() method until set back to true.
     */
    [[maybe_unused]] void doActualize(bool actualize)
    {
        _actualize = actualize;
    }

    // Other methods declarations
    /**
     * Adds a value to the Sprite's depth. If the result exceeds the valid limits the result will remain at
     * the corresponding limit.
     */
    [[maybe_unused]] void addDepth(int depth);
    /**
     * Sprite's loop() actualizes the position, rotation and scale of the printed image following
     * the associated Element.
     */
    void loop() override;
};
} // namespace mate

#endif // GDMATE_SPRITE_H
