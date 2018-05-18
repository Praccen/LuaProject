#include "LuaHandler.h"

Scene* LuaHandler::m_scene;

void ConsoleThread(lua_State* L) {
	char command[1000];
	while (GetConsoleWindow()) {
		memset(command, 0, 1000);
		std::cin.getline(command, 1000);
		if (luaL_loadstring(L, command) || lua_pcall(L, 0, 0, 0))
			std::cout << lua_tostring(L, -1) << '\n';
	}
}

LuaHandler::LuaHandler(Scene* scene) {
	m_scene = scene;

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	m_conThread = std::thread(ConsoleThread, L); //Starts console input thread

	lua_pushcfunction(L, addMesh);
	lua_setglobal(L, "addMesh");
	lua_pushcfunction(L, addBox);
	lua_setglobal(L, "addBox");
	lua_pushcfunction(L, camera);
	lua_setglobal(L, "camera");
	lua_pushcfunction(L, getNodes);
	lua_setglobal(L, "getNodes");
	lua_pushcfunction(L, snapshot);
	lua_setglobal(L, "snapshot");
}


LuaHandler::~LuaHandler() {

}

void LuaHandler::join() {
	m_conThread.join();
}

//Test data
//addMesh({{0,0,0}, {5,0,0}, {0,0,5}})		 --Simple triangle in the plane
//addMesh({{ "orange" }, {}, {}})              --Error: non - numeric coordinates
//addMesh({{ 1,2,3,4 },{},{}})               --Error : number of components
//addMesh({{ 1,2,3 }})                       --Error : not a valid number of vertices
int LuaHandler::addMesh(lua_State * L) {
	luaL_argcheck(L, lua_istable(L, -1), -1, "Error - Expected table");
	int input = lua_gettop(L);

	//Verifying length of input to be mod(3) arguments
	lua_len(L, input);
	int totLen = (int)lua_tonumber(L, -1);
	luaL_argcheck(L, totLen%3 == 0, -1, "Error - Wrong number of vertices");
	int k = 1;
	std::vector<std::vector<irr::core::vector3df>> vertices;
	//Getting data for first vertex
	while (totLen > k) {
		lua_rawgeti(L, input, k++);//Pushing first table of vertex list on lua stack
		int first = lua_gettop(L);//Saving first table
		irr::core::vector3df vertex1 = createVector3(L, first);//creating vector with first table

		//Getting data for second vertex
		lua_rawgeti(L, input, k++);
		int second = lua_gettop(L);
		irr::core::vector3df vertex2 = createVector3(L, second);

		//Getting data for third vertex
		lua_rawgeti(L, input, k++);
		int third = lua_gettop(L);
		irr::core::vector3df vertex3 = createVector3(L, third);
		
		std::vector<irr::core::vector3df> temp;
		temp.push_back(vertex1);
		temp.push_back(vertex2);
		temp.push_back(vertex3);
		vertices.push_back(temp);
	}

	m_scene->addMesh(vertices);

	return 0;
}

int LuaHandler::addBox(lua_State * L) {

	int top = lua_gettop(L);
	
	//Error checking
	luaL_argcheck(L, top > 1 && top < 4, -1, "Error - Invalid number of arguments");
	luaL_argcheck(L, lua_istable(L, 1), -1, "Error - Expected table in first argument");
	luaL_argcheck(L, lua_isnumber(L, 2), -1, "Error - Expected number in second argument");
	if (top == 3) {
		luaL_argcheck(L, lua_type(L, 3) == LUA_TSTRING, -1, "Error - Expected string in third argument");
	}

	//Getting first argument - pos
	irr::core::vector3df pos = createVector3(L, 1);

	//Getting second argument - size
	float size = (float)lua_tonumber(L, 2);

	//Getting third argument - name
	std::string name = "";
	if (top == 3) {
		name = (std::string)lua_tostring(L, 3);
	}

	m_scene->addBox(pos, size, name);

	return 0;
}

//TEST DATA
/*
camera({0,5,-20}, {0,0,0})
*/
int LuaHandler::camera(lua_State * L) {
	int top = lua_gettop(L);
	
	//Error checking
	luaL_argcheck(L, top == 2, -1, "Error - Invalid number of arguments");

	//Getting coordinates for camera position
	irr::core::vector3df pos = createVector3(L, 1);

	//Getting coordinates for target position
	irr::core::vector3df target = createVector3(L, 2);

	m_scene->setCamera(pos, target);
	
	return 0;
}

int LuaHandler::getNodes(lua_State * L) {
	
	std::vector<nodeInfo> temp = m_scene->getNodes();

	lua_newtable(L);
	int top = lua_gettop(L);

	int k = 1;
	for (int i = 0; i < temp.size(); i++) {
		lua_pushnumber(L, k);
		lua_newtable(L);
		int current = lua_gettop(L);
		lua_pushstring(L, "name");
		lua_pushstring(L, temp.at(i).name.c_str());
		lua_settable(L, current);
		lua_settable(L, top);
		k++;
		lua_pushnumber(L, k);
		lua_newtable(L);
		current = lua_gettop(L);
		lua_pushstring(L, "id");
		lua_pushnumber(L, temp.at(i).id);
		lua_settable(L, current);
		lua_settable(L, top);
		k++;
	}

	return 1;
}

int LuaHandler::snapshot(lua_State * L) {
	luaL_argcheck(L, lua_type(L, 1) == LUA_TSTRING, -1, "Error - expected string");
	std::string file = lua_tostring(L, 1);

	int result = m_scene->snapshot(file);
	luaL_argcheck(L, result == 1, -1, "Error - file could not be created");

	return 0;
}

irr::core::vector3df LuaHandler::createVector3(lua_State * L, int idx) {
	float x, y, z;
	luaL_argcheck(L, lua_istable(L, idx), -1, "Error - Vector must be a table");
	lua_len(L, idx);
	int len = (int)lua_tonumber(L, -1);
	luaL_argcheck(L, len == 3, -1, "Error - Vector has to have 3 arguments");
	lua_pop(L, 1);
	
	lua_rawgeti(L, idx, 1);
	luaL_argcheck(L, lua_isinteger(L, -1), -1, "Error - Vector has to have numerical entries");
	x = (float)lua_tonumber(L, -1);
	lua_rawgeti(L, idx, 2);
	luaL_argcheck(L, lua_isinteger(L, -1), -1, "Error - Vector has to have numerical entries");
	y = (float)lua_tonumber(L, -1);
	lua_rawgeti(L, idx, 3);
	luaL_argcheck(L, lua_isinteger(L, -1), -1, "Error - Vector has to have numerical entries");
	z = (float)lua_tonumber(L, -1);
	lua_pop(L, 3);
	irr::core::vector3df vector(x, y, z);
	
	return vector;
}