#ifdef KG_OLD_CALLBACK_SYSTEM

#include "Callback.h"

namespace kg
{
	CallbackReciever::CallbackReciever() : m_CallbackRecieverDummyObject( std::make_shared<bool>( true ) )
	{ }

	std::weak_ptr<bool> CallbackReciever::getWeakPointer() const
	{
		return m_CallbackRecieverDummyObject;
	}
}

#endif