#include "Entity.h"
using namespace std;

namespace kg
{
	bool Entity::operator==( const Entity& entity )const
	{
		if( &entity == this )
			return true;
		return false;
	}
}