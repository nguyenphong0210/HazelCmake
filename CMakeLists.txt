cmake_minimum_required(VERSION 3.16)

project(HazelWorkspace LANGUAGES CXX)

# Set build platform to 64-bit
if(MSVC)
    set(CMAKE_GENERATOR_PLATFORM x64)
endif()

# Set build configuration to Release by default
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# -- Include directories relative to root folder (solution directory)--
set(GLFW_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/Hazel/vendor/GLFW/include)
set(Glad_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/Hazel/vendor/Glad/include)

add_subdirectory(Hazel/vendor/GLFW)
add_subdirectory(Hazel/vendor/Glad)

# --------------------------------- Hazel ---------------------------------
add_library(Hazel SHARED
    Hazel/src/Hazel.h
)

target_precompile_headers(Hazel PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Hazel/src/hzpch.h"
)

file(GLOB_RECURSE HAZEL_HEADERS "Hazel/src/*.h")
file(GLOB_RECURSE HAZEL_SOURCES "Hazel/src/*.cpp")
target_sources(Hazel PRIVATE ${HAZEL_HEADERS} ${HAZEL_SOURCES})

target_include_directories(Hazel PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/Hazel/src
    ${CMAKE_CURRENT_SOURCE_DIR}/Hazel/vendor/spdlog/include
    ${GLFW_DIRS}
    ${Glad_DIRS}
)

target_link_libraries(Hazel PRIVATE glfw Glad opengl32.lib)

target_compile_features(Hazel PUBLIC cxx_std_17)

if(MSVC)
    target_compile_definitions(Hazel PRIVATE HZ_PLATFORM_WINDOWS HZ_BUILD_DLL GLFW_INCLUDE_NONE)
    # static runtime (/MT(d)) instead of dynamic (/MD(d))
    foreach(flag_var CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE 
                     CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
        string(REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endforeach()
elseif (MINGW)
    target_compile_definitions(Hazel PRIVATE HZ_PLATFORM_WINDOWS HZ_BUILD_DLL GLFW_INCLUDE_NONE)
    # MinGW do not have /MD or /MT options, but you can choose static linking for libgcc/libstdc++
    target_compile_options(Hazel PRIVATE -static -static-libgcc -static-libstdc++)
endif()

if(MSVC)
    # Post-build copy DLL to Sandbox
    add_custom_command(TARGET Hazel POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Hazel>
            ${CMAKE_CURRENT_BINARY_DIR}/bin/$<CONFIG>/Sandbox
    )
endif()

# --------------------------------- Sandbox ---------------------------------
add_executable(Sandbox)

file(GLOB_RECURSE SANDBOX_HEADERS "Sandbox/src/*.h")
file(GLOB_RECURSE SANDBOX_SOURCES "Sandbox/src/*.cpp")
target_sources(Sandbox PRIVATE ${SANDBOX_HEADERS} ${SANDBOX_SOURCES})

target_include_directories(Sandbox PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Hazel/src
)

target_link_libraries(Sandbox PRIVATE Hazel)
target_compile_features(Sandbox PUBLIC cxx_std_17)

if(MSVC)
    target_compile_definitions(Sandbox PRIVATE HZ_PLATFORM_WINDOWS)
elseif (MINGW)
    target_compile_definitions(Sandbox PRIVATE HZ_PLATFORM_WINDOWS)
endif()

# ------------------- Configurations -------------------
# Debug
set_property(TARGET Hazel Sandbox APPEND PROPERTY COMPILE_DEFINITIONS
    $<$<CONFIG:Debug>:HZ_DEBUG>
    $<$<CONFIG:Release>:HZ_RELEASE>
)