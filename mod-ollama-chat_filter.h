#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <cstdint>

// Filters responses based on content safety and immersion rules
bool IsSpammyEconomicResponse(std::string_view response);
bool ContainsToxicLanguage(std::string_view response);
bool IsContentInappropriateForLevel(std::string_view response, uint32_t level);
bool IsInformationalQuestion(const std::string& message);
