#pragma once
#include <iostream>

struct ScriptFunction {
    const char* name;
    void (*functionPointer)();
};

/// <summary>
/// Main script functions
/// </summary>
void TestScript();
void AutoDivination();

