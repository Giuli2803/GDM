/**
 * @brief Camera class declaration.
 * @file
 */

#ifndef GDMATEEXAMPLES_CAMERA_H
#define GDMATEEXAMPLES_CAMERA_H

#include "GDMBasics.h"
#include "Sprite.h"

namespace mate
{
class Sprite;

/**
 * @brief Component for the control of window's views.
 *
 * Camera manages an sf::View object and attaches it to a render_target to display it.
 */
class Camera : public Component
{
  public:
    explicit Camera(const std::weak_ptr<Element> &parent);
    ~Camera();

    /**
     * How should the view change when the window changes in size.
     */
    enum ScaleType
    {
        LETTERBOX, ///< Keep the view ratio without stretching objects by adding black lines on the extra space.
        RESCALE,   ///< Stretch the objects in the view to fit the window space while keeping the ratio.
        REVEAL     ///< Keep the world objects sizes persistent and show more or less as the window changes in size.
    };

  private:
    sf::View _view;
    std::weak_ptr<Game> _game_manager;
    std::list<std::weak_ptr<const Sprite>> _visible_sprites;

    float _aspect_ratio;
    ScaleType _scale_type = RESCALE;

  public:
    u_int target_id = 0; ///< id value of the target (window) to print into.

    // Simple methods
    void setSize(float x, float y)
    {
        _view.setSize(x, y);
        _aspect_ratio = x / y;
    }

    sf::Vector2f getSize()
    {
        return _view.getSize();
    }

    void setScaleType(ScaleType scale_type)
    {
        _scale_type = scale_type;
    }

    ScaleType getScaleType() const
    {
        return _scale_type;
    }

    void setTarget(u_int id)
    {
        target_id = id;
    }

    void addSprite(const std::weak_ptr<const Sprite> &sprite)
    {
        _visible_sprites.push_back(sprite);
    }

    void removeSprite(const std::shared_ptr<const Sprite> &sprite)
    {
        _visible_sprites.remove_if(
            [&sprite](const std::weak_ptr<const Sprite> &weak_sprite) { return weak_sprite.lock() == sprite; });
    }

    // Other methods declarations
    /**
     * @return View width on pixels / view height on pixels.
     */
    [[maybe_unused]] float getRatio();
    /**
     * Generates a new render_target (window by default) to print the view into.
     */
    unsigned int useNewTarget(const std::string &title);
    void loop() override{};
    void renderLoop() override;
    void windowResizeEvent() override;

#ifdef GDM_TESTING_ENABLED
    std::weak_ptr<const Sprite> getTopSprite()
    {
        return _visible_sprites.front();
    }

    std::weak_ptr<const Sprite> getBottomSprite()
    {
        return _visible_sprites.back();
    }

    sf::View getView() const
    {
        return _view;
    }
#endif
};
} // namespace mate
#endif // GDMATEEXAMPLES_CAMERA_H
