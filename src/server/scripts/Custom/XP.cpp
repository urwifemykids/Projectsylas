// Includes for TrinityCore 3.3.5
#include "ScriptMgr.h" // This is usually ScriptPCH.h in TrinityCore for common includes
#include "Player.h"   // For Player class


// It's good practice to use a descriptive name for your script class
class DynamicXPRatePlayerScript : public PlayerScript
{
public:
    // Constructor matching TrinityCore's PlayerScript constructor
    explicit DynamicXPRatePlayerScript() : PlayerScript("DynamicXPRatePlayerScript") { }

    // No OnLogin needed as per your request
    // virtual void OnLogin(Player* /*player*/, bool /*firstLogin*/) override
    // {
    //     // No message, no configuration reading
    // }

    // Override OnGiveXP.
    // IMPORTANT: Note the absence of 'uint8 xpSource' parameter as per TrinityCore's API
    void OnGiveXP(Player* player, uint32& amount, Unit* /*victim*/) override
    {
        // No general enable/disable boolean as per your request,
        // the logic is always active on compilation.
        if (player->GetLevel() <= 9)
            amount *= 1; // Fixed rate for 1-9
        else if (player->GetLevel() <= 19)
            amount *= 2; // Fixed rate for 10-19
        else if (player->GetLevel() <= 29)
            amount *= 3; // Fixed rate for 20-29
        else if (player->GetLevel() <= 39)
            amount *= 4; // Fixed rate for 30-39
        else if (player->GetLevel() <= 49)
            amount *= 5; // Fixed rate for 40-49
        else if (player->GetLevel() <= 59)
            amount *= 6; // Fixed rate for 50-59
        else if (player->GetLevel() <= 69)
            amount *= 7; // Fixed rate for 60-69
        else if (player->GetLevel() <= 79)
            amount *= 8; // Fixed rate for 70-79
        // You can add more else if blocks for higher levels if needed
    }
};

// This is the standard way to register a custom script in TrinityCore 3.3.5
// This function will be called by the ScriptMgr
void AddSC_DynamicXPRate()
{
    // Create an instance of your script class and register it
    new DynamicXPRatePlayerScript();
}
