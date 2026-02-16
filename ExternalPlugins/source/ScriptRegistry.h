#pragma once

#include "ScriptLoader.h"
#include <cstddef>

void RegisterScript(const ScriptFunction& script);
const ScriptFunction* GetRegisteredScripts(size_t* count);

struct ScriptRegistrar {
    ScriptRegistrar(const char* name, void (*functionPointer)(), const char* description) {
        RegisterScript({ name, functionPointer, description });
    }
};
