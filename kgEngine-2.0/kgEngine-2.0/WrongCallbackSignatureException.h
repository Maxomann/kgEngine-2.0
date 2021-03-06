#pragma once
#include "stdafx.h"
#pragma warning(push)
#pragma warning(disable: 4275) // possible loss of data

namespace kg
{
	class DLL_EXPORT WrongCallbackSignatureException : public std::exception
	{
		const std::string m_msg;

	public:
		WrongCallbackSignatureException( const char* expectedCallbackSignature )
			:m_msg( "CallbackSender::triggerCallback wrong callback signature. Expected Signature:" + std::string( expectedCallbackSignature ) )
		{ }

		virtual const char* what()const override
		{
			return m_msg.c_str();
		};
	};
}

#pragma warning(pop)