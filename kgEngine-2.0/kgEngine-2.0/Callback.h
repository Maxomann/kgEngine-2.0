#pragma once
#include "stdafx.h"

// EXAMPLE OF USAGE:
// class Foo : public CallbackSender
// {
// public:
// 	void go()
// 	{
// 		triggerCallback<const sf::Vector2i&, int>( 100, sf::Vector2i( 50, 66 ), 5235 );
// 	};
// };
//
// class Bar : public CallbackReciever
// {
// public:
// 	void functionToCallback( int callbackId, const sf::Vector2i& myVector, int test )
// 	{
// 		cout << myVector.x << endl << myVector.y << endl << test << endl;
// 	}
//
// 	void registerCallback( CallbackSender& sender )
// 	{
// 		sender.registerCallback_2<Bar, const sf::Vector2i&, int>( 100,
// 																  this,
// 																  &Bar::functionToCallback );
// 	}
// };

namespace kg
{
	class DLL_EXPORT CallbackReciever
	{
		// this pointer is used in aCallbackSender to check
		// if the Object, which member function should be called, still exists
		const std::shared_ptr<bool> m_CallbackRecieverDummyObject;

	public:
		CallbackReciever();

		std::weak_ptr<bool> getWeakPointer()const;
	};

	//T=AdditionalCallbackArguments
	class DLL_EXPORT CallbackSender
	{
		typedef std::list<std::pair<std::weak_ptr<bool>, std::pair<std::shared_ptr<void>/*std::function<void( int, T... )>*/, size_t> > > CallbackList;
		std::map<int, CallbackList> m_callbacksById;

	protected:
		template<class variadic T>
		void triggerCallback( const int callbackID, T... args )
		{
			auto& list = m_callbacksById[callbackID];
			std::vector < CallbackList::const_iterator > invalidReferences;
			for( auto it = begin( list ); it != end( list ); ++it )
			{
				const std::shared_ptr<bool> ptr( it->first.lock() );

				//object on which the callback should be called exists
				//it->first.lock() returns a std::shared_ptr
				//in the if, the shared_ptr checks if the memory its pointing to still exists
				if( ptr.operator bool() )
				{
					//call callback
					//pass callbackId
					auto function = std::static_pointer_cast< std::function<void( int, T... )> >(it->second.first);
					if( it->second.second != typeid(function).hash_code() )
						throw std::bad_function_call( "CallbackSender::triggerCallback wrong callback signature" );
					(*function)(callbackID, args...);
				}
				//object has been deleted
				else
				{
					//add iterator to invalidReferences
					invalidReferences.push_back( it );
				}
			}

			//remove all references on to be removed list
			for( const auto& el : invalidReferences )
				list.erase( el );

			return;
		};

	public:
		//if you register a callback twice it will be called twice
		template<class variadic T>
		void registerCallback( int callbackId,
							   const CallbackReciever* thisPointer,
							   std::shared_ptr< std::function<void( int, T... )> > callbackFunction )
		{
			m_callbacksById[callbackId].push_back(
				std::make_pair( thisPointer->getWeakPointer(),
				std::make_pair( std::static_pointer_cast< void >(callbackFunction), typeid(callbackFunction).hash_code() ) ) );
		};

		//number behind functionName is the number of additional arguments
		template<class ClassName, class variadic T>
		void registerCallback_0( int callbackId,
								 ClassName* thisPointer,
								 void(ClassName::* mem_fn_ptr) (int, T...) )
		{
			registerCallback( callbackId,
							  thisPointer,
							  std::make_shared<std::function<void( int, T... )> >(
							  std::bind(
							  mem_fn_ptr,
							  thisPointer,
							  std::placeholders::_1 ) ) );
		}

		//number behind functionName is the number of additional arguments
		template<class ClassName, class variadic T>
		void registerCallback_1( int callbackId,
								 ClassName* thisPointer,
								 void(ClassName::* mem_fn_ptr) (int, T...) )
		{
			registerCallback( callbackId,
							  thisPointer,
							  std::make_shared<std::function<void( int, T... )> >(
							  std::bind(
							  mem_fn_ptr,
							  thisPointer,
							  std::placeholders::_1,
							  std::placeholders::_2 ) ) );
		}

		//number behind functionName is the number of additional arguments
		template<class ClassName, class variadic T>
		void registerCallback_2( int callbackId,
								 ClassName* thisPointer,
								 void(ClassName::* mem_fn_ptr) (int, T...) )
		{
			registerCallback( callbackId,
							  thisPointer,
							  std::make_shared<std::function<void( int, T... )> >(
							  std::bind(
							  mem_fn_ptr,
							  thisPointer,
							  std::placeholders::_1,
							  std::placeholders::_2,
							  std::placeholders::_3 ) ) );
		}

