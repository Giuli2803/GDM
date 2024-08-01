//
// Created by elly_sparky on 14/01/24.
//

#ifndef GDMATE_CAMERA_H
#define GDMATE_CAMERA_H

#include <SFML/Graphics.hpp>
#include "GDMBasics.h"

namespace mate{
    class Camera : public Component {
    public:
        explicit Camera(const std::weak_ptr<Element>& parent);
        ~Camera();

        enum ScaleType {
            LETTERBOX, RESCALE, REVEAL
        };
    private:
        sf::View _view;
        std::shared_ptr<sf::RenderTarget> _target;
        float _aspect_ratio;
        ScaleType _scale_type = RESCALE;
    public:
        //Simple methods
        void setSize(float x, float y){
            _view.setSize(x, y);
            _aspect_ratio = x / y;
        }

        sf::Vector2f getSize(){
            return _view.getSize();
        }

        [[maybe_unused]]
        float getRatio() {
            if(_scale_type == REVEAL){
                auto size = _view.getSize();
                _aspect_ratio = size.x / size.y;
            }
            return _aspect_ratio;
        }

        void setScaleType(ScaleType scale_type){
            _scale_type = scale_type;
        }

        [[maybe_unused]]
        void setTarget(std::shared_ptr<sf::RenderTarget> target){
            _target = std::move(target);
        }

        //Other methods declarations
        void Loop() override;
        void WindowResizeEvent() override;
    };
}//mate

#endif //GDMATE_CAMERA_H
