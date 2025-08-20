#ifndef TC_LEARNSPELLS_ON_LEVELUP_H
#define TC_LEARNSPELLS_ON_LEVELUP_H

#include "ScriptMgr.h"
#include "Player.h"

// (These enums are unchanged from before.)
enum SpellType
{
    TYPE_CLASS = 0,
    TYPE_TALENTS = 1,
    TYPE_PROFICIENCIES = 2,
    TYPE_MOUNTS = 3
};

enum SpellColumn
{
    SPELL_ID = 0,
    SPELL_REQUIRED_TEAM = 1,
    SPELL_REQUIRED_RACE = 2,
    SPELL_REQUIRED_CLASS = 3,
    SPELL_REQUIRED_LEVEL = 4,
    SPELL_REQUIRED_SPELL_ID = 5,
    SPELL_REQUIRES_QUEST = 6
};

enum Riding
{
    SPELL_APPRENTICE_RIDING = 33388,
    SPELL_JOURNEYMAN_RIDING = 33391,
    SPELL_EXPERT_RIDING = 34090,
    SPELL_ARTISAN_RIDING = 34091,
    SPELL_COLD_WEATHER_FLYING = 54197
};

class LearnSpellsOnLevelUp : public PlayerScript, public WorldScript
{
public:
    LearnSpellsOnLevelUp();

    // === PlayerScript hooks (must match exactly TrinityCore 3.3.5 signatures) ===

    /// Called when a player logs in (firstLogin = is this the very first time?).
    void OnLogin(Player* player, bool firstLogin);

    /// Called when a player's level changes.
    void OnLevelChanged(Player* player, uint8 oldLevel);

    /// Called when a talent is learned (talentId = the talent entry, talentRank = the new rank).
    void OnLearnTalents(Player* player, uint32 talentId, uint32 talentRank);

    // === WorldScript hook (TrinityCore 3.3.5) ===

    /// Called after worldserver.conf is (re)loaded.
    void OnConfigLoad(bool reload);

private:
    // Configuration flags (populated in OnConfigLoad)
    bool EnableGamemasters;
    bool EnableClassSpells;
    bool EnableTalentRanks;
    bool EnableProficiencies;
    bool EnableFromQuests;
    bool EnableApprenticeRiding;
    bool EnableJourneymanRiding;
    bool EnableExpertRiding;
    bool EnableArtisanRiding;
    bool EnableColdWeatherFlying;

    // The "master" function that re-runs all learning routines when needed
    void LearnAllSpells(Player* player);

    // Individual sub-routines for each category
    void LearnClassSpells(Player* player);
    void LearnTalentRanks(Player* player);
    void LearnProficiencies(Player* player);
    void LearnMounts(Player* player);
    void AddTotems(Player* player);

    // Returns the static, hard-coded spell lists (as vector<vector<int>>)
    std::vector<std::vector<int>> GetSpells(int type);
};

#endif // TC_LEARNSPELLS_ON_LEVELUP_H
