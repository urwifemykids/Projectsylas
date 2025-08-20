#include "learnspells_on_levelup.h"

// This function's name MUST match what's declared in custom_script_loader.cpp:
//   void AddSC_LearnSpellsOnLevelUp();
// And custom_script_loader.cpp already calls it inside AddCustomScripts() :contentReference[oaicite:2]{index=2}

void AddSC_LearnSpellsOnLevelUp()
{
    new LearnSpellsOnLevelUp();
}