		//number behind functionName is the number of additional arguments
		template<class ClassName, class variadic T>
		void registerCallback_3( int callbackId,
								 ClassName* thisPointer,
								 void(ClassName::* mem_fn_ptr) (int, T...) )
		{
			registerCallback( callbackId,
							  thisPointer,
							  std::make_shared<std::function<void( int, T... )> >(
							  std::bind(
							  mem_fn_ptr,
							  thisPointer,
							  std::placeholders::_1,
							  std::placeholders::_2,
							  std::placeholders::_3,
							  std::placeholders::_4 ) ) );
		}

		//number behind functionName is the number of additional arguments
		template<class ClassName, class variadic T>
		void registerCallback_4( int callbackId,
								 ClassName* thisPointer,
								 void(ClassName::* mem_fn_ptr) (int, T...) )
		{
			registerCallback( callbackId,
							  thisPointer,
							  std::make_shared<std::function<void( int, T... )> >(
							  std::bind(
							  mem_fn_ptr,
							  thisPointer,
							  std::placeholders::_1,
							  std::placeholders::_2,
							  std::placeholders::_3,
							  std::placeholders::_4,
							  std::placeholders::_5 ) ) );
		}

		//number behind functionName is the number of additional arguments
		template<class ClassName, class variadic T>
		void registerCallback_5( int callbackId,
								 ClassName* thisPointer,
								 void(ClassName::* mem_fn_ptr) (int, T...) )
		{
			registerCallback( callbackId,
							  thisPointer,
							  std::make_shared<std::function<void( int, T... )> >(
							  std::bind(
							  mem_fn_ptr,
							  thisPointer,
							  std::placeholders::_1,
							  std::placeholders::_2,
							  std::placeholders::_3,
							  std::placeholders::_4,
							  std::placeholders::_5,
							  std::placeholders::_6 ) ) );
		}

		//number behind functionName is the number of additional arguments
		template<class ClassName, class variadic T>
		void registerCallback_6( int callbackId,
								 ClassName* thisPointer,
								 void(ClassName::* mem_fn_ptr) (int, T...) )
		{
			registerCallback( callbackId,
							  thisPointer,
							  std::make_shared<std::function<void( int, T... )> >(
							  std::bind(
							  mem_fn_ptr,
							  thisPointer,
							  std::placeholders::_1,
							  std::placeholders::_2,
							  std::placeholders::_3,
							  std::placeholders::_4,
							  std::placeholders::_5,
							  std::placeholders::_6,
							  std::placeholders::_7 ) ) );
		}

		//number behind functionName is the number of additional arguments
		template<class ClassName, class variadic T>
		void registerCallback_7( int callbackId,
								 ClassName* thisPointer,
								 void(ClassName::* mem_fn_ptr) (int, T...) )
		{
			registerCallback( callbackId,
							  thisPointer,
							  std::make_shared<std::function<void( int, T... )> >(
							  std::bind(
							  mem_fn_ptr,
							  thisPointer,
							  std::placeholders::_1,
							  std::placeholders::_2,
							  std::placeholders::_3,
							  std::placeholders::_4,
							  std::placeholders::_5,
							  std::placeholders::_6,
							  std::placeholders::_7,
							  std::placeholders::_8 ) ) );
		}

		//number behind functionName is the number of additional arguments
		template<class ClassName, class variadic T>
		void registerCallback_8( int callbackId,
								 ClassName* thisPointer,
								 void(ClassName::* mem_fn_ptr) (int, T...) )
		{
			registerCallback( callbackId,
							  thisPointer,
							  std::make_shared<std::function<void( int, T... )> >(
							  std::bind(
							  mem_fn_ptr,
							  thisPointer,
							  std::placeholders::_1,
							  std::placeholders::_2,
							  std::placeholders::_3,
							  std::placeholders::_4,
							  std::placeholders::_5,
							  std::placeholders::_6,
							  std::placeholders::_7,
							  std::placeholders::_8,
							  std::placeholders::_9 ) ) );
		}

		//number behind functionName is the number of additional arguments
		template<class ClassName, class variadic T>
		void registerCallback_9( int callbackId,
								 ClassName* thisPointer,
								 void(ClassName::* mem_fn_ptr) (int, T...) )
		{
			registerCallback( callbackId,
							  thisPointer,
							  std::make_shared<std::function<void( int, T... )> >(
							  std::bind(
							  mem_fn_ptr,
							  thisPointer,
							  std::placeholders::_1,
							  std::placeholders::_2,
							  std::placeholders::_3,
							  std::placeholders::_4,
							  std::placeholders::_5,
							  std::placeholders::_6,
							  std::placeholders::_7,
							  std::placeholders::_8,
							  std::placeholders::_9,
							  std::placeholders::_10 ) ) );
		}
	};
}
