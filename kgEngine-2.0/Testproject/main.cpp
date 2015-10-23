#include "stdafx.h"

using namespace std;
using namespace sf;
using namespace tgui;
using namespace placeholders;

class Foo
{
public:
	virtual int operation() = 0;
};

class Bar : public Foo
{
	virtual int operation() override
	{
		return 1;
	}

};

int main()
{
	unique_ptr<Foo> ptr = make_unique<Bar>();

	cout << ptr->operation() << endl;

	system( "pause" );

}
