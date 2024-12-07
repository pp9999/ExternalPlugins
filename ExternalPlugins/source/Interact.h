#pragma once
#include <vector>
#include <string>
#ifdef LIBRARY_EXPORTS
#    define LIBRARY_API __declspec(dllexport)
#else
#    define LIBRARY_API __declspec(dllimport)
#endif



struct Interact {
    LIBRARY_API bool NPC(std::string name, std::string action, int distance = 60);
    LIBRARY_API bool Object(std::string name, std::string action, int distance = 60);
    //LIBRARY_API bool Item();
};
