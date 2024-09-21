/**
 * @brief Declaration of classes LocalCoords and Bounds, and function weakPtrIsUninitialized.
 * @file
 */

#ifndef GDMATE_LOCALCOORDS_H
#define GDMATE_LOCALCOORDS_H

#include <SFML/Graphics.hpp>
#include <memory>

namespace mate
{
/**
 * @brief Simple template function for the verification of weak_ptr initialization.
 * @param weak weak_ptr to verify.
 * @return true if the parameter weak_ptr was not initialized. false otherwise.
 */
template <typename T> bool weakPtrIsUninitialized(std::weak_ptr<T> const &weak)
{
    using wt = std::weak_ptr<T>;
    return !weak.owner_before(wt{}) && !wt{}.owner_before(weak);
}

/**
 * @brief Secondary set of coordinates used for Component specific corrections.
 *
 * Bounds is a simple local coordinates adjustment for Components such as Trigger and Sprite. It has only one
 * sf::Rect<float> variable and it's access methods, yet it adds a consistency on the usage of this object for
 * all Components that require an special adjustment.
 */
class Bounds
{
  public:
    /**
     * Local coordinates adjustment for Components only.
     */
    sf::Rect<float> rect_bounds;

    explicit Bounds()
    {
        rect_bounds.left = 0;
        rect_bounds.top = 0;
        rect_bounds.width = 1;
        rect_bounds.height = 1;
    }

    /**
     * @brief Calculates the adjusted position coordinates.
     * @param pos Position coordinates of the relative LocalCoords to adjust.
     * @return Adjusted position coordinates.
     */
    [[nodiscard]] sf::Vector2f getPositionBounds(sf::Vector2f pos) const
    {
        pos.x += rect_bounds.left;
        pos.y += rect_bounds.top;
        return pos;
    }

    /**
     * @brief Calculates the adjusted scale coordinates.
     * @param scale Scale coordinates of the relative LocalCoords to adjust.
     * @return Adjusted scale coordinates.
     */
    [[nodiscard]] sf::Vector2f getDimensionBounds(sf::Vector2f scale) const
    {
        scale.x *= rect_bounds.width;
        scale.y *= rect_bounds.height;
        return scale;
    }
};

/**
 * @brief Local and global coordinates tracking.
 *
 * LocalCoords keeps the current local position, rotation and scale values for an object within the game, and holds a
 * reference to the "parent" object's coordinates to calculate global coordinates.
 */
class LocalCoords : public sf::Transformable, public std::enable_shared_from_this<LocalCoords>
{
  private:
    std::weak_ptr<LocalCoords> _parent; ///< Parent's object coordinates reference.
  public:
    /**
     * @brief Pseudo third dimension.
     *
     * Although games run on 2D only a pseudo third dimension is still needed for stuff such as Sprite ordering and
     * Trigger depth detection.
     * In any case lower values correspond to objects being closer to the background and higher values to objects
     * closer to the foreground.
     */
    int depth = 0;
    // Constructors:

    /**
     * This constructor is meant for highest authority objects that have no parent such as Room.
     */
    [[maybe_unused]] LocalCoords();
    [[maybe_unused]] explicit LocalCoords(const std::weak_ptr<LocalCoords> &parent);
    [[maybe_unused]] explicit LocalCoords(sf::Vector2f position, const std::shared_ptr<LocalCoords> &parent);
    [[maybe_unused]] explicit LocalCoords(sf::Vector2f, float, const std::shared_ptr<LocalCoords> &parent);
    [[maybe_unused]] explicit LocalCoords(sf::Vector2f, sf::Vector2f, const std::shared_ptr<LocalCoords> &parent);
    [[maybe_unused]] explicit LocalCoords(sf::Vector2f, sf::Vector2f, float,
                                          const std::shared_ptr<LocalCoords> &parent);

    // Getters

    /**
     * Recursively calculates object's world position relative to it's parent's world position.
     * @return local position + parent's world position.
     */
    sf::Vector2f getWorldPosition();

    /**
     * Recursively calculates object's world scale relative to it's parent's world scale.
     * @return local scale * parent's world scale.
     */
    sf::Vector2f getWorldScale();

    /**
     * Recursively calculates object's world rotation relative to it's parent's worlds rotation.
     * @return local rotation + parent's world rotation.
     */
    float getWorldRotation() const;

    std::weak_ptr<LocalCoords> getParent() const
    {
        return _parent;
    }

    /**
     * @deprecated depth is now a public variable.
     */
    [[deprecated]] int getDepth() const
    {
        return depth;
    }

    // Setters

    /**
     * @deprecated depth is now a public variable.
     */
    [[deprecated]] void setDepth(int depth_)
    {
        depth = depth_;
    }

    void setParent(const std::weak_ptr<LocalCoords> &parent)
    {
        _parent = parent;
    }
};
} // namespace mate

#endif // GDMATE_LOCALCOORDS_H
