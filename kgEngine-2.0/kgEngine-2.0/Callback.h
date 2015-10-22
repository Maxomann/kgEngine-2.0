#pragma once
#include "stdafx.h"

template <std::size_t... Is>
struct indices
{ };

template <std::size_t N, std::size_t... Is>
struct build_indices
	: build_indices < N - 1, N - 1, Is... >
{ };

template <std::size_t... Is>
struct build_indices<0, Is...> : indices < Is... >
{ };

template<int I> struct placeholder
{ };

namespace std
{
	template<int I>
	struct is_placeholder< ::placeholder<I>> : std::integral_constant < int, I >
	{ };
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

	template<class T> class DLL_EXPORT MySignal
	{ };

	// Callback receiver MOST NOT be DESTROYED without unregistering
	template<class returnType, class ... parameterType >
	class DLL_EXPORT MySignal < returnType( parameterType... ) >
	{
		std::vector<std::function<returnType( parameterType... )>> m_registeredCallbacks;
		boost::signals2::signal < returnType( parameterType... ), aggregate_values<std::vector<returnType>> > m_safeSignal;

	public:

		void addCallback( std::function<returnType( parameterType... )>& callbackToAdd )
		{
			m_registeredCallbacks.push_back( callbackToAdd );
		};

		void addCallback( std::function<returnType( parameterType... )>&& callbackToAdd )
		{
			m_registeredCallbacks.push_back( callbackToAdd );
		};

		void addCallback_safe( std::function<returnType( parameterType... )>& callbackToAdd, const boost::shared_ptr<bool>& pointerToTrack )
		{
			m_safeSignal.connect( boost::signals2::signal<returnType( parameterType ... )>
								  ::slot_type( callbackToAdd ).track( pointerToTrack ) );
		};

		void addCallback_safe( std::function<returnType( parameterType... )>&& callbackToAdd, const boost::shared_ptr<bool>& pointerToTrack )
		{
			m_safeSignal.connect( boost::signals2::signal<returnType( parameterType ... )>
								  ::slot_type( callbackToAdd ).track( pointerToTrack ) );
		};

		//returns a value ONLY if just ONE function IS REGISTERED
		std::vector<returnType> operator()( parameterType&... Args )const
		{
			std::vector<returnType> vec = m_safeSignal( Args... );

			for( const auto& el : m_registeredCallbacks )
				vec.push_back( m_registeredCallbacks.at( 0 )(Args...) );

			return vec;
		}
	};

	template<class ... parameterType >
	class DLL_EXPORT MySignal < void( parameterType... ) >
	{
		std::vector<std::function<void( parameterType... )>> m_registeredCallbacks;
		boost::signals2::signal < void( parameterType... ) > m_safeSignal;

	public:

		void addCallback( std::function<void( parameterType... )>& callbackToAdd )
		{
			m_registeredCallbacks.push_back( callbackToAdd );
		};

		void addCallback( std::function<void( parameterType... )>&& callbackToAdd )
		{
			m_registeredCallbacks.push_back( callbackToAdd );
		};

		void addCallback_safe( std::function<void( parameterType... )>& callbackToAdd, const boost::shared_ptr<bool>& pointerToTrack )
		{
			m_safeSignal.connect( boost::signals2::signal<void( parameterType ... )>
								  ::slot_type( callbackToAdd ).track( pointerToTrack ) );
		};

		void addCallback_safe( std::function<void( parameterType... )>&& callbackToAdd, const boost::shared_ptr<bool>& pointerToTrack )
		{
			m_safeSignal.connect( boost::signals2::signal<void( parameterType ... )>
								  ::slot_type( callbackToAdd ).track( pointerToTrack ) );
		};

		void operator()( parameterType&... Args )const
		{
			m_safeSignal( Args... );
			for( const auto& func : m_registeredCallbacks )
				func( Args... );
		}
	};

	template< class ... parameterType >
	using Signal = MySignal < void( parameterType... ) >;

	template< class returnType, class ... parameterType >
	using ReturningSignal = MySignal < returnType( parameterType... ) >;

	class DLL_EXPORT CallbackReciever
	{
	public:
		template<class returnType, class ... parameterType >
		void m_connectToSignal(
			MySignal<returnType( parameterType... )>& signal,
			std::function<returnType( parameterType... )>& function )
		{
			signal.addCallback( function );
		};

		template<class className, class returnType, class ... parameterType >
		void m_connectToSignal(
			MySignal<returnType( parameterType... )>& signal,
			returnType( className::* mem_fn_ptr ) (parameterType...) )
		{
			std::function<returnType( className*, parameterType... )> f1 = std::mem_fn( mem_fn_ptr );
			//auto temp = std::bind( f1, static_cast< className* >(this) );
			signal.addCallback( std::move( easy_bind( f1, static_cast< className* >(this) ) ) );
		};
	};

	// Callback receiver CAN be DESTROYED without unregistering
	class DLL_EXPORT SafeCallbackReciever
	{
		const boost::shared_ptr<bool> thisWeakPointer = boost::make_shared<bool>( false );

	protected:
		template<class returnType, class ... parameterType >
		void m_connectToSignal_safe(
			MySignal<returnType( parameterType... )>& signal,
			std::function<returnType( parameterType... )>& function )
		{
			signal.addCallback_safe( function, thisWeakPointer );
		};

		template<class className, class returnType, class ... parameterType >
		void m_connectToSignal_safe(
			MySignal<returnType( parameterType... )>& signal,
			returnType( className::* mem_fn_ptr ) (parameterType...) )
		{
			std::function<returnType( className*, parameterType... )> f1 = std::mem_fn( mem_fn_ptr );
			std::function<returnType( parameterType... )> f2 = easy_bind( f1, static_cast< className* >(this) );

			signal.addCallback_safe( std::move( f2 ), thisWeakPointer );
		};
	};
}
