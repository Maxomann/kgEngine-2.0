#include "GlobalBounds.h"
using namespace std;
using namespace sf;

namespace kg
{
	void GlobalBounds::preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues )
	{
		return;
	}

	void GlobalBounds::init( Engine& engine, ComponentManager& thisEntity )
	{
		r_position = thisEntity.getComponent<Position>().get();
		r_size = thisEntity.getComponent<Size>().get();
		r_rotation = thisEntity.getComponent<Rotation>().get();

		//register callbacks:
		m_connectToSignal( r_position->s_changed, std::function<void( const sf::Vector2i& )>( [&]( const sf::Vector2i& )
		{
			onTransformationChanged();
		} ) );
		m_connectToSignal( r_size->s_changed, std::function<void( const sf::Vector2i& )>( [&]( const sf::Vector2i& )
		{
			onTransformationChanged();
		} ) );
		if( r_rotation != nullptr )
		{
			m_connectToSignal( r_rotation->s_changed, std::function<void( const float& )>( [&]( const float& )
			{
				onTransformationChanged();
			} ) );
		}

		return;
	}

	void GlobalBounds::update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime )
	{
		return;
	}

	double GlobalBounds::getUpdateImportance() const
	{
		return ( double )id::ComponentUpdateImportance::GLOBAL_BOUNDS;
	}

	std::vector<size_t> GlobalBounds::getRequieredComponents() const
	{
		return{ typeid(Position).hash_code(), typeid(Size).hash_code() };
	}

	const std::string& GlobalBounds::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	int GlobalBounds::getPluginId() const
	{
		return ( int )id::ComponentPluginId::GLOBAL_BOUNDS;
	}

	const sf::FloatRect GlobalBounds::get() const
	{
		auto position = r_position->get();
		auto size = r_size->get();
		float rotation = 0;
		if( r_rotation )
			rotation = r_rotation->get();

		RectangleShape shape;
		shape.setSize( sf::Vector2f( size ) );
		shape.setOrigin( sf::Vector2f( static_cast< float >(size.x) / 2, static_cast< float >(size.y) / 2 ) );
		shape.setPosition( sf::Vector2f( position ) );
		shape.setRotation( rotation );

		return shape.getGlobalBounds();
	}

	void GlobalBounds::onTransformationChanged()
	{
		s_changed( get() );
	}

	const std::string GlobalBounds::PLUGIN_NAME = "GlobalBounds";
}