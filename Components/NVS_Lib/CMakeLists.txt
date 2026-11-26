cmake_minimum_required(VERSION 3.10...3.21)
project(NVS_Lib LANGUAGES CXX)

add_library(NVS_Lib STATIC Src/NVS.cpp)
add_library(NVS::NVS ALIAS NVS_Lib)
target_include_directories(NVS_Lib PUBLIC Inc)
target_compile_features(NVS_Lib PUBLIC cxx_std_11)

option(NVS_CONF_DEBUG "Enable NVS diagnostic messages" ON)
option(NVS_CONF_USE_STRING_KEY "Use string keys instead of uint16_t keys" ON)
target_compile_definitions(NVS_Lib PUBLIC
    NVS_CONF_DEBUG=$<BOOL:${NVS_CONF_DEBUG}>
    NVS_CONF_USE_STRING_KEY=$<BOOL:${NVS_CONF_USE_STRING_KEY}>
)

# CPM OPTIONS can supply normal variables, including on older CMake versions.
function(nvs_config_value name default_value description)
    if(NOT DEFINED ${name})
        set(${name} "${default_value}" CACHE STRING "${description}")
    endif()
    target_compile_definitions(NVS_Lib PUBLIC "${name}=${${name}}")
endfunction()

if(NVS_CONF_USE_STRING_KEY)
    nvs_config_value(NVS_CONF_KEY_SIZE 14 "String key size in bytes, including the terminator")
endif()
nvs_config_value(NVS_CONF_MAXBINARYCELLS_COUNT 2 "Maximum number of binary payload cells")
nvs_config_value(NVS_CONF_TYPEOF_STATE uint32_t "Unsigned integer type for states and tags")
nvs_config_value(NVS_CONF_MAXSIZE_OF_CELL_VALUES 8 "Maximum scalar value size in bytes")
nvs_config_value(NVS_CONF_TAG_START_VALUE 0x35353535 "Start tag for pages and cells")
nvs_config_value(NVS_CONF_ERASED_STATE_VALUE 0xFFFFFFFF "Erased flash state")
nvs_config_value(NVS_CONF_VALID_STATE_VALUE 0xAAAAAAAA "Valid flash state")
nvs_config_value(NVS_CONF_RELEASED_STATE_VALUE 0x00000000 "Released flash state")
