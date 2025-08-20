#include "learnspells_on_levelup.h"
#include "Config.h"

// When TrinityCore loads worldserver.conf, this WorldScript hook will pull in
// the "LearnSpellsOnLevelUp.*" settings (if they exist); otherwise we use defaults.

LearnSpellsOnLevelUp::LearnSpellsOnLevelUp()
    : PlayerScript("LearnSpellsOnLevelUp_PlayerScript")
    , WorldScript("LearnSpellsOnLevelUp_WorldScript")
    , EnableGamemasters(true)
    , EnableClassSpells(true)
    , EnableTalentRanks(true)
    , EnableProficiencies(true)
    , EnableFromQuests(false)
    , EnableApprenticeRiding(true)
    , EnableJourneymanRiding(true)
    , EnableExpertRiding(true)
    , EnableArtisanRiding(true)
    , EnableColdWeatherFlying(false)
{
    // Actual values populated in OnConfigLoad()
}

void LearnSpellsOnLevelUp::OnConfigLoad(bool /*reload*/)
{
    EnableGamemasters = sConfigMgr->GetBoolDefault("LearnSpellsOnLevelUp.EnableGamemasters", true);
    EnableClassSpells = sConfigMgr->GetBoolDefault("LearnSpellsOnLevelUp.EnableClassSpells", true);
    EnableTalentRanks = sConfigMgr->GetBoolDefault("LearnSpellsOnLevelUp.EnableTalentRanks", true);
    EnableProficiencies = sConfigMgr->GetBoolDefault("LearnSpellsOnLevelUp.EnableProficiencies", true);
    EnableFromQuests = sConfigMgr->GetBoolDefault("LearnSpellsOnLevelUp.EnableFromQuests", false);
    EnableApprenticeRiding = sConfigMgr->GetBoolDefault("LearnSpellsOnLevelUp.EnableApprenticeRiding", true);
    EnableJourneymanRiding = sConfigMgr->GetBoolDefault("LearnSpellsOnLevelUp.EnableJourneymanRiding", true);
    EnableExpertRiding = sConfigMgr->GetBoolDefault("LearnSpellsOnLevelUp.EnableExpertRiding", true);
    EnableArtisanRiding = sConfigMgr->GetBoolDefault("LearnSpellsOnLevelUp.EnableArtisanRiding", true);
    EnableColdWeatherFlying = sConfigMgr->GetBoolDefault("LearnSpellsOnLevelUp.EnableColdWeatherFlying", false);
}
