#pragma once
#include <boost/signals2.hpp>

#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>

template <std::size_t... Is>
struct indices
{ };

template <std::size_t N, std::size_t... Is>
struct build_indices
	: build_indices < N - 1, N - 1, Is... >
{ };

template <std::size_t... Is>
struct build_indices<0, Is...> : indices < Is... > { };

template<int I> struct placeholder
{ };

namespace std
{
	template<int I>
	struct is_placeholder< ::placeholder<I>> : std::integral_constant < int, I > { };
} // std::

namespace detail
{
	template<std::size_t... Is, class F, class... Args>
	auto easy_bind( indices<Is...>, F const& f, Args&&... args )
		-> decltype(std::bind( f, std::forward<Args>( args )..., placeholder < 1 + Is > {}... ))
	{
		return std::bind( f, std::forward<Args>( args )..., placeholder < 1 + Is > {}... );
	}
} // detail::

template<class R, class... FArgs, class... Args>
auto easy_bind( std::function<R( FArgs... )> const& f, Args&&... args )
-> decltype(detail::easy_bind( build_indices < sizeof...( FArgs )-sizeof...(Args) > {}, f, std::forward<Args>( args )... ))
{
	return detail::easy_bind( build_indices < sizeof...( FArgs )-sizeof...(Args) > {}, f, std::forward<Args>( args )... );
}

namespace kg
{
#define signals public

	// COMBINER FOR SIGNALS:
	// aggregate_values is a combiner which places all the values returned
	// from slots into a container
	template<typename Container>
	struct aggregate_values
	{
		typedef Container result_type;

		template<typename InputIterator>
		Container operator()( InputIterator first, InputIterator last ) const
		{
			Container values;

			while( first != last )
			{
				values.push_back( *first );
				++first;
			}
			return values;
		}
	};


	template< class ... parameterType >
	using Signal = boost::signals2::signal < void( parameterType... ) >;

	class CallbackReciever
	{
		const std::shared_ptr<int> thisWeakPointer = std::make_shared<int>( 0 );

	protected:
		template<class returnType, class ... parameterType >
		void m_connectToSignal(
			boost::signals2::signal<returnType( parameterType... )>& signal,
			std::function<returnType( parameterType... )>& function )
		{
			signal.connect( boost::signals2::signal<returnType( parameterType ... )>
							::slot_type( function ).track_foreign( thisWeakPointer ) );
		};

		template<class className, class returnType, class ... parameterType >
		void m_connectToSignal(
			boost::signals2::signal<returnType( parameterType... )>& signal,
			returnType( className::* mem_fn_ptr ) (parameterType...) )
		{
			std::function<returnType( className*, parameterType... )> f1 = std::mem_fn( mem_fn_ptr );
			std::function<returnType( parameterType... )> f2 = easy_bind( f1, static_cast< className* >(this) );

			m_connectToSignal( signal, f2 );
		};
	};
}

#ifdef KG_OLD_CALLBACK_SYSTEM

#include "stdafx.h"
#include "WrongCallbackSignatureException.h"

using namespace std;

// EXAMPLE OF USAGE:
// class Foo : public CallbackSender
// {
// public:
// 	void go()
// 	{
// 		triggerCallback<sf::Vector2i&, int>( 100, sf::Vector2i( 50, 66 ), 5235 );
// 	};
// };
//
// class Bar : public CallbackReciever
// {
// public:
// 	void functionToCallback( int callbackId, sf::Vector2i& myVector, int test )
// 	{
// 		cout << myVector.x << endl << myVector.y << endl << test << endl;
// 	}
//
// 	void registerCallback( CallbackSender& sender )
// 	{
// 		sender.registerCallback_2<Bar, sf::Vector2i, int>( 100,
// 															this,
// 															&Bar::functionToCallback );
// 	}
// };

// CALLBACK ARGUMENTS ARE ALLWAYS PASSED AS NON CONST REFERENCE
// DO NOT WRITE
// sender.registerCallback_2<Bar, const sf::Vector2i>
// OR
// sender.registerCallback_2<Bar, sf::Vector2i&>
//
// ALWAYS USE:
// callbackFuntion(int callbackId, sf::Vector2i&)
// sender.registerCallback_2<Bar, sf::Vector2i>(100, this, &Class::CallbackFuntion);

