//g++ ./core.cpp -I/usr/include/lua5.4 -llua5.4 -fPIC -shared -o fsys.so 

#include <iostream>
#include <fstream>
#include <cstring>
#include <filesystem>
#include <cmath>
#include <chrono>
#include <sstream>

#ifdef __cplusplus
  #include "lua.hpp"
#else
  #include "lua.h"
  #include "lualib.h"
  #include "lauxlib.h"
#endif

namespace fs = std::filesystem;


extern "C"
{
    static int scand_dir(lua_State *L){
        const char *pathx = luaL_checkstring(L,1);
        int index = 1;

        lua_newtable(L);

        for (const auto & entry : fs::directory_iterator(pathx)){
            lua_pushnumber(L, index++);
            lua_pushstring(L, entry.path().c_str());
            lua_settable(L, -3);
        }

        return 1;
    }   

    //later: convert bytes to human readable
    static int file_size(lua_State *L) 
    {
        const char *p = luaL_checkstring(L,1);

        try 
        {
            lua_pushnumber(L, fs::file_size(p));
        }
        catch(std::filesystem::filesystem_error const& ex) 
        {
            std::cout<<"Error: " <<  ex.code().message() << '\n';
            lua_pushboolean(L,false);
        }

        return 1;
    }

    static int abs_path(lua_State *L) 
    {
        const char *str = luaL_checkstring(L,1);
        fs::path p = str, s = fs::absolute(p);
        
        lua_pushstring(L, s.c_str());
        return 1;
    }

    static int current_path(lua_State *L)
    {
        try 
        {
            lua_pushstring(L, fs::current_path().c_str());
        }
        catch(std::filesystem::filesystem_error const& ex)
        {
            std::cout<<"Error: " <<  ex.code().message() << '\n';
            lua_pushboolean(L,false);
        }

        return 1;
    }

    
    static int relative_path(lua_State *L)
    {
        const char *x = luaL_checkstring(L,1), *y = luaL_checkstring(L,2);
        fs::path p = fs::relative(x,y);

        lua_pushstring(L, p.c_str());
        return 1;
    }

    //!implement copy options
    //catch errors if file alrady exists
    static int copy(lua_State *L)
    {
        const char *x = luaL_checkstring(L,1), *y = luaL_checkstring(L,2);

        try 
        {
            // copy x -> y
            fs::copy(x,y);
            lua_pushboolean(L,true);
        }
        catch(std::filesystem::filesystem_error const& ex) 
        {
            std::cout<<"Error: " <<  ex.code().message() << '\n';
            lua_pushboolean(L,false);
        }

        return 1;
    }

    static int equivalent(lua_State *L)
    {
        const char *f1 = luaL_checkstring(L,1), *f2 = luaL_checkstring(L,2);

        if(fs::equivalent(f1, f2))
            lua_pushboolean(L,true);
        else lua_pushboolean(L,false);

        return 1;
    }
    

    static int create_dir(lua_State *L) 
    {
        const char *str = luaL_checkstring(L,1);

        try 
        {
            fs::create_directories(str);
            lua_pushboolean(L,true);
        }
        catch(std::filesystem::filesystem_error const& ex) 
        {
            std::cout<<"Error: " <<  ex.code().message() << '\n';
            lua_pushboolean(L,false);
        }

        return 1;
    }

    static int file_exists(lua_State *L)
    {
        const char *str = luaL_checkstring(L,1);
        if(fs::exists(str)) lua_pushboolean(L,true);
        else lua_pushboolean(L,false);

        return 1;
    }

    static int is_dir(lua_State *L){
        const char *str = luaL_checkstring(L,1);
        if( std::filesystem::is_directory(str)) 
            lua_pushboolean(L,true);
        else lua_pushboolean(L,false);

        return 1;
    }

    
    //output and return in readable format

    /*
    static int last_write_time(lua_State *L)
    {
        const char *path = luaL_checkstring(L,-1);
        auto ftime = std::filesystem::last_write_time(path);

        lua_pushnumber(L, 77);
        return 1;
    }
    */

    static int resize_file(lua_State *L)
    {
        const char *str = luaL_checkstring(L,1);
        std::uintmax_t sizex = luaL_checknumber(L,2);
        try {
            fs::resize_file(str,sizex);
            lua_pushboolean(L,true);
        }
        catch(std::filesystem::filesystem_error const& ex) {
            std::cout<<"Error: " <<  ex.code().message() << '\n';
            lua_pushboolean(L,false);
        }

        return 1;
    }

    static int file_type(lua_State *L)
    {
        const char *s = luaL_checkstring(L,1);
        std::string typex = "";

        if(fs::is_regular_file(s)) typex = "is a regular file";
        if(fs::is_directory(s)) typex = "is a directory";
        if(fs::is_block_file(s)) typex = "is a block device";
        if(fs::is_character_file(s)) typex = "is a character device";
        if(fs::is_fifo(s)) typex = "is a named IPC pipe";
        if(fs::is_socket(s)) typex = "is a named IPC socket";
        if(fs::is_symlink(s)) typex = "is a symlink";
        if(!fs::exists(s)) typex = "does not exist";

        lua_pushstring(L, typex.c_str());
        return 1;
    }   


    static int temp_dir_path(lua_State *L)
    {
        lua_pushstring(L, fs::temp_directory_path().c_str());
        return 1;
    }


    //register lib
    static const struct luaL_Reg fsys [] = 
    {
        {"abs_path" , abs_path},
        {"file_size", file_size},
        {"relative_path", relative_path},
        {"copy", copy},
        {"equivalent", equivalent},
        {"create_dir", create_dir},
        {"is_dir", is_dir},
        {"file_exists", file_exists},
        {"resize_file", resize_file},
        {"temp_dir_path",temp_dir_path},
        {"file_type", file_type},
        {"scan", scand_dir},
        {"current_path",current_path},
        //{"last_write_time", last_write_time}, implement
        {NULL, NULL}  /* sentinel */
    };

    int luaopen_fsys (lua_State *L)
    {
        luaL_newlib(L, fsys);
        return 1;
    }
}
