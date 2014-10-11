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
#include <fstream>
#include <typeinfo>
#include <typeindex>
#include <functional>
#include <filesystem>

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#ifdef _WIN32

#elif
#error TARGET_PLATFORM_NOT_SUPPORTED
#endif
#include<windows.h>
#define variadic ...

#endif//STDAFX_H