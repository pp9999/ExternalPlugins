#pragma once

#include "ScriptRegistry.h"
#include <vector>

static std::vector<ScriptFunction>& ScriptRegistryStorage() {
    static std::vector<ScriptFunction> scripts;
    return scripts;
}

void RegisterScript(const ScriptFunction& script) {
    ScriptRegistryStorage().push_back(script);
}

const ScriptFunction* GetRegisteredScripts(size_t* count) {
    auto& scripts = ScriptRegistryStorage();
    if (count) {
        *count = scripts.size();
    }
    return scripts.empty() ? nullptr : scripts.data();
}
