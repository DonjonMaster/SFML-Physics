#include "ColliderComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

namespace engine
{
sf::FloatRect ColliderComponent::getWorldBounds() const
{
    sf::Vector2f position{0.f, 0.f};
    if (auto* transform = dynamic_cast<TransformComponent*>(getOwner()->getComponent("Transform")))
    {
        position = transform->getPosition();
    }

    return sf::FloatRect(position + m_offset, m_size);
}

void ColliderComponent::onRender(sf::RenderTarget& target)
{
    if (!m_debugDraw)
    {
        return;
    }

    const sf::FloatRect bounds = getWorldBounds();
    sf::RectangleShape debugRect;
    debugRect.setPosition(bounds.position);
    debugRect.setSize(bounds.size);
    debugRect.setFillColor(sf::Color(0, 0, 0, 0));
    debugRect.setOutlineColor(m_debugColor);
    debugRect.setOutlineThickness(1.f);
    target.draw(debugRect);
}
} // namespace engine
