#include "stdafx.h"
#include "Core.h"

using namespace std;
using namespace kg;

int main()
{
	Core core;
	core.init();
	while( !core.shouldTerminate() )
		core.update();
}