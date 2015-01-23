#include <iostream>
#include <functional>
#include <sstream>

#include <boost/signals2.hpp>

using namespace std;
using namespace placeholders;

template<int ID>
class Foo
{
public:
	int get()const
	{
		return ID;
	}
};

enum E
{
	A=100
};

int main()
{
	using Type = std::string;

	Type str( "Hello, world!" );
	

	Foo<E::A> f;
	cout << f.get() << endl;


	system( "pause" );
}
