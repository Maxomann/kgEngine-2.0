#include "RandomNumberGenerator.h"
using namespace std;

namespace kg
{


	RandomNumberGenerator::RandomNumberGenerator()
	{
		m_reSeed();
	}

	void RandomNumberGenerator::m_reSeed()
	{
		m_rng.seed( m_rdv() );
	}

}

