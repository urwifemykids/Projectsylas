#include "learnspells_on_levelup.h"
#include "Player.h"

// Called when a player logs in (firstLogin indicates if it's their first-ever login).
void LearnSpellsOnLevelUp::OnLogin(Player* player, bool /*firstLogin*/)
{
    LearnAllSpells(player);
}

// Called when a player's level changes.
void LearnSpellsOnLevelUp::OnLevelChanged(Player* player, uint8 /*oldLevel*/)
{
    LearnAllSpells(player);
}

// Called when a player learns a talent (talentId and talentRank).
void LearnSpellsOnLevelUp::OnLearnTalents(Player* player, uint32 /*talentId*/, uint32 /*talentRank*/)
{
    LearnAllSpells(player);
}

// The "master" function that runs all sub-routines when needed.
void LearnSpellsOnLevelUp::LearnAllSpells(Player* player)
{
    // If this is a GM account and we've disabled GM learning, skip entirely.
    if (player->IsGameMaster() && !EnableGamemasters)
        return;



    // Run each category in turn:
    LearnClassSpells(player);
    LearnTalentRanks(player);
    LearnProficiencies(player);
    LearnMounts(player);
    AddTotems(player);
}

// 1) Learn "class" spells from the TYPE_CLASS list.
void LearnSpellsOnLevelUp::LearnClassSpells(Player* player)
{
    if (!EnableClassSpells && !EnableFromQuests)
        return;

    auto spells = GetSpells(TYPE_CLASS);
    if (spells.empty())
        return;

    for (auto& row : spells)
    {
        // If this row requires a quest (flag=1) but we disabled "from quests", skip it.
        if (row[SPELL_REQUIRES_QUEST] == 1 && !EnableFromQuests)
            continue;

        // If this row does NOT require a quest (flag=0) but we disabled class spells, skip it.
        if (row[SPELL_REQUIRES_QUEST] == 0 && !EnableClassSpells)
            continue;

        // Race check (-1 means ANY race).
        if (row[SPELL_REQUIRED_RACE] != -1 && row[SPELL_REQUIRED_RACE] != int(player->GetRace()))
            continue;

        // Class must match exactly.
        if (row[SPELL_REQUIRED_CLASS] != int(player->GetClass()))
            continue;

        // Level requirement.
        if (player->GetLevel() < row[SPELL_REQUIRED_LEVEL])
            continue;

        // Prerequisite spell ID (-1 means none).
        if (row[SPELL_REQUIRED_SPELL_ID] != -1 && !player->HasSpell(row[SPELL_REQUIRED_SPELL_ID]))
            continue;

        // Finally, teach the spell if the player does not already know it.
        if (!player->HasSpell(row[SPELL_ID]))
            player->LearnSpell(row[SPELL_ID], false);
    }

    // Paladin-specific overrides (Seal and mount at fixed levels).
    if (player->GetClass() == CLASS_PALADIN)
    {
        // Level 64: Paladin seal (Alliance vs. Horde).
        if (player->GetLevel() >= 64)
        {
            if (player->GetTeamId() == TEAM_ALLIANCE && !player->HasSpell(31801))
                player->LearnSpell(31801, false); // Seal of Vengeance (Alliance)
            if (player->GetTeamId() == TEAM_HORDE && !player->HasSpell(53736))
                player->LearnSpell(53736, false); // Seal of Corruption (Horde)
        }

        // Level 20: Basic mount.
        if (player->GetLevel() >= 20)
        {
            if (player->GetTeamId() == TEAM_ALLIANCE && !player->HasSpell(13819))
                player->LearnSpell(13819, false); // Summon Warhorse (Alliance)
            if (player->GetTeamId() == TEAM_HORDE && !player->HasSpell(34769))
                player->LearnSpell(34769, false); // Summon Thalassian Warhorse (Horde)
        }

        // Level 40: Improved mount.
        if (player->GetLevel() >= 40)
        {
            if (player->GetTeamId() == TEAM_ALLIANCE && !player->HasSpell(23214))
                player->LearnSpell(23214, false); // Summon Charger (Alliance)
            if (player->GetTeamId() == TEAM_HORDE && !player->HasSpell(34767))
                player->LearnSpell(34767, false); // Summon Great Exarch's Charger (Horde)
        }
    }
}

// 2) Learn "talent rank" spells from the TYPE_TALENTS list.
void LearnSpellsOnLevelUp::LearnTalentRanks(Player* player)
{
    if (!EnableTalentRanks)
        return;

    auto spells = GetSpells(TYPE_TALENTS);
    if (spells.empty())
        return;

    for (auto& row : spells)
    {
        // Race check.
        if (row[SPELL_REQUIRED_RACE] != -1 && row[SPELL_REQUIRED_RACE] != int(player->GetRace()))
            continue;

        // Class check.
        if (row[SPELL_REQUIRED_CLASS] != int(player->GetClass()))
            continue;

        // If it requires a quest but we disabled quest learning, skip.
        if (row[SPELL_REQUIRES_QUEST] == 1 && !EnableFromQuests)
            continue;

        // Prerequisite spell: if specified, player must already have it.
        if (row[SPELL_REQUIRED_SPELL_ID] != -1 && !player->HasSpell(row[SPELL_REQUIRED_SPELL_ID]))
            continue;

        // Level requirement.
        if (player->GetLevel() < row[SPELL_REQUIRED_LEVEL])
            continue;

        if (!player->HasSpell(row[SPELL_ID]))
            player->LearnSpell(row[SPELL_ID], false);
    }
}

