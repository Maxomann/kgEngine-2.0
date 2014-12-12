#include <iostream>
#include <functional>
#include <boost/signals2.hpp>

using namespace std;
using namespace placeholders;

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

class CallbackReciever
{
	const std::shared_ptr<int > thisWeakPointer = std::make_shared<int>( 0 );

protected:
	template<class returnType, class ... parameterType >
	void m_connectToSignal(
		boost::signals2::signal<returnType( parameterType... )>& signal,
		const std::function<returnType( parameterType... )>& function )
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

template<class ... parameterType >
using Signal = boost::signals2::signal < void( parameterType... ) > ;

class Foo
{
public:

	Signal<string, string, int> mySignal;
};

class Bar : public CallbackReciever
{
	std::shared_ptr<int> thisWeakPointer = make_shared<int>( 0 );

	int m_i = 5000;

	void myCallback( string a, string b, int c )
	{
		cout << a << b << c << m_i << endl;
	};

public:

	void connectIt( Foo& sender )
	{
		m_connectToSignal( sender.mySignal, &Bar::myCallback );
	}
};

int main()
{
	cout << std::atoi( "ga435ga" ) << endl;

	system( "pause" );
}