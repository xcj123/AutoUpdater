g++ -m32 -fPIC -g -c -shared -std=c++0x -I../app/includes/autoupdater/ -I./includes src/*.cpp
g++ -m32 -g -shared -o steamcmd.so *.o