#include "stdafx.h"
#include "Core.h"

using namespace std;
using namespace kg;

int main()
{
	Core core;
	while( !core.shouldTerminate() )
		core.update();

	system( "pause" );
}