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
	//// hej
	m_conThread = std::thread(ConsoleThread, L); //Starts console input thread
}


LuaHandler::~LuaHandler() {

}

void LuaHandler::join() {
	m_conThread.join();
}