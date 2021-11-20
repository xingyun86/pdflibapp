# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("pdflibapp")

if (MSVC)
    MESSAGE(STATUS "CMAKE_BUILD_TYPE=" ${CMAKE_BUILD_TYPE})
    set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
    foreach(CompilerFlag ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS _WINSOCK_DEPRECATED_NO_WARNINGS WIN32_LEAN_AND_MEAN _WIN32_WINNT=0x0501)
    add_compile_options("/W4" "/bigobj" "/std:c++latest" "/IGNORE:4200")
    add_link_options("/IGNORE:4006,4075,4088,4099")
    set(TARGET_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
    if(ENABLE_PUBLISHER_LEVEL)
        if(CMAKE_CL_64)
            set(WINDOW_VERSION "5.02")
        else(CMAKE_CL_64)
            set(WINDOW_VERSION "5.01")
        endif(CMAKE_CL_64)
        set(TARGET_LINKER_FLAGS "${TARGET_LINKER_FLAGS} /SUBSYSTEM:\"WINDOWS\",\"${WINDOW_VERSION}\" /ENTRY:\"mainCRTStartup\"")
    endif(ENABLE_PUBLISHER_LEVEL)
    if(ENABLE_ADMINISTRATOR_LEVEL)
        set(TARGET_LINKER_FLAGS "${TARGET_LINKER_FLAGS} /MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\"")
    endif(ENABLE_ADMINISTRATOR_LEVEL)
else(MSVC)
endif(MSVC)
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_compile_definitions(BUILD_DEBUG=1)
else()
    add_compile_definitions()
endif()
if (CMAKE_HOST_APPLE)
    message(STATUS "Platform=APPLE")
    add_compile_definitions(BUILD_PLATFORM_APPLE=1)
elseif (CMAKE_HOST_UNIX)
    message(STATUS "Platform=LINUX")
    add_compile_definitions(BUILD_PLATFORM_LINUX=1)
elseif (CMAKE_HOST_WIN32)
    message(STATUS "Platform=WINDOWS")
    add_compile_definitions(BUILD_PLATFORM_WINDOWS=1)
endif ()

# Include sub-projects.
add_subdirectory ("vendor/zlib-1.2.11")
add_subdirectory ("vendor/libpng")
add_subdirectory ("vendor/libharu_2.3.0")
add_subdirectory ("pdflibapp")