// 3) Learn weapon/armor proficiencies from the TYPE_PROFICIENCIES list.
void LearnSpellsOnLevelUp::LearnProficiencies(Player* player)
{
    if (!EnableProficiencies)
        return;

    auto spells = GetSpells(TYPE_PROFICIENCIES);
    if (spells.empty())
        return;

    for (auto& row : spells)
    {
        // Race check (-1 means any).
        if (row[SPELL_REQUIRED_RACE] != -1 && row[SPELL_REQUIRED_RACE] != int(player->GetRace()))
            continue;

        // Class check (-1 or matching).
        if (row[SPELL_REQUIRED_CLASS] != -1 && row[SPELL_REQUIRED_CLASS] != int(player->GetClass()))
            continue;

        // Team/faction check (-1 or matching).
        if (row[SPELL_REQUIRED_TEAM] != -1 && row[SPELL_REQUIRED_TEAM] != int(player->GetTeamId()))
            continue;

        // Prerequisite spell.
        if (row[SPELL_REQUIRED_SPELL_ID] != -1 && !player->HasSpell(row[SPELL_REQUIRED_SPELL_ID]))
            continue;

        // Level requirement.
        if (player->GetLevel() < row[SPELL_REQUIRED_LEVEL])
            continue;

        if (!player->HasSpell(row[SPELL_ID]))
            player->LearnSpell(row[SPELL_ID], false);
    }
}

// 4) Learn mounts and related spells from the TYPE_MOUNTS list.
void LearnSpellsOnLevelUp::LearnMounts(Player* player)
{
    auto spells = GetSpells(TYPE_MOUNTS);
    if (spells.empty())
        return;

    for (auto& row : spells)
    {
        // If it's a riding spell, check the config flags:
        if ((row[SPELL_ID] == SPELL_APPRENTICE_RIDING || row[SPELL_REQUIRED_SPELL_ID] == SPELL_APPRENTICE_RIDING)
            && !EnableApprenticeRiding)
            continue;
        if ((row[SPELL_ID] == SPELL_JOURNEYMAN_RIDING || row[SPELL_REQUIRED_SPELL_ID] == SPELL_JOURNEYMAN_RIDING)
            && !EnableJourneymanRiding)
            continue;
        if ((row[SPELL_ID] == SPELL_EXPERT_RIDING || row[SPELL_REQUIRED_SPELL_ID] == SPELL_EXPERT_RIDING)
            && !EnableExpertRiding)
            continue;
        if ((row[SPELL_ID] == SPELL_ARTISAN_RIDING || row[SPELL_REQUIRED_SPELL_ID] == SPELL_ARTISAN_RIDING)
            && !EnableArtisanRiding)
            continue;
        if (row[SPELL_ID] == SPELL_COLD_WEATHER_FLYING && !EnableColdWeatherFlying)
            continue;

        // If this row requires a quest but we disabled quest learning, skip.
        if (row[SPELL_REQUIRES_QUEST] == 1 && !EnableFromQuests)
            continue;

        // Race check.
        if (row[SPELL_REQUIRED_RACE] != -1 && row[SPELL_REQUIRED_RACE] != int(player->GetRace()))
            continue;

        // Class check (-1 or matching).
        if (row[SPELL_REQUIRED_CLASS] != -1 && row[SPELL_REQUIRED_CLASS] != int(player->GetClass()))
            continue;

        // Team/faction check (-1 or matching).
        if (row[SPELL_REQUIRED_TEAM] != -1 && row[SPELL_REQUIRED_TEAM] != int(player->GetTeamId()))
            continue;

        // Prerequisite spell.
        if (row[SPELL_REQUIRED_SPELL_ID] != -1 && !player->HasSpell(row[SPELL_REQUIRED_SPELL_ID]))
            continue;

        // Level requirement.
        if (player->GetLevel() < row[SPELL_REQUIRED_LEVEL])
            continue;

        if (!player->HasSpell(row[SPELL_ID]))
            player->LearnSpell(row[SPELL_ID], false);
    }
}

// 5) Add Shaman totem items if needed.
void LearnSpellsOnLevelUp::AddTotems(Player* player)
{
    if (player->GetClass() != CLASS_SHAMAN)
        return;

    if (!EnableClassSpells && !EnableFromQuests)
        return;

    // Each entry: { itemEntry, totemCategory, requiredLevel }
    static uint32 totems[4][3] =
    {
        { 5175,   2,  4 },  // Earth Totem (category 2), level 4
        { 5176,   4, 10 },  // Fire Totem (category 4), level 10
        { 5177,   5, 20 },  // Water Totem (category 5), level 20
        { 5178,   3, 30 }   // Air Totem (category 3), level 30
    };

    for (int i = 0; i < 4; ++i)
    {
        if (player->GetLevel() < int(totems[i][2]))
            continue;

        // If player does not have any totem of that category equipped, give them the item.
        if (!player->HasItemTotemCategory(totems[i][1]))
            player->AddItem(totems[i][0], 1);
    }
}
