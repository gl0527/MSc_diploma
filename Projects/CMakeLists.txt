cmake_minimum_required (VERSION 3.5.1 FATAL_ERROR)

project (MSC_diploma C CXX)

if (MSVC)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /W4 /EHc")
endif (MSVC)

if (WIN32)
	set (STATIC_LIB_EXTENSION "lib")
	set (DYNAMIC_LIB_EXTENSION "dll")
else ()
	set (STATIC_LIB_EXTENSION "a")
	set (DYNAMIC_LIB_EXTENSION "so")
endif (WIN32)

# todo ezzel is baj van, mert lehet olyan eset, hogy itt Debug-gal generalok, de a VS-ben Release-zel build-elek
set (CONFIG_SUFFIX "")
# if (CMAKE_BUILD_TYPE MATCHES "Debug")

set (CMAKE_BUILD_TYPE Debug)
set (CONFIG_SUFFIX "_d")
# endif ()

add_definitions (-DEXPMODULE)
if (WIN32)
	add_definitions (-D_WINDLL)
else ()
	add_definitions (-DLINUX)
endif ()

if (NOT WIN32)
	find_program (CCACHE_FOUND ccache)
	if (CCACHE_FOUND)
		set_property (GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
	endif (CCACHE_FOUND)
endif ()

# setting up package variables
set (3RDPARTY_DIR 			"${PROJECT_SOURCE_DIR}/../Bin/3rdparty" 		CACHE FILEPATH "")
set (CMAKE_MODULE_PATH 		"${3RDPARTY_DIR}/OGRE-1.9.0-vc140-x86/CMake" 	CACHE FILEPATH "")
set (BOOST_ROOT 			"${3RDPARTY_DIR}/boost-1.61-vc140-x86" 			CACHE FILEPATH "")
set (BULLET_ROOT 			"${3RDPARTY_DIR}/bullet-2.82-r2704" 			CACHE FILEPATH "")
set (OGRE_ROOT 				"${3RDPARTY_DIR}/OGRE-1.9.0-vc140-x86" 			CACHE FILEPATH "")
set (OPENAL_INCLUDE_DIR 	"${3RDPARTY_DIR}/OpenAL/include" 				CACHE FILEPATH "OPENAL_INCLUDE_DIR" FORCE)
set (OPENAL_LIBRARY 		"${3RDPARTY_DIR}/OpenAL/lib" 					CACHE FILEPATH "")
set (TINYXML_INCLUDE_DIR 	"${3RDPARTY_DIR}/tinyxml" 						CACHE FILEPATH "")
set (TINYXML_LIBRARY 		"${TINYXML_INCLUDE_DIR}/Debugtinyxml"			CACHE FILEPATH "")
set (TINYXMLSTL_LIBRARY 	"${TINYXML_INCLUDE_DIR}/DebugtinyxmlSTL"		CACHE FILEPATH "")

# set include and library directory
include_directories (${OGRE_ROOT}/include/OGRE)
link_directories (${OGRE_ROOT}/lib/${CMAKE_BUILD_TYPE})

include_directories (${OGRE_ROOT}/include/OIS)

include_directories (${BOOST_ROOT})
link_directories (${BOOST_ROOT}/lib)

include_directories (${BULLET_ROOT}/src)
link_directories (${BULLET_ROOT}/lib/${CMAKE_BUILD_TYPE})

include_directories (${OPENAL_INCLUDE_DIR})
link_directories (${OPENAL_LIBRARY})

include_directories (${TINYXML_INCLUDE_DIR})
link_directories (${TINYXML_LIBRARY})
link_directories (${TINYXMLSTL_LIBRARY})

if (WIN32)
	set (ADDITIONAL_LIBS_DEBUG "winmm.${STATIC_LIB_EXTENSION}")
endif ()

set (BULLET_LIB_SUFFIX _Debug)

set (LIBRARIES_DEBUG	"OgreMain${CONFIG_SUFFIX}.${STATIC_LIB_EXTENSION}"
						"OIS${CONFIG_SUFFIX}.${STATIC_LIB_EXTENSION}"
						"OgreOverlay${CONFIG_SUFFIX}.${STATIC_LIB_EXTENSION}"
						"BulletCollision${BULLET_LIB_SUFFIX}.${STATIC_LIB_EXTENSION}"
						"BulletDynamics${BULLET_LIB_SUFFIX}.${STATIC_LIB_EXTENSION}"
						"BulletSoftBody${BULLET_LIB_SUFFIX}.${STATIC_LIB_EXTENSION}"
						"LinearMath${BULLET_LIB_SUFFIX}.${STATIC_LIB_EXTENSION}"
						"alut.${STATIC_LIB_EXTENSION}"
						"OpenAL32.${STATIC_LIB_EXTENSION}"
						"tinyxml.${STATIC_LIB_EXTENSION}"
						"tinyxmlSTL.${STATIC_LIB_EXTENSION}"
						"${ADDITIONAL_LIBS_DEBUG}")

add_subdirectory (Engine)
add_subdirectory (ShooterGame)