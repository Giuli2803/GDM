#include "GDMBasics.h"

#ifndef GDMATEEXAMPLES_CAMERA_H
#define GDMATEEXAMPLES_CAMERA_H
namespace mate{
class Camera : public Component {
public:
    explicit Camera(const std::weak_ptr<Element> &parent);

    ~Camera();

    enum ScaleType {
        LETTERBOX, RESCALE, REVEAL
    };
private:
    sf::View _view;
    std::weak_ptr<Game> _game_manager;
    std::list<std::weak_ptr<const ord_sprite>> _visible_sprites;

    float _aspect_ratio;
    ScaleType _scale_type = RESCALE;
public:
    u_int target_id = 0;

    //Simple methods
    void setSize(float x, float y) {
        _view.setSize(x, y);
        _aspect_ratio = x / y;
    }

    sf::Vector2f getSize() {
        return _view.getSize();
    }

    [[maybe_unused]]
    float getRatio() {
        if (_scale_type == REVEAL) {
            auto size = _view.getSize();
            _aspect_ratio = size.x / size.y;
        }
        return _aspect_ratio;
    }

    void setScaleType(ScaleType scale_type) {
        _scale_type = scale_type;
    }

    void setTarget(u_int id) {
        target_id = id;
    }

    void UseNewTarget() {
        if (auto _spt_game = _game_manager.lock()) {
            target_id = _spt_game->AddSecondaryTarget(_view);
        }
    }

    void addSprite(const std::weak_ptr<const ord_sprite> &sprite) {
        _visible_sprites.push_back(sprite);
    }

    void RemoveSprite(const std::shared_ptr<const ord_sprite> &sprite) {
        _visible_sprites.remove_if([&sprite](const std::weak_ptr<const ord_sprite> &weak_sprite) {
            return weak_sprite.lock() == sprite;
        });
    }

    //Other methods declarations
    void Loop() override;

    void RenderLoop() override;

    void WindowResizeEvent() override;
};
}
#endif //GDMATEEXAMPLES_CAMERA_H
