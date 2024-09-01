//
// Created by elly_sparky on 01/08/24.
//
#include "GDMBasics.h"

namespace mate
{
void TriggerManager::removeTrigger(int trigger_id)
{
    for (const std::unique_ptr<Trigger> &trigger : triggers)
    {
        if (trigger->getID() == trigger_id)
        {
            trigger->destroy();
        }
    }
}

// Todo: Rotated rectangles
// Todo: Add depth to triggers
void TriggerManager::checkTrigger(ShapeType shape, const TriggerShooter &shooter)
{
    sf::Vector2f position = shooter.getPosition();
    sf::Vector2f dimensions = shooter.getDimensions();
    float radius, trig_radius;

    for (const std::unique_ptr<Trigger> &trigger : triggers)
    {
        sf::Vector2 trig_position = trigger->getWorldPosition();
        sf::Vector2 trig_dimensions = trigger->getWorldScale();

        switch (trigger->shape)
        {
        case RECTANGLE:
            switch (shape)
            {
            case RECTANGLE:
                // Collision between two rectangles
                if (rectangleToRectangleCheck(position, dimensions, trig_position, trig_dimensions))
                    trigger->triggerIn();
                break;
            case CIRCLE:
                // Shooter is a circle, trigger is a rectangle

                // Use the center of the circle instead of the corner, also stores the radius
                radius = std::max(dimensions.x, dimensions.y) / 2;
                position.x += radius;
                position.y += radius;

                if (circleToRectangleCheck(position, radius, trig_position, trig_dimensions))
                    trigger->triggerIn();
                break;
            }
            break;
        case CIRCLE:
            // Use the center of the circle instead of the corner, also stores the radius
            trig_radius = std::max(trig_dimensions.x, trig_dimensions.y) / 2;
            trig_position.x += trig_radius;
            trig_position.y += trig_radius;

            switch (shape)
            {
            case RECTANGLE:
                if (circleToRectangleCheck(trig_position, trig_radius, position, dimensions))
                    trigger->triggerIn();
                break;
            case CIRCLE:
                // Collision between circles
                radius = std::max(dimensions.x, dimensions.y) / 2;
                position.x += radius;
                position.y += radius;

                if (circleToCircleCheck(position, radius, trig_position, trig_radius))
                    trigger->triggerIn();
                break;
            }
            break;
        }
    }
}

bool TriggerManager::rectangleToRectangleCheck(sf::Vector2f rect1_pos, sf::Vector2f rect1_dim, sf::Vector2f rect2_pos,
                                               sf::Vector2f rect2_dim)
{
    return (rect1_pos.x + rect1_dim.x > rect2_pos.x && rect1_pos.x < rect2_pos.x + rect2_dim.x &&
            rect1_pos.y < rect2_pos.y + rect2_dim.y && rect1_pos.y + rect1_dim.y > rect2_pos.y);
}

bool TriggerManager::circleToCircleCheck(sf::Vector2f circ1_pos, float circ1_rad, sf::Vector2f circ2_pos,
                                         float circ2_rad)
{
    double distance = std::sqrt(std::pow(circ1_pos.x - circ2_pos.x, 2) + std::pow(circ1_pos.y - circ2_pos.y, 2));
    return (circ1_rad + circ2_rad > distance);
}

bool TriggerManager::circleToRectangleCheck(sf::Vector2f circ_pos, float radius, sf::Vector2f rect_pos,
                                            sf::Vector2f rect_dim)
{

    double distance_x = rect_pos.x + rect_dim.x / 2 - circ_pos.x;
    short sign = std::signbit(distance_x);
    // If distance_x is positive means circle is on the left, negative means it's on the right
    // Checks if the circle is inside the rectangle using it's nearest horizontal sides
    if ((circ_pos.x + ((distance_x >= 0) - (distance_x < 0)) * radius >= rect_pos.x + sign * rect_dim.x) != sign &&
        circ_pos.y <= rect_pos.y + rect_dim.y && circ_pos.y >= rect_pos.y)
    {
        return true;
    }

    double distance_y = rect_pos.y + rect_dim.y / 2 - circ_pos.y;
    sign = std::signbit(distance_y);
    // If distance_y is positive means circle is above, negative means it's below
    // Same as before but using the vertical sides
    if ((circ_pos.y + ((distance_y >= 0) - (distance_y < 0)) * radius >= rect_pos.y + sign * rect_dim.y) != sign &&
        circ_pos.x <= rect_pos.x + rect_dim.x && circ_pos.x >= rect_pos.x)
    {
        return true;
    }

    // Finally checks if the nearest corner of the rectangle is inside the circle
    //  (Which can happen even when both the previows conditions are false)
    // This is checked last to reduce the use of the sqrt function
    distance_x += ((distance_x < 0) - (distance_x >= 0)) * rect_dim.x / 2;
    distance_y += ((distance_y < 0) - (distance_y >= 0)) * rect_dim.y / 2;

    double distance = std::sqrt(std::pow(distance_y, 2) + std::pow(distance_x, 2));
    return (distance < radius);
}
} // namespace mate
