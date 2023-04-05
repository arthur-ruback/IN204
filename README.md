# IN204
Projet final en C++ pour la classe IN204

Brief Description:
    The program is a multi-user chat with a server.

Requirements and Dependencies:
- cmake
- SFML modules: network
- SDL2 modules: image, ttf

Instalation of dependencies on Linux (Tested on Ubuntu 22.04):
- run "sudo apt-get install dependencies.txt -f"

Compilation:
- First clone repository
- Execute command "cmake CMakeLists.txt"
- Execute command "make all"

Execution:
The program consists of two executables, a server(Server) and a client(StateMachine). 
After the server is up and running, the clients can be executed specifying the
IPV4 adress of the server to directly connect to them as "./StateMachine 123.123.123.123" 

The server prints its IP in the console to make it easier to connect to it.

To talk to other clients, no other IP except the server is required.

The name of the person you would like to talk to when creating a new chat is CASE SENSITIVE and if not specified correctly will not generate a new chat.
    


Execution:

