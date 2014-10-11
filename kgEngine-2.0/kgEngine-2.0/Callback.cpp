#include "Callback.h"

namespace kg
{
	CallbackReciever::CallbackReciever() : m_CallbackRecieverDummyObject( std::make_shared<bool>( true ) )
	{

	}

	std::weak_ptr<bool> CallbackReciever::aGetWeakPointer() const
	{
		return m_CallbackRecieverDummyObject;
	}

}
