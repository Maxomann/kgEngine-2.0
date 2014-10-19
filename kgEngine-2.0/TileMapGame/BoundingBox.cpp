#include "BoundingBox.h"
using namespace std;
using namespace sf;

namespace kg
{
	void BoundingBox::preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues )
	{
		auto it = blueprintValues.find( BLUEPRINT_HEIGHT );
		if( it != blueprintValues.end() )
			m_boundingBox.height = it->second.toInt();
		it = blueprintValues.find( BLUEPRINT_WIDTH );
		if( it != blueprintValues.end() )
			m_boundingBox.width = it->second.toInt();
	}

	void BoundingBox::init( Engine& engine, ComponentManager& componentManager )
	{
		r_position = componentManager.getComponent<Position>().get();
		r_position->registerCallback_1<BoundingBox, const sf::Vector2i&>(
			( int )Position::CallbackId::CHANGED,
			this,
			&BoundingBox::onPositionChanged );
		return;
	}

	void BoundingBox::update( Engine& engine, World& world, ComponentManager& componentManager )
	{
		return;
	}

	double BoundingBox::getUpdateImportance() const
	{
		return ( double )id::ComponentUpdateImportance::BOUNDING_BOX;
	}

	std::vector<size_t> BoundingBox::getRequieredComponents() const
	{
		return{ typeid(Position).hash_code() };
	}

	const std::string& BoundingBox::getPluginName() const
	{
		return "BoundingBox";
	}

	int BoundingBox::getPluginId() const
	{
		return ( int )id::ComponentPluginId::BOUNDING_BOX;
	}

	void BoundingBox::onPositionChanged( int CallbackId, const sf::Vector2i& position )
	{
		setCenter( position );
	}

	void BoundingBox::setSize( const sf::Vector2i& size )
	{
		m_boundingBox.width = size.x;
		m_boundingBox.height = size.y;
		setCenter( r_position->get() );
	}

	sf::Vector2i BoundingBox::getSize() const
	{
		return sf::Vector2i( m_boundingBox.width, m_boundingBox.height );
	}

	void BoundingBox::setCenter( const sf::Vector2i& position )
	{
		m_boundingBox.left = position.x - m_boundingBox.width / 2;
		m_boundingBox.top = position.y - m_boundingBox.height / 2;
		triggerCallback<const sf::IntRect&>( ( int )CallbackId::CHANGED, m_boundingBox );
	}

	const sf::IntRect& BoundingBox::getBoundingBox() const
	{
		return m_boundingBox;
	}

	const std::string BoundingBox::BLUEPRINT_WIDTH = "width";
	const std::string BoundingBox::BLUEPRINT_HEIGHT = "height";
}
