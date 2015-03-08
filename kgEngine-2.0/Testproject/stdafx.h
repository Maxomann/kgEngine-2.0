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
#include <cstdint>
#include <list>
#include <forward_list>
#include <random>
#include <queue>
#include <thread>
#include <mutex>
#include <stack>
#include <algorithm>
#include <unordered_set>

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <boost/core/noncopyable.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/remove_if.hpp>
#include <boost/optional.hpp>

#ifdef _WIN32
#include<windows.h>
#elif
#error TARGET_PLATFORM_NOT_SUPPORTED
#endif

#define variadic ...
#define DLL_EXPORT __declspec(dllexport)

#endif//STDAFX_H