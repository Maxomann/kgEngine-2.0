#pragma once
#include "stdafx.h"

namespace kg
{
	/// A random number generator.
	///
	/// @author	Kay
	/// @date	27.01.2015

	class DLL_EXPORT RandomNumberGenerator
	{
		std::random_device m_rdv;   // The rdv
		std::mt19937 m_rng; // The random number generator

		/**********************************************************************************************//**
		 * Re seed the random number generator. Can be called for 'more random' numbers. Otherwise only
		 * needs to be called in the constructor.
		 *
		 * @author	Kay
		 * @date	27.01.2015
		 **************************************************************************************************/

		void m_reSeed();

	public:

		/**********************************************************************************************//**
		 * Default constructor. Seeds the random device.
		 *
		 * @author	Kay
		 * @date	27.01.2015
		 **************************************************************************************************/

		RandomNumberGenerator();

		/**********************************************************************************************//**
		 * Gets random int.
		 *
		 * @author	Kay
		 * @date	27.01.2015
		 *
		 * @tparam	T	Type of the number to generate. Can only be: short, int, long, long long, unsigned short, unsigned int, unsigned long, or unsigned long long.
		 *
		 * @return	A random integer.
		 **************************************************************************************************/

		template< class T = int >
		T getRandomInt()
		{
			return std::uniform_int_distribution< T >()(rng);
		}

		/**********************************************************************************************//**
		 * Gets random int.
		 *
		 * @author	Kay
		 * @date	27.01.2015
		 *
		 * @tparam	T	Type of the number to generate. Can only be: short, int, long, long long, unsigned short, unsigned int, unsigned long, or unsigned long long.
		 * @param	from	Number range from.
		 * @param	to  	Number range to.
		 *
		 * @return	The random int.
		 **************************************************************************************************/

		template< class T = int >
		T getRandomInt( T from, T to )
		{
			return std::uniform_int_distribution< T >( from, to )(m_rng);
		}

		/**********************************************************************************************//**
		 * Gets random float.
		 *
		 * @author	Kay
		 * @date	27.01.2015
		 *
		 * @tparam	T	Type of the number to generate. Can only be: float, double, or long double.
		 *
		 * @return	The random float.
		 **************************************************************************************************/

		template< class T = double >
		T getRandomFloat()
		{
			return std::uniform_real_distribution< T >()(rng);
		}

		/**********************************************************************************************//**
		 * Gets random float.
		 *
		 * @author	Kay
		 * @date	27.01.2015
		 *
		 * @tparam	T	Type of the number to generate. Can only be: float, double, or long double.
		 * @param	from	Number range from.
		 * @param	to  	Number range to.
		 *
		 * @return	The random float.
		 **************************************************************************************************/

		template< class T = double >
		T getRandomFloat( T from, T to )
		{
			return std::uniform_real_distribution< T >( from, to )(m_rng);
		}
	};
};//kg
