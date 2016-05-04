::boost
::MANUALLY

::SFML
cmake SFML/CMakeLists.txt -G"Visual Studio 14 2015"
cmake --build SFML --config Debug
cmake --build SFML --config Release

::prevent SFML not being found by cmake find_sfml()
XCOPY "./SFML/lib/Debug" "./SFML/lib" /Y
XCOPY "./SFML/lib/Release" "./SFML/lib" /Y

::Thor
cmake Thor/CMakeLists.txt -G"Visual Studio 14 2015" -DSFML_ROOT="./SFML"
cmake --build Thor --config Debug
cmake --build Thor --config Release

::TGUI
cmake TGUI/CMakeLists.txt -G"Visual Studio 14 2015" -DSFML_ROOT="./SFML"
cmake --build TGUI --config Debug
cmake --build TGUI --config Release