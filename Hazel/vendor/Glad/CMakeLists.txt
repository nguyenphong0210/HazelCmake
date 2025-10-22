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

if(MSVC)
    set_property(TARGET Glad PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()

#  Configurations Debug/Release
target_compile_definitions(Glad PRIVATE
    $<$<CONFIG:Debug>:HZ_DEBUG>
    $<$<CONFIG:Release>:HZ_RELEASE>
)