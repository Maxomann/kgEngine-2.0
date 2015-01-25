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
	A = 100
};

int main()
{
	int chunkSize = 10;

	for( int x = 0; x <= chunkSize; ++x )
		cout << x / chunkSize << ":";
	cout << endl;
	for( int x = 0; x >= chunkSize*-1; --x )
		cout << x / chunkSize << ":";
	cout << endl;


	system( "pause" );
}
