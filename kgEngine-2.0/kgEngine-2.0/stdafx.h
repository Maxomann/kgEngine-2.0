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
#include <set>
#include <unordered_set>
#include <atomic>
#include <iomanip>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <ctime>

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <gl/glew.h>
//#include <gl/glut.h>
#include <SFML/OpenGL.hpp>

#include <boost/core/noncopyable.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/remove_if.hpp>
#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/container/static_vector.hpp>
#include <boost/container/slist.hpp>
#include <boost/container/deque.hpp>
#include <boost/signals2.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <Thor/Input.hpp>

#include <TGUI/TGUI.hpp>

#ifdef _WIN32
#include<windows.h>
#elif
#error TARGET_PLATFORM_NOT_SUPPORTED
#endif

#define variadic ...
#define DLL_EXPORT __declspec(dllexport)

template< class T >
const std::size_t getRuntimeTypeInfo()
{
	return typeid(T).hash_code();
}

#endif//STDAFX_H