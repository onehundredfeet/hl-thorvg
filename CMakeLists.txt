cmake_minimum_required(VERSION 3.10)
project (thorvg C CXX )

set(APP_DIR "${CMAKE_CURRENT_LIST_DIR}"
    CACHE PATH "Application directory")

set(THORVG_DIR "${APP_DIR}/ext/thorvg"
    CACHE PATH "Location of 'thorvg'")

set(THORVG_INC_DIR ${THORVG_DIR}/installed/include)
set(THORVG_LIB_DIR ${THORVG_DIR}/installed/lib)

if (NOT HDLL_DESTINATION) 
    set(HDLL_DESTINATION "/usr/local/lib")
endif()

if (NOT HL_INCLUDE_DIR) 
    set(HL_INCLUDE_DIR "/usr/local/include")
endif()

if (NOT COMMON_LIB_DIR) 
    set(COMMON_LIB_DIR "/usr/local/lib")
endif()

# Enable Blend2D static build.

#### Project definitions
find_library(LIBTHORVG NAMES thorvg HINTS ${THORVG_LIB_DIR} )

add_library(thorvg.hdll SHARED
#src/hl-idl-helpers.cpp
src/thorvg.cpp
)


set_target_properties(thorvg.hdll
PROPERTIES
PREFIX ""
OUTPUT_NAME thorvg
SUFFIX .hdll
)

set_property(TARGET thorvg.hdll PROPERTY CXX_STANDARD 17)

cmake_policy(SET CMP0015 NEW)

target_include_directories(thorvg.hdll
PRIVATE
${HL_INCLUDE_DIR}
${THORVG_INC_DIR}
)

if (UNIX)
    # Some special flags are needed for GNU GCC compiler
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fPIC -fpermissive")
    #not sure why the ${HL_LIB_DIR} is necessary given the above.
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -shared  ")

    find_library(LIBHL NAMES hl HINTS ${HL_LIB_DIR} )
else()
    find_library(LIBHL NAMES libhl HINTS ${HL_LIB_DIR} )
endif (UNIX)

set_property (TARGET thorvg.hdll APPEND_STRING PROPERTY COMPILE_FLAGS )
 
target_link_libraries(thorvg.hdll ${LIBHL} ${LIBTHORVG})

install(TARGETS thorvg.hdll DESTINATION ${HDLL_DESTINATION})
