#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <debug.h>
#include <unistd.h>


int test_export(lua_State * L) {
    int n;
    
    n = lua_tonumber(L, 1);
    
    printf("test_export called with %i\n", n);
    
    lua_pushnumber(L, -n);
    
    return 1;
}

int test_error(lua_State * L) {
    int n = lua_gettop(L);
    int i;
    
    printf("Got LUA error.\n");
    
    if (n == 0) {
	printf("Stack is empty.\n");
	return 0;
    }
    
    for (i = 1; i <= n; i++) {
	printf("%i: ", i);
        switch(lua_type(L, i)) {
        case LUA_TNONE:
	    printf("Invalid");
            break;
        case LUA_TNIL:
            printf("(Nil)");
            break;
        case LUA_TNUMBER:
            printf("(Number) %f", lua_tonumber(L, i));
            break;
        case LUA_TBOOLEAN:
            printf("(Bool)   %s", (lua_toboolean(L, i) ? "true" : "false"));
            break;
        case LUA_TSTRING:
            printf("(String) %s", lua_tostring(L, i));
            break;
        case LUA_TTABLE:
            printf("(Table)");
            break;
        case LUA_TFUNCTION:
            printf("(Function)");
            break;
        default:
            printf("Unknown");
	}

	printf("\n");
    }

    return 0;
}


int main(int argc, char ** argv) {
    init_scr();
    sleep(1);
    lua_State * L;

    scr_printf("%s\n", LUA_COPYRIGHT); //Gets Lua version
    
    if (argc != 2) {
	printf("You have to specify the lua script to run.\n");
	return -1;
    }
    
    // Create lua VM
       
    if (!(L = luaL_newstate())) {
    printf("Error creating lua_State\n");
	return -1;
    }
    
    lua_atpanic(L, test_error);

    
    printf("Creating luaVM... ");
    luaL_openlibs(L); 

   
    printf("Binding some functions... ");
    printf("test_export "); lua_register(L, "test_export", test_export);
    printf("done!\n");
    
    scr_printf("Loading script file: %s\n", argv[1]);
    
    // load script
    int status = luaL_loadfile(L, argv[1]);
	
    // call script
    if (status == 0) 
    {
	printf("Running.\n");
    	status = lua_pcall(L, 0, LUA_MULTRET, 0);
    }

    // show error, if any
    if (status != 0) 
    {
	printf("error: %s\n", lua_tostring(L, -1));
	lua_pop(L, 1);  // remove error message
    }

   
    printf("Done!.\n");
    
    // cleanup
    lua_close(L);


    sleep(30);
    return 0;
}