namespace kg
{
	class Entity;

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
		typedef std::list<std::pair<std::weak_ptr<bool>, std::pair<std::shared_ptr<void>/*std::function<void( int, T&... )>*/, size_t> > > CallbackList;
		std::map<int, CallbackList> m_callbacksById;

	protected:
		template<class variadic T>
		void triggerCallback( const int callbackID, T&... args )
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
					if( it->second.second == typeid(std::shared_ptr<std::function<void( int )>>).hash_code() )
					{
						//if callback function should ignore callback arguments
						auto function = std::static_pointer_cast< std::function<void( int )> >(it->second.first);
						(*function)(callbackID);
					}
					else
					{
						std::shared_ptr< std::function<void( int, T&... )> > function = std::static_pointer_cast< std::function<void( int, T&... )> >(it->second.first);
						if( it->second.second != typeid(function).hash_code() )
						{
							//if callback function has wrong signature
							auto expectedSignature = typeid(function).name();
							throw WrongCallbackSignatureException( expectedSignature );
						}

						//if callback function has correct signature
						(*function)(callbackID, args...);
					}
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
							   std::shared_ptr< std::function<void( int, T&... )> > callbackFunction )
		{
			m_callbacksById[callbackId].push_back(
				std::make_pair( thisPointer->getWeakPointer(),
				std::make_pair( std::static_pointer_cast< void >(callbackFunction), typeid(callbackFunction).hash_code() ) ) );
		};

		//if you register a callback twice it will be called twice
		template<class variadic T>
		void registerCallback( int callbackId,
							   const CallbackReciever* thisPointer,
							   const std::function<void( int, T&... )>& callbackFunction )
		{
			auto callbackPointer = std::make_shared<std::function<void( int, T&... )>>( callbackFunction );

			registerCallback( callbackId, thisPointer, callbackPointer );
		};

		template<class ClassName>
		void registerCallback_0_entity( int callbackId,
										ClassName* thisPointer,
										std::shared_ptr<Entity>& entity,
										void(ClassName::* mem_fn_ptr) (int) )
		{
			auto func = std::make_shared<std::function<void( int )> >( std::bind(
				mem_fn_ptr,
				this,
				placeholders::_1,
				entity ) );

			registerCallback( callbackId,
							  thisPointer,
							  func );
		}

		template<class ClassName, class variadic T>
		void registerCallback_1_entity( int callbackId,
										ClassName* thisPointer,
										std::shared_ptr<Entity>& entity,
										void(ClassName::* mem_fn_ptr) (int, std::shared_ptr<Entity>&, T&...) )
		{
			auto func = std::make_shared<std::function<void( int, T&... )> >( std::bind(
				mem_fn_ptr,
				thisPointer,
				placeholders::_1,
				entity,
				placeholders::_2 ) );

			registerCallback<T...>( callbackId,
									thisPointer,
									func );
		}

		template<class ClassName, class variadic T>
		void registerCallback_2_entity( int callbackId,
										ClassName* thisPointer,
										std::shared_ptr<Entity>& entity,
										void(ClassName::* mem_fn_ptr) (int, std::shared_ptr<Entity>&, T&...) )
		{
			auto func = std::make_shared<std::function<void( int, T&... )> >( std::bind(
				mem_fn_ptr,
				thisPointer,
				placeholders::_1,
				entity,
				placeholders::_2,
				placeholders::_3 ) );

			registerCallback<T...>( callbackId,
									thisPointer,
									func );
		}

		template<class ClassName, class variadic T>
		void registerCallback_3_entity( int callbackId,
										ClassName* thisPointer,
										std::shared_ptr<Entity>& entity,
										void(ClassName::* mem_fn_ptr) (int, std::shared_ptr<Entity>&, T&...) )
		{
			auto func = std::make_shared<std::function<void( int, T&... )> >( std::bind(
				mem_fn_ptr,
				thisPointer,
				placeholders::_1,
				entity,
				placeholders::_2,
				placeholders::_3,
				placeholders::_4 ) );

			registerCallback<T...>( callbackId,
									thisPointer,
									func );
		}

		template<class ClassName, class variadic T>
		void registerCallback_4_entity( int callbackId,
										ClassName* thisPointer,
										std::shared_ptr<Entity>& entity,
										void(ClassName::* mem_fn_ptr) (int, std::shared_ptr<Entity>&, T&...) )
		{
			auto func = std::make_shared<std::function<void( int, T&... )> >( std::bind(
				mem_fn_ptr,
				thisPointer,
				placeholders::_1,
				entity,
				placeholders::_2,
				placeholders::_3,
				placeholders::_4,
				placeholders::_5 ) );

			registerCallback<T...>( callbackId,
									thisPointer,
									func );
		}

		template<class ClassName, class variadic T>
		void registerCallback_5_entity( int callbackId,
										ClassName* thisPointer,
										std::shared_ptr<Entity>& entity,
										void(ClassName::* mem_fn_ptr) (int, std::shared_ptr<Entity>&, T&...) )
		{
			auto func = std::make_shared<std::function<void( int, T&... )> >( std::bind(
				mem_fn_ptr,
				thisPointer,
				placeholders::_1,
				entity,
				placeholders::_2,
				placeholders::_3,
				placeholders::_4,
				placeholders::_5,
				placeholders::_6 ) );

			registerCallback<T...>( callbackId,
									thisPointer,
									func );
		}

		//number behind functionName is the number of additional arguments
		template<class ClassName>
		void registerCallback_0( int callbackId,
								 ClassName* thisPointer,
								 void(ClassName::* mem_fn_ptr) (int) )
		{
			registerCallback( callbackId,
							  thisPointer,
							  std::make_shared<std::function<void( int )> >(
							  std::bind(
							  mem_fn_ptr,
							  thisPointer,
							  std::placeholders::_1 ) ) );
		}

		//number behind functionName is the number of additional arguments
		template<class ClassName, class variadic T>
		void registerCallback_1( int callbackId,
								 ClassName* thisPointer,
								 void(ClassName::* mem_fn_ptr) (int, T&...) )
		{
			registerCallback( callbackId,
							  thisPointer,
							  std::make_shared<std::function<void( int, T&... )> >(
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
								 void(ClassName::* mem_fn_ptr) (int, T&...) )
		{
			registerCallback( callbackId,
							  thisPointer,
							  std::make_shared<std::function<void( int, T&... )> >(
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
								 void(ClassName::* mem_fn_ptr) (int, T&...) )
		{
			registerCallback( callbackId,
							  thisPointer,
							  std::make_shared<std::function<void( int, T&... )> >(
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
								 void(ClassName::* mem_fn_ptr) (int, T&...) )
		{
			registerCallback( callbackId,
							  thisPointer,
							  std::make_shared<std::function<void( int, T&... )> >(
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
								 void(ClassName::* mem_fn_ptr) (int, T&...) )
		{
			registerCallback( callbackId,
							  thisPointer,
							  std::make_shared<std::function<void( int, T&... )> >(
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
								 void(ClassName::* mem_fn_ptr) (int, T&...) )
		{
			registerCallback( callbackId,
							  thisPointer,
							  std::make_shared<std::function<void( int, T&... )> >(
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
								 void(ClassName::* mem_fn_ptr) (int, T&...) )
		{
			registerCallback( callbackId,
							  thisPointer,
							  std::make_shared<std::function<void( int, T&... )> >(
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
								 void(ClassName::* mem_fn_ptr) (int, T&...) )
		{
			registerCallback( callbackId,
							  thisPointer,
							  std::make_shared<std::function<void( int, T&... )> >(
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
								 void(ClassName::* mem_fn_ptr) (int, T&...) )
		{
			registerCallback( callbackId,
							  thisPointer,
							  std::make_shared<std::function<void( int, T&... )> >(
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

#endif