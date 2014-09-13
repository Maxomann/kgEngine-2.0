#include "PositionComponent.h"

namespace kg
{


	void PositionComponent::setPosition( const sf::Vector2i& position )
	{
		m_position = position;
	}

	const sf::Vector2i& PositionComponent::getPosition() const
	{
		return m_position;
	}

	void PositionComponent::init( Engine& engine, ComponentManager& componentManager )
	{
		return;
	}

	void PositionComponent::update( Engine& engine, World& world, ComponentManager& componentManager )
	{
		return;
	}

	int PositionComponent::getId()
	{
		return ComponentId::PositionComponent;
	}

	double PositionComponent::getUpdateImportance() const
	{
		return ComponentUpdateImportance::PositionComponent;
	}

}
