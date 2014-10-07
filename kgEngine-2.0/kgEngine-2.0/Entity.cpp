#include "Entity.h"

namespace kg
{

	Entity::Entity( const Id& id )
		:m_id( id )
	{

	}

	const Entity::Id& Entity::getId() const
	{
		return m_id;
	}

	void Entity::initFromBlueprint( Engine& engine, const blueprint::Entity& entity )
	{
		auto components = entity.getComponentNames();
		for( const auto& name : components )
		{
			auto createdComponent = engine.pluginManager.createComponentPlugin( name );
			auto component = std::get<2>( createdComponent );
			component->preInit( entity.getComponentValues(name) );
			addComponent( component, std::get<1>( createdComponent ) );
		}
		initComponents( engine );
	}
}
