cmake_minimum_required(VERSION 3.16)

project(HazelWorkspace LANGUAGES CXX)

# Set build platform to 64-bit
if(MSVC)
    set(CMAKE_GENERATOR_PLATFORM x64)
endif()

# Set build configuration to Release by default
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# Thiết lập global output folder cho tất cả target
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

# -- Include directories relative to root folder (solution directory)--
set(GLFW_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/Hazel/vendor/GLFW/include)
set(Glad_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/Hazel/vendor/Glad/include)
set(ImGui_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/Hazel/vendor/imgui)
set(Glm_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/Hazel/vendor/glm)

add_subdirectory(Hazel/vendor/GLFW)
add_subdirectory(Hazel/vendor/Glad)
add_subdirectory(Hazel/vendor/imgui)
add_subdirectory(Hazel/vendor/glm)

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
    ${ImGui_DIRS}
    ${Glm_DIRS}
)

target_link_libraries(Hazel PRIVATE glfw Glad ImGui opengl32.lib)

if(WIN32)
    target_compile_features(Hazel PUBLIC cxx_std_17)
    target_compile_definitions(Hazel PRIVATE HZ_PLATFORM_WINDOWS HZ_BUILD_DLL GLFW_INCLUDE_NONE)
endif()

if(MSVC)
    set_property(TARGET Hazel PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()

#  Configurations Debug/Release
target_compile_definitions(Hazel PRIVATE
    $<$<CONFIG:Debug>:HZ_DEBUG>
    $<$<CONFIG:Release>:HZ_RELEASE>
)

# --------------------------------- Sandbox ---------------------------------
add_executable(Sandbox)

file(GLOB_RECURSE SANDBOX_HEADERS "Sandbox/src/*.h")
file(GLOB_RECURSE SANDBOX_SOURCES "Sandbox/src/*.cpp")
target_sources(Sandbox PRIVATE ${SANDBOX_HEADERS} ${SANDBOX_SOURCES})

target_include_directories(Sandbox PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Hazel/src
    ${Glm_DIRS}
)

target_link_libraries(Sandbox PRIVATE Hazel)

if(WIN32)
    target_compile_features(Sandbox PUBLIC cxx_std_17)
    target_compile_definitions(Sandbox PRIVATE HZ_PLATFORM_WINDOWS)
endif()

if(MSVC)
    set_property(TARGET Sandbox PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()

#  Configurations Debug/Release
target_compile_definitions(Sandbox PRIVATE
    $<$<CONFIG:Debug>:HZ_DEBUG>
    $<$<CONFIG:Release>:HZ_RELEASE>
)
