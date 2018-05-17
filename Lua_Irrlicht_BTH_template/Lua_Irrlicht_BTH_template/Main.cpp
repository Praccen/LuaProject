#pragma comment(lib, "Irrlicht.lib")
#ifdef _DEBUG
#pragma comment(lib, "LuaLibd.lib")
#else
#pragma comment(lib, "Lualib.lib")
#endif

#include <lua.hpp>
#include <Windows.h>
#include <iostream>
#include <thread>
#include "lua.hpp"
#include <irrlicht.h>

#include "Scene.h"
#include "LuaHandler.h"

int main()
{
	irr::IrrlichtDevice* device = irr::createDevice(irr::video::EDT_SOFTWARE, irr::core::dimension2d<irr::u32>(640, 480), 16, false, false, true, 0);
	if(!device)
		return 1;

	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");
	irr::video::IVideoDriver* driver	= device->getVideoDriver();
	irr::gui::IGUIEnvironment* guienv	= device->getGUIEnvironment();

	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!", irr::core::rect<irr::s32>(10, 10, 260, 22), true);

	Scene scene(device); //Creates scene containing objects and camera
	LuaHandler luaHandler(&scene); //Starts the LuaHandler and it's thread

	while(device->run()) {
		driver->beginScene(true, true, irr::video::SColor(255, 90, 101, 140));
		
		scene.update();
		scene.draw();
		guienv->drawAll();

		driver->endScene();		
	}

	device->drop();

	luaHandler.join(); //Joins LuaHandler thread
	return 0;
}