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
