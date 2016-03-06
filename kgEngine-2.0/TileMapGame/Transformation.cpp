#include "Transformation.h"
using namespace std;
using namespace sf;

namespace kg
{
	void Transformation::preInit( Engine& engine, const std::map<blueprint::ComponentValue::Name, const blueprint::ComponentValue*>& blueprintValues )
	{
		auto it = blueprintValues.find( BLUEPRINT_HEIGHT );
		if( it != blueprintValues.end() )
			m_size.x = it->second->asInt();
		it = blueprintValues.find( BLUEPRINT_WIDTH );
		if( it != blueprintValues.end() )
			m_size.y = it->second->asInt();

		recalculateGlobalBounds();
	}

	void Transformation::init( Engine& engine, const World& world, const ComponentManager& thisEntity )
	{
		auto saveComponent = thisEntity.getComponentTry<Save>();
		if( saveComponent )
		{
			m_connectToSignal( saveComponent->s_loadSaveInformation[( int )id::ComponentPluginId::TRANSFORMATION],
							   &Transformation::onLoadSaveInformation );
			m_connectToSignal( saveComponent->s_writeSaveInformation[( int )id::ComponentPluginId::TRANSFORMATION],
							   &Transformation::onWriteSaveInformation );
		}
	}

	void Transformation::update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime )
	{
		return;
	}

	double Transformation::getUpdateImportance() const
	{
		return id::ComponentUpdateImportance::TRANSFORMATION;
	}

	std::vector<Plugin::Id> Transformation::getRequieredComponents() const
	{
		return{ };
	}

	const std::string& Transformation::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	Plugin::Id Transformation::getPluginId() const
	{
		return id::ComponentPluginId::TRANSFORMATION;
	}

	sf::FloatRect Transformation::getGlobalBounds() const
	{
		return m_globalBounds;
	}

	bool Transformation::intersects( const sf::FloatRect& rect )const
	{
		return m_globalBounds.intersects( rect );
	}

	const size_t& Transformation::getRTTI_hash() const
	{
		return type_hash;
	}

	void Transformation::setPosition( const Position2d& position )
	{
		m_position = position;

		s_transformationChanged();
		s_position2dChanged( m_position );
	}

	const Position2d& Transformation::getPosition() const
	{
		return m_position;
	}

	void Transformation::moveXYsetWorldLayer( const sf::Vector2i& offsetXY, int worldLayer )
	{
		m_position.x = offsetXY.x;
		m_position.y = offsetXY.y;
		m_position.worldLayer = worldLayer;

		s_transformationChanged();
		s_position2dChanged( m_position );
	}

	void Transformation::setPositionXY( const PositionXY& position )
	{
		m_position.x = position.x;
		m_position.y = position.y;

		recalculateGlobalBounds();

		s_transformationChanged();
		s_position2dChanged( m_position );
	}

	kg::PositionXY Transformation::getPositionXY() const
	{
		return m_position.toPositionXY();
	}

	void Transformation::moveXY( const sf::Vector2i& offsetXY )
	{
		m_position.x += offsetXY.x;
		m_position.y += offsetXY.y;

		recalculateGlobalBounds();

		s_transformationChanged();
		s_position2dChanged( m_position );
	}

	void Transformation::setRotation( const float rotationInDegree )
	{
		m_rotation = rotationInDegree;

		recalculateGlobalBounds();

		s_transformationChanged();
		s_rotationChanged( m_rotation );
	}

	float Transformation::getRotationInDegree() const
	{
		return m_rotation;
	}

	void Transformation::rotate( const float offsetInDegree )
	{
		m_rotation += offsetInDegree;

		recalculateGlobalBounds();

		s_transformationChanged();
		s_rotationChanged( m_rotation );
	}

	void Transformation::setSize( const sf::Vector2i& size )
	{
		m_size = size;

		recalculateGlobalBounds();

		s_transformationChanged();
		s_sizeChanged( m_size );
	}

	sf::Vector2i Transformation::getSize() const
	{
		return m_size;
	}

	void Transformation::onLoadSaveInformation( const std::vector<std::string>& information )
	{
		if( information.size() != 6 )
			throw exception();
		else
		{
			setPosition(
				Position2d(
					atoi( information.at( 0 ).c_str() ),
					atoi( information.at( 1 ).c_str() ),
					atoi( information.at( 2 ).c_str() )
					)
				);
			setRotation( static_cast< float >(atof( information.at( 3 ).c_str() )) );
			setSize( Vector2i(
				atoi( information.at( 4 ).c_str() ),
				atoi( information.at( 5 ).c_str() ) ) );
		}
	}

	std::vector<std::string> Transformation::onWriteSaveInformation()
	{
		return{
			to_string( m_position.x ),
			to_string( m_position.y ),
			to_string( m_position.worldLayer ),
			to_string( m_rotation ),
			to_string( m_size.x ),
			to_string( m_size.y ), };
	}

	void Transformation::recalculateGlobalBounds()
	{
		RectangleShape shape;

		shape.setSize( sf::Vector2f( m_size ) );
		shape.setOrigin( sf::Vector2f( static_cast< float >(m_size.x) / 2, static_cast< float >(m_size.y) / 2 ) );
		shape.setPosition( sf::Vector2f( m_position.x, m_position.y ) );
		shape.setRotation( m_rotation );

		m_globalBounds = shape.getGlobalBounds();
	}

	int Transformation::getWorldLayer() const
	{
		return m_position.worldLayer;
	}

	void Transformation::setWorldLayer( int layer )
	{
		m_position.worldLayer = layer;

		s_transformationChanged();
		s_position2dChanged( m_position );
	}

	const boost::optional<ChunkPosition>& Transformation::getLastChunkPosition() const
	{
		return m_chunkPosition;
	}

	void Transformation::setChunkPostion( ChunkPosition chunkPosition )
	{
		m_chunkPosition = chunkPosition;
	}

	const std::string Transformation::PLUGIN_NAME = "Transformation";

	const size_t Transformation::type_hash = getRuntimeTypeInfo<Transformation>();

	const std::string Transformation::BLUEPRINT_WIDTH = "width";
	const std::string Transformation::BLUEPRINT_HEIGHT = "height";
}