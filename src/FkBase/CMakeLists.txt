cmake_minimum_required(VERSION 3.4.1)
project(FkBase LANGUAGES C CXX)

set(INCLUDES
        ${ANDROID_NDK}/sources/android/cpufeatures
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/../glm
        ${THIRD_PARTY_DIR}/cjson/include
        )
include_directories(${INCLUDES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS_CONFIGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_FLAGS_CONFIGS}")

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    if (${ENABLE_TINY})
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--exclude-libs,ALL,--gc-sections")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/rules-tiny.txt")
    endif ()
endif ()
#将Warning视为Error
if (${ENABLE_BAN_WARNING})
    foreach (var ${WARNING_CONFIGS})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${var}")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${var}")
    endforeach (var)
endif ()
message(STATUS "${PROJECT_NAME} CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

#------------------STATIC START----------------------
#add_library(cJSON STATIC IMPORTED)
#set_target_properties(cJSON PROPERTIES IMPORTED_LOCATION
#        "${THIRD_PARTY_DIR}/cjson/static/${ANDROID_ABI}/libcjson.a")

#------------------STATIC END----------------------

aux_source_directory(${PROJECT_SOURCE_DIR}/core SOURCES_CORE)
aux_source_directory(${PROJECT_SOURCE_DIR}/entity SOURCES_ENTITY)
aux_source_directory(${PROJECT_SOURCE_DIR}/utils SOURCES_UTILS)
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    aux_source_directory(${PROJECT_SOURCE_DIR}/utils/apple SOURCES_UTILS_PLATFORM)
else ()
    aux_source_directory(${PROJECT_SOURCE_DIR}/utils/android SOURCES_UTILS_PLATFORM)
endif ()
aux_source_directory(${PROJECT_SOURCE_DIR}/thread SOURCES_THREAD)
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    aux_source_directory(${PROJECT_SOURCE_DIR}/thread/apple SOURCES_THREAD_PLATFORM)
else ()
    aux_source_directory(${PROJECT_SOURCE_DIR}/thread/android SOURCES_THREAD_PLATFORM)
endif ()

list(APPEND SOURCES_ROOT ${SOURCES_CORE})
list(APPEND SOURCES_ROOT ${SOURCES_ENTITY})
list(APPEND SOURCES_ROOT ${SOURCES_UTILS})
list(APPEND SOURCES_ROOT ${SOURCES_UTILS_PLATFORM})
list(APPEND SOURCES_ROOT ${SOURCES_THREAD})
list(APPEND SOURCES_ROOT ${SOURCES_THREAD_PLATFORM})

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "Make for Darwin")
else ()
    if (${ANDROID_ABI} STREQUAL "armeabi-v7a")
        set_property(SOURCE ${SOURCES_ROOT} APPEND_STRING PROPERTY COMPILE_FLAGS " -mfpu=neon")
        add_definitions("-DHAVE_NEON=1")
    elseif (${ANDROID_ABI} STREQUAL "x86")
        set_property(SOURCE ${SOURCES_ROOT} APPEND_STRING PROPERTY COMPILE_FLAGS
                " -mssse3  -Wno-unknown-attributes \
                    -Wno-deprecated-declarations \
                    -Wno-constant-conversion \
                    -Wno-static-in-inline")
        add_definitions(-DHAVE_NEON_X86=1 -DHAVE_NEON=1)
    endif ()
endif ()

set(THIRD_PARTY "")
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    find_package(Threads REQUIRED)
    set(THIRD_PARTY "${THIRD_PARTY} ${Threads}")
endif ()
find_library(
        log-lib
        log)
if (log-lib)
    set(THIRD_PARTY "${THIRD_PARTY} ${log-lib}")
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_library(${PROJECT_NAME} STATIC ${SOURCES_ROOT})
    set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../FkBase)
else ()
    add_library(${PROJECT_NAME} OBJECT ${SOURCES_ROOT})
endif()
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
target_link_libraries(${PROJECT_NAME} PUBLIC ${THIRD_PARTY})
# target_link_options(${PROJECT_NAME} PUBLIC -Wl,-no_adhoc_codesign)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
