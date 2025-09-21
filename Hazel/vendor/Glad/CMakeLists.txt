# Create Glad project as StaticLib using C
project(Glad C)

# Create Glad project as StaticLib
add_library(Glad STATIC
    src/glad.c
    include/glad/glad.h
    include/KHR/khrplatform.h
)

# Add include dir
target_include_directories(Glad PUBLIC include)

# Output build directory (similar to targetdir/objdir)
set_target_properties(Glad PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/Glad"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/Glad"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/Glad"
)

# If building on Windows
if (WIN32)
    # systemversion "latest" ~ latest toolset
    set(CMAKE_SYSTEM_VERSION 10.0)  # or leave default, depending on toolset
endif()

# staticruntime "On" + /MT for MSVC
if (MSVC)
    foreach(flag_var CMAKE_C_FLAGS_RELEASE CMAKE_CXX_FLAGS_RELEASE)
        string(REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endforeach()
elseif (MINGW)
    target_compile_options(Glad PRIVATE -static -static-libgcc -static-libstdc++)
endif()
