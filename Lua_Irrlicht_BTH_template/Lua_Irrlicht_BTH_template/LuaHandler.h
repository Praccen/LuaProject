#pragma once

#ifdef _DEBUG
#pragma comment(lib, "LuaLibd.lib")
#else
#pragma comment(lib, "Lualib.lib")
#endif

#include <lua.hpp>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <irrlicht.h>
#include "lua.hpp"

#include "Scene.h"

class LuaHandler {
private:
	static Scene * m_scene;
	lua_State * m_luaState;
	std::thread m_conThread;

public:
	LuaHandler(Scene* scene);
	~LuaHandler();

	static int addMesh(lua_State * L);
	static int addBox(lua_State * L);

	void join();
};