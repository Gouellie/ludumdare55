# Download this repository
You can either download the zip file, or clone the repository.

# Generate Projects
For windows users, there are two batch files you can use depending on what compiler you are using. For linux users you can simply use a terminal.
Only do ONE of these options depending on your compiler and platform.
## Windows Users
Visual Studio users should run

    premake-VisualStudio.bat
	
This will generate a Visual Studio project (and download Raylib).
	
## MinGW-w64 Users
Please make sure you have a recent version of MinGW-W64. The older versons from mingw.org will not work.
We recomend the W64Devkit. It has everything needed to build raylib. It can be downloaded from here https://github.com/skeeto/w64devkit/releases

Once you have MinGW-W64
Run the batch file.

    premake-mingw.bat

This will generate a makefile for you
	
## Linux users
cd to the root folder and run

    ./premake5 gmake2

This will generate a makefile for you.

# Build your game
Only do ONE of these options depending on your compiler and platform.
## Windows Users
Double click the .sln file that was generated in the folder. From here you can use the project as normal.
	
## MinGW-w64 Users
Open your compiler terminal (w64devkit if you are using it), change to the game folder and type 

    make
	
This will build your game
	
## Linux/macOS users
Open your terminal, change to the root folder and type.

    make
	
This will build your starting game template
	
# Building extra libs
If you need to add a separate library to your game you can do that very easily.
Simply copy the extras/example_library folder and rename it to what you want your lib to be called.
Replace lib.c with the sources for your library (just copy them in the folder).
If you library has an include folder, copy that too.
Then go to the premake5.lua file in the game folder, and link your library by calling link_to with the folder name for the library.

link_to("LIB_FOLDER_NAME")

Rerun premake and it will build your library for you.
Note that by default link_to will add include dirs for your library folder and library/include. If you have other include needs you will have to add those to your premake file manually.