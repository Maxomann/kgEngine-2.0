#include "Transformation.h"
using namespace std;
using namespace sf;

namespace kg
{
	void Transformation::preInit( Engine& engine, std::map<std::string, blueprint::Value>& blueprintValues )
	{
		auto it = blueprintValues.find( BLUEPRINT_HEIGHT );
		if( it != blueprintValues.end() )
			m_size.x = it->second.toInt();
		it = blueprintValues.find( BLUEPRINT_WIDTH );
		if( it != blueprintValues.end() )
			m_size.y = it->second.toInt();
		it = blueprintValues.find( BLUEPRINT_ZVALUE );
		if( it != blueprintValues.end() )
			m_zValue = it->second.toInt();
	}

	void Transformation::init( Engine& engine, World& world, ComponentManager& thisEntity )
	{
		auto saveComponent = thisEntity.getComponent<Save>();
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

	std::vector<size_t> Transformation::getRequieredComponents() const
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

	const sf::FloatRect Transformation::getGlobalBounds() const
	{
		RectangleShape shape;
		shape.setSize( sf::Vector2f( m_size ) );
		shape.setOrigin( sf::Vector2f( static_cast< float >(m_size.x) / 2, static_cast< float >(m_size.y) / 2 ) );
		shape.setPosition( sf::Vector2f( m_position ) );
		shape.setRotation( m_rotation );

		return shape.getGlobalBounds();
	}

	void Transformation::setPosition( const sf::Vector2i& position )
	{
		m_position = position;

		s_transformationChanged();
		s_positionChanged( m_position );
	}

	const sf::Vector2i& Transformation::getPosition() const
	{
		return m_position;
	}

	void Transformation::move( const sf::Vector2i& offset )
	{
		m_position += offset;

		s_transformationChanged();
		s_positionChanged( m_position );
	}

	void Transformation::setRotation( const float rotationInDegree )
	{
		m_rotation = rotationInDegree;

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

		s_transformationChanged();
		s_rotationChanged( m_rotation );
	}

	void Transformation::setSize( const sf::Vector2i& size )
	{
		m_size = size;

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
				Vector2i( atoi( information.at( 0 ).c_str() ),
				atoi( information.at( 1 ).c_str() ) ) );
			setRotation( static_cast<float>(atof( information.at( 2 ).c_str() )) );
			setSize( Vector2i(
				atoi( information.at( 3 ).c_str() ),
				atoi( information.at( 4 ).c_str() ) ) );
			setZValue( atoi( information.at( 5 ).c_str() ) );
		}
	}

	std::vector<std::string> Transformation::onWriteSaveInformation()
	{
		return{
			to_string( m_position.x ),
			to_string( m_position.y ),
			to_string( m_rotation ),
			to_string( m_size.x ),
			to_string( m_size.y ),
			to_string( m_zValue ) };
	}

	int Transformation::getZValue() const
	{
		return m_zValue;
	}

	void Transformation::setZValue( int zValue )
	{
		m_zValue = zValue;
	}

	const std::string Transformation::BLUEPRINT_ZVALUE = "zValue";


	const std::string Transformation::PLUGIN_NAME = "Transformation";

	const std::string Transformation::BLUEPRINT_WIDTH = "width";
	const std::string Transformation::BLUEPRINT_HEIGHT = "height";
}
