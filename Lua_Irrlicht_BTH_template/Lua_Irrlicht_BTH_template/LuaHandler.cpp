#include "LuaHandler.h"

void ConsoleThread(lua_State* L) {
	char command[1000];
	while (GetConsoleWindow()) {
		memset(command, 0, 1000);
		std::cin.getline(command, 1000);
		if (luaL_loadstring(L, command) || lua_pcall(L, 0, 0, 0))
			std::cout << lua_tostring(L, -1) << '\n';
	}
}

LuaHandler::LuaHandler() {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	m_conThread = std::thread(ConsoleThread, L); //Starts console input thread

	lua_pushcfunction(L, addMesh);
	lua_setglobal(L, "addMesh");
}


LuaHandler::~LuaHandler() {

}

void LuaHandler::join() {
	m_conThread.join();
}

int LuaHandler::addMesh(lua_State * L) {
	luaL_argcheck(L, lua_istable(L, -1), -1, "hej");
	int b = lua_gettable(L, 1);
	//int a = lua_getfield(L, -1, "hej");

	return 0;
}