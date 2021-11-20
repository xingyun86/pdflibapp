# CMakeList.txt : CMake project for pdflibapp, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(project_name "pdflibapp")
project(${project_name})

if (CMAKE_HOST_APPLE)
    message(STATUS "Platform=APPLE")
    add_compile_definitions(BUILD_PLATFORM_APPLE=1)
	set(LIBS_PATH "Apple")
elseif (CMAKE_HOST_UNIX)
    message(STATUS "Platform=LINUX")
    add_compile_definitions(BUILD_PLATFORM_LINUX=1)
	set(LIBS_PATH "Linux/Raspi")
elseif (CMAKE_HOST_WIN32)
    message(STATUS "Platform=WINDOWS")
    add_compile_definitions(BUILD_PLATFORM_WINDOWS=1)
	set(LIBS_PATH "Windows")
	if (CMAKE_BUILD_TYPE STREQUAL Debug)
		set(SUFFIX_T d)
	else()
		set(SUFFIX_T )
	endif()
endif ()
if(CMAKE_CL_64)
    set(LIBS_PATH "x64/${LIBS_PATH}")
else(CMAKE_CL_64)
    set(LIBS_PATH "x32/${LIBS_PATH}")
endif(CMAKE_CL_64)

message(STATUS "LIBS_PATH=${LIBS_PATH}")

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_SOURCE_DIR}/vendor/libharu_2.3.0/include
	${CMAKE_BINARY_DIR}/vendor/libharu_2.3.0/include
)

link_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_BINARY_DIR}/vendor/libharu_2.3.0
)

if(MSVC)
	include_directories(
		${CMAKE_SOURCE_DIR}/vendor/libs/${LIBS_PATH}/include
	)
	link_directories(
		${CMAKE_SOURCE_DIR}/vendor/libs/${LIBS_PATH}/lib
	)
else(MSVC)
	include_directories(
		${CMAKE_SOURCE_DIR}/vendor/libs/${LIBS_PATH}/include
	)
	link_directories(
		${CMAKE_SOURCE_DIR}/vendor/libs/${LIBS_PATH}/lib
	)
endif(MSVC)
file(GLOB SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

if(MSVC)
	list(APPEND LIBS 
		hpdfs
		)
else(MSVC)
	list(APPEND LIBS
		z
		)
endif(MSVC)

add_executable(${project_name} ${SRCS})

target_link_libraries(${project_name} PRIVATE ${LIBS})

if(MSVC)
	#file(GLOB DOCS_FILES "${CMAKE_SOURCE_DIR}/docs/*")
	#file(GLOB FFMPEG_DLL_FILES "${CMAKE_SOURCE_DIR}/vendor/libs/${LIBS_PATH}/ffmpeg-4.4/bin/*.dll")
	#add_custom_command(
	#	TARGET ${project_name}
	#	POST_BUILD
	#	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DOCS_FILES} ${CMAKE_CURRENT_BINARY_DIR}/
	#	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FFMPEG_DLL_FILES} ${CMAKE_CURRENT_BINARY_DIR}/
	#	)
endif(MSVC)

# TODO: Add tests and install targets if needed.
