#include "mod-ollama-chat_filter.h"
#include <algorithm>
#include <string>
#include <string_view>
#include <vector>

bool IsSpammyEconomicResponse(std::string_view response)
{
    std::vector<std::string> keywords = { "gold", "money", "buy", "sell", "profit", "auction", "trade" };
    int matchCount = 0;
    std::string lower(response);
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    for (const auto& word : keywords)
    {
        size_t pos = lower.find(word);
        while (pos != std::string::npos)
        {
            ++matchCount;
            pos = lower.find(word, pos + word.length());
        }
    }

    return matchCount > 1;
}

bool ContainsToxicLanguage(std::string_view response)
{
    std::vector<std::string> toxicWords = { "idiot", "noob", "trash", "shut up", "stupid", "kys", "moron" };
    std::string lower(response);
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    for (const auto& word : toxicWords)
    {
        if (lower.find(word) != std::string::npos)
            return true;
    }

    return false;
}

bool IsContentInappropriateForLevel(std::string_view response, uint32_t level)
{
    if (level >= 71)
        return false;

    std::vector<std::string> wotlkTerms = {
        "icecrown", "ulduar", "naxxramas", "lich king", "northrend", "arthas", "storm peaks",
        "death knight", "dk", "wrath loot", "ebon hold", "valkyr", "frozen throne"
    };

    std::string lower(response);
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    for (const auto& word : wotlkTerms)
    {
        if (lower.find(word) != std::string::npos)
            return true;
    }

    return false;
}

bool IsInformationalQuestion(const std::string& message)
{
    std::vector<std::string> keywords = {
        "where", "how do i get", "how can i get", "how to", "quest", "turn in", "location",
        "find", "go to", "flight", "zeppelin", "boat", "portal", "direction"
    };

    std::string lower = message;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    for (const auto& keyword : keywords)
    {
        if (lower.find(keyword) != std::string::npos)
            return true;
    }

    return false;  // ✅ správně uzavřeno uvnitř funkce
}
