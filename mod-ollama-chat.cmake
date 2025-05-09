# Ensure the module is correctly registered before linking
if(TARGET modules)
    target_link_libraries(modules PRIVATE
        CURL::libcurl
        nlohmann_json::nlohmann_json
    )
endif()

find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(SOURCES
    ...
    mod-ollama-chat_filter.cpp
)

