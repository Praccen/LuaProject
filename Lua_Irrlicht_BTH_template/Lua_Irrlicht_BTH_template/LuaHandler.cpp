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
	int len;
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
		//Checking that first index is a table
		luaL_argcheck(L, lua_istable(L, -1), -1, "Error - vertex has to be a table");
		//Checking the length of the first vertex to be 3
		lua_len(L, first);//Pushing the length of the first table on the lua stack
		len = (int)lua_tonumber(L, -1);//Retrieving number on top of stack
		luaL_argcheck(L, len == 3, -1, "Error - vertex has to have 3 coordinates");
		//Getting first index
		int i = 1;
		lua_rawgeti(L, first, i++);//Pushing first index of first table on lua stack
		luaL_argcheck(L, lua_isinteger(L, -1), -1, "Error - First point has to be a number");
		float x = (float)lua_tonumber(L, -1);//Retrieving number on top of stack
		//Second index
		lua_rawgeti(L, first, i++);//Pushing second index of first table on lua stack
		luaL_argcheck(L, lua_isinteger(L, -1), -1, "Error - Second point has to be a number");
		float y = (float)lua_tonumber(L, -1);//Retrieving number on top of stack
		//Third index
		lua_rawgeti(L, first, i);//Pushing third index of first table on lua stack
		luaL_argcheck(L, lua_isinteger(L, -1), -1, "Error - Third point has to be a number");
		float z = (float)lua_tonumber(L, -1);//Retrieving number on top of stack
		//Setting vector to be sent to scene
		irr::core::vector3df vertex1(x, y, z);

		//Getting data for second vertex
		lua_rawgeti(L, input, k++);
		int second = lua_gettop(L);
		luaL_argcheck(L, lua_istable(L, -1), -2, "Error - vertex has to be a table");
		lua_len(L, second);
		len = (int)lua_tonumber(L, -1);
		luaL_argcheck(L, len == 3, -2, "Error - vertex has to have 3 coordinates");
		lua_rawgeti(L, second, 1);
		luaL_argcheck(L, lua_isinteger(L, -1), -2, "Error - First point has to be a number");
		x = (float)lua_tonumber(L, -1);
		lua_rawgeti(L, second, 2);
		luaL_argcheck(L, lua_isinteger(L, -1), -2, "Error - Second point has to be a number");
		y = (float)lua_tonumber(L, -1);
		lua_rawgeti(L, second, 3);
		luaL_argcheck(L, lua_isinteger(L, -1), -2, "Error - Third point has to be a number");
		z = (float)lua_tonumber(L, -1);

		irr::core::vector3df vertex2(x, y, z);

		//Getting data for third vertex
		lua_rawgeti(L, input, k++);
		int third = lua_gettop(L);
		luaL_argcheck(L, lua_istable(L, -1), -3, "Not a table in 3rd argument");
		lua_len(L, third);
		len = (int)lua_tonumber(L, -1);
		luaL_argcheck(L, len == 3, -3, "Error - vertex has to have 3 coordinates");
		lua_rawgeti(L, third, 1);
		luaL_argcheck(L, lua_isinteger(L, -1), -3, "wrong 1 argument in third table");
		x = (float)lua_tonumber(L, -1);
		lua_rawgeti(L, third, 2);
		luaL_argcheck(L, lua_isinteger(L, -1), -3, "wrong 2 argument in third table");
		y = (float)lua_tonumber(L, -1);
		lua_rawgeti(L, third, 3);
		luaL_argcheck(L, lua_isinteger(L, -1), -3, "wrong 3 argument in third table");
		z = (float)lua_tonumber(L, -1);

		irr::core::vector3df vertex3(x, y, z);
		
		std::vector<irr::core::vector3df> temp;
		temp.push_back(vertex1);
		temp.push_back(vertex2);
		temp.push_back(vertex3);
		vertices.push_back(temp);
	}

	//m_scene->addMesh(vertices);

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

	lua_len(L, 1);
	int len = (int)lua_tonumber(L, -1);
	luaL_argcheck(L, len == 3, -1, "Error - Invalid number of coordinates");
	lua_pop(L, 1);

	//Getting first argument - pos
	float x, y, z;
	lua_rawgeti(L, 1, 1);
	x = (float)lua_tonumber(L, -1);
	lua_rawgeti(L, 1, 2);
	y = (float)lua_tonumber(L, -1);
	lua_rawgeti(L, 1, 3);
	z = (float)lua_tonumber(L, -1);
	lua_pop(L, 3);
	irr::core::vector3df pos(x, y, z);

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