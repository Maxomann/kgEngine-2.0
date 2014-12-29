#include "Entity.h"
using namespace std;

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

}
