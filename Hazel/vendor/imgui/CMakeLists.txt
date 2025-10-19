cmake_minimum_required(VERSION 3.16)

project(ImGui LANGUAGES CXX)

# Create ImGui static library
add_library(ImGui STATIC
    imgui.cpp
    imgui_draw.cpp
    imgui_widgets.cpp
    imgui_demo.cpp

    imconfig.h
    imgui.h
    imgui_internal.h
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
)

# Set C++17
target_compile_features(ImGui PUBLIC cxx_std_17)

# Output directory (similar to targetdir/objdir)
set_target_properties(ImGui PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/ImGui"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/ImGui"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/ImGui"
)

# Windows-specific settings
if (WIN32)
    set(CMAKE_SYSTEM_VERSION 10.0)
endif()

# staticruntime "On" + /MT for MSVC
if (MSVC)
    foreach(flag_var CMAKE_C_FLAGS_RELEASE CMAKE_CXX_FLAGS_RELEASE)
        string(REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endforeach()
elseif (MINGW)
    target_compile_options(Glad PRIVATE -static -static-libgcc -static-libstdc++)
endif()