#include "stdafx.h"
#include "Core.h"
#include "Blueprint.h"

using namespace std;
using namespace kg;

int main()
{
	ifstream file;
	file.open( "test.blueprint" );

	vector<string> lines;

	string line;
	while( getline( file, line ) )
	{
		lines.push_back( line );
	}

	blueprint::BlueprintManager manager;
	manager.parse( lines );
	manager.link();

	auto& entities = manager.getEntitiesById();

	system( "pause" );

	Core core;
	core.init();
	while( !core.shouldTerminate() )
		core.update();
}
