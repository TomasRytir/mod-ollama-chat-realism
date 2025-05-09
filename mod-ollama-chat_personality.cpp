#include "mod-ollama-chat_personality.h"
#include "Player.h"
#include "PlayerbotMgr.h"
#include "Log.h"
#include "mod-ollama-chat_config.h"
#include <random>

// Internal personality map
static std::unordered_map<uint64_t, BotPersonalityType> botPersonalities;

extern bool g_EnableRPPersonalities; // from config

// Retrieves (and possibly initializes) the personality for a bot.
BotPersonalityType GetBotPersonality(Player* bot)
{
    uint64_t botGuid = bot->GetGUID().GetRawValue();    
    if (botPersonalities.find(botGuid) == botPersonalities.end())
    {
        if (!g_EnableRPPersonalities)
        {
            botPersonalities[botGuid] = static_cast<BotPersonalityType>(-1);
        }
        else
        {
            if (botPersonalityList.find(botGuid) == botPersonalityList.end())
            {
                uint32 newType = (urand(0, PERSONALITY_TYPES_COUNT - 1));
                botPersonalities[botGuid] = static_cast<BotPersonalityType>(newType);

                QueryResult tableExists = CharacterDatabase.Query("SELECT * FROM information_schema.tables WHERE table_schema = 'acore_characters' AND table_name = 'mod_ollama_chat_personality' LIMIT 1;");
                if (!tableExists)
                {
                    LOG_INFO("server.loading", "[Ollama Chat] Please source the required database table first");

                }
                else
                {
                    CharacterDatabase.Execute("INSERT INTO mod_ollama_chat_personality (guid, personality) VALUES ({}, {})", botGuid, newType);
                }
                LOG_INFO("server.loading", "Generated new personality for bot {}: {}", bot->GetName(), botPersonalities[botGuid]);
            }
            else
            {                
                botPersonalities[botGuid] = static_cast<BotPersonalityType>(botPersonalityList[botGuid]);
                LOG_INFO("server.loading", "Using database personality for bot {}: {}", bot->GetName(), botPersonalities[botGuid]);
            }
            
        }
        
    }
    else
    {
        LOG_INFO("server.loading", "Using existing personality for bot {}: {}", bot->GetName(), botPersonalities[botGuid]);
    }
    return botPersonalities[botGuid];
}

std::string GetPersonalityPromptAddition(BotPersonalityType type)
{
    switch (type)
    {
    case PERSONALITY_GAMER:
        return "Focus on game mechanics and efficiency, but stay friendly and respectful.";
    case PERSONALITY_ROLEPLAYER:
        return "Respond in-character with natural references to lore and world events.";
    case PERSONALITY_LOOTGOBLIN:
        return "Mention interesting item finds or rare loot occasionally, but don’t obsess over gold.";
    case PERSONALITY_PVP_HARDCORE:
        return "Talk about PvP strategies and matchups, but avoid bragging or aggression.";
    case PERSONALITY_RAIDER:
        return "Share insights about raid bosses and teamwork, with a cooperative tone.";
    case PERSONALITY_CASUAL:
        return "Keep a laid-back tone, talk about exploration, quests, and having fun.";
    case PERSONALITY_TRADER:
        return "Discuss trading and the in-game economy with practical tips, avoiding greed-focused talk.";
    case PERSONALITY_NPC_IMPERSONATOR:
        return "Respond like a quest-giving NPC, but make it subtle and immersive.";
    case PERSONALITY_GRUMPY_VETERAN:
        return "Comment nostalgically on past expansions or features, without constant complaining.";
    case PERSONALITY_HEROIC_LEADER:
        return "Speak with confidence and leadership, offering encouragement and vision.";
    case PERSONALITY_TRICKSTER:
        return "Be playful and clever with your words, but avoid confusing or trolling players.";
    case PERSONALITY_LONE_WOLF:
        return "Keep responses short and efficient, but not rude.";
    case PERSONALITY_FOOL:
        return "Act curious and enthusiastic, occasionally misunderstanding things in a humorous way.";
    case PERSONALITY_ANCIENT_WISE_ONE:
        return "Speak thoughtfully and cryptically, like an old sage, but stay clear.";
    case PERSONALITY_BARD:
        return "Use poetic language or metaphors now and then, without breaking clarity.";
    case PERSONALITY_CONSPIRACY_THEORIST:
        return "Speculate on in-game mysteries in a fun way, not obsessively.";
    case PERSONALITY_EDGE_LORD:
        return "Be dramatic or intense, but avoid negativity or hostility.";
    case PERSONALITY_FANATIC:
        return "Express strong admiration for your class or faction, but stay respectful.";
    case PERSONALITY_HYPE_MAN:
        return "Be excited and supportive, making others feel great about what they do.";
    case PERSONALITY_PARANOID:
        return "Speak cautiously and suspiciously, but avoid constant fear-mongering.";
    case PERSONALITY_FLIRT:
        return "Be charming and playful, but keep interactions light and appropriate.";
    case PERSONALITY_RAGER:
        return "Express frustration in a comical and exaggerated way, not offensively.";
    case PERSONALITY_STONER:
        return "Maintain a chill, easy-going tone, without sounding incoherent.";
    case PERSONALITY_YOUNG_APPRENTICE:
        return "Be eager to learn and improve, asking good questions.";
    case PERSONALITY_MENTOR:
        return "Explain things clearly and patiently, offering helpful advice.";
    case PERSONALITY_SCHOLAR:
        return "Use a factual and thoughtful tone, like a well-read researcher.";
    case PERSONALITY_GLITCHED_AI:
        return "Occasionally use glitchy or robotic phrasing, but ensure meaning is clear.";
    case PERSONALITY_WANNABE_VILLAIN:
        return "Speak theatrically about ‘evil plans,’ but with clear humor and no real malice.";
    case PERSONALITY_JOLLY_BEER_LOVER:
        return "Be cheerful and hearty, like a tavern regular, without drunken slurring.";
    case PERSONALITY_GOBLIN_MERCHANT:
        return "Talk about deals and profit with a sharp tone, but don't spam gold talk.";
    case PERSONALITY_PIRATE:
        return "Use light pirate slang for flavor, not full sentences of gibberish.";
    case PERSONALITY_CHEF:
        return "Relate some things to food or recipes in a fun and creative way.";
    case PERSONALITY_POET:
        return "Add poetic phrasing when fitting, but avoid making it hard to understand.";
    default:
        return "Talk like a normal WoW player with balanced, helpful responses.";
    }
}


