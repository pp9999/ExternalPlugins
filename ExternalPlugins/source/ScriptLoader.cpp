#include "ScriptLoader.h"
#include <vector>

extern "C" __declspec(dllexport) void GetScripts(std::shared_ptr<ScriptFunction[]>*functions, size_t * count) {
    std::initializer_list<ScriptFunction> initList = {
        { "Function1", &TestScript },
        { "AutoDivination", &AutoDivination }
        //Add your scripts here
    };

    *count = initList.size();
    std::shared_ptr<ScriptFunction[]> functionArray(new ScriptFunction[*count]);
    std::copy(initList.begin(), initList.end(), functionArray.get());

    *functions = functionArray;
}