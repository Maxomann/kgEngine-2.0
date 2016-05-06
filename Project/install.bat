::SFML
COPY ".\SFML\lib\Debug\sfml-audio-d-2.dll" "C:\Windows\SysWOW64\"
COPY ".\SFML\lib\Debug\sfml-graphics-d-2.dll" "C:\Windows\SysWOW64\"
COPY ".\SFML\lib\Debug\sfml-network-d-2.dll" "C:\Windows\SysWOW64\"
COPY ".\SFML\lib\Debug\sfml-system-d-2.dll" "C:\Windows\SysWOW64\"
COPY ".\SFML\lib\Debug\sfml-window-d-2.dll" "C:\Windows\SysWOW64\"

COPY ".\SFML\lib\Debug\sfml-audio-d-2.dll" "..\Build\Debug\"
COPY ".\SFML\lib\Debug\sfml-graphics-d-2.dll" "..\Build\Debug\"
COPY ".\SFML\lib\Debug\sfml-network-d-2.dll" "..\Build\Debug\"
COPY ".\SFML\lib\Debug\sfml-system-d-2.dll" "..\Build\Debug\"
COPY ".\SFML\lib\Debug\sfml-window-d-2.dll" "..\Build\Debug\"

COPY ".\SFML\lib\Release\sfml-audio-2.dll" "C:\Windows\SysWOW64\"
COPY ".\SFML\lib\Release\sfml-graphics-2.dll" "C:\Windows\SysWOW64\"
COPY ".\SFML\lib\Release\sfml-network-2.dll" "C:\Windows\SysWOW64\"
COPY ".\SFML\lib\Release\sfml-system-2.dll" "C:\Windows\SysWOW64\"
COPY ".\SFML\lib\Release\sfml-window-2.dll" "C:\Windows\SysWOW64\"

COPY ".\SFML\lib\Release\sfml-audio-2.dll" "..\Build\Release\"
COPY ".\SFML\lib\Release\sfml-graphics-2.dll" "..\Build\Release\"
COPY ".\SFML\lib\Release\sfml-network-2.dll" "..\Build\Release\"
COPY ".\SFML\lib\Release\sfml-system-2.dll" "..\Build\Release\"
COPY ".\SFML\lib\Release\sfml-window-2.dll" "..\Build\Release\"

::SFML-Openal32
COPY ".\SFML\extlibs\bin\x86\openal32.dll" "C:\Windows\SysWOW64\"
COPY ".\SFML\extlibs\bin\x86\openal32.dll" "..\Build\Debug\"
COPY ".\SFML\extlibs\bin\x86\openal32.dll" "..\Build\Release\"

::Glew
COPY ".\glew\bin\Release\Win32\glew32.dll" "..\Build\Debug\"
COPY ".\glew\bin\Release\Win32\glew32.dll" "..\Build\Release\"

::TGUI
COPY ".\TGUI\lib\Debug\tgui-d.dll" "C:\Windows\SysWOW64\"
COPY ".\TGUI\lib\Debug\tgui-d.dll" "..\Build\Debug\"

COPY ".\TGUI\lib\Release\tgui.dll" "C:\Windows\SysWOW64\"
COPY ".\TGUI\lib\Release\tgui.dll" "..\Build\Release\"

::Thor
COPY ".\Thor\src\Debug\thor-d.dll" "C:\Windows\SysWOW64\"
COPY ".\Thor\src\Debug\thor-d.dll" "..\Build\Debug\"

COPY ".\Thor\src\Release\thor.dll" "C:\Windows\SysWOW64\"
COPY ".\Thor\src\Release\thor.dll" "..\Build\Release\"