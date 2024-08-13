#include "GDMBasics.h"
#include "Sprite.h"

#ifndef GDMATEEXAMPLES_CAMERA_H
#define GDMATEEXAMPLES_CAMERA_H
namespace mate
{
class Sprite;

class Camera : public Component
{
  public:
    explicit Camera(const std::weak_ptr<Element> &parent);
    ~Camera();

    enum ScaleType
    {
        LETTERBOX,
        RESCALE,
        REVEAL
    };

  private:
    sf::View _view;
    std::weak_ptr<Game> _game_manager;
    std::list<std::weak_ptr<const Sprite>> _visible_sprites;

    float _aspect_ratio;
    ScaleType _scale_type = RESCALE;

  public:
    u_int target_id = 0;

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
    [[maybe_unused]] float getRatio();
    void useNewTarget();
    void loop() override{};
    void renderLoop() override;
    void windowResizeEvent() override;
};
} // namespace mate
#endif // GDMATEEXAMPLES_CAMERA_H
