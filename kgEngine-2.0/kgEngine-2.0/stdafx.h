#pragma once

#ifndef STDAFX_H
#define STDAFX_H

#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <tuple>
#include <cctype>

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#define variadic ...

//checks if string a contains string b
bool contains( std::string&a, std::string b )
{
	return a.find( b ) != std::string::npos;
}

#endif//STDAFX_H