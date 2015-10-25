#include "stdafx.h"

using namespace std;
using namespace sf;
using namespace tgui;
using namespace placeholders;

int main()
{
	vector<int> vec;
	for( int i = 0; i < 15000; ++i )
	{
		vec.push_back( 1 );
		vec.push_back( 2 );
	}

	vector<int> vec2;
	vec2.resize( 30000 );
	for( int i = 0; i < 15000; ++i )
	{
		vec2.push_back( 1 );
		vec2.push_back( 2 );
	}





	system( "pause" );

}
