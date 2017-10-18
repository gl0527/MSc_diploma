cmake_minimum_required (VERSION 3.5.1 FATAL_ERROR)

project (MSC_diploma C CXX)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP2 /EHc")

# add preprocessor definitions
add_definitions (-DEXPMODULE)
if (WIN32)
	add_definitions (-D_WINDLL)
else ()
	add_definitions (-DLINUX)
endif ()

find_package (OGRE REQUIRED)
if (OGRE_FOUND)
	include_directories ("${OGRE_INCLUDE_DIRS}")
	link_directories ("${OGRE_LIBRARY_DIRS}")
	message (STATUS "================== OGRE found ==================")
else ()
	message (FATAL_ERROR "OGRE not found!")
endif (OGRE_FOUND)

find_package (OIS REQUIRED)
if (OIS_FOUND)
	include_directories ("${OIS_INCLUDE_DIRS}")
	message (STATUS "================== OIS found =================== ")
else ()
	message (FATAL_ERROR "OIS not found!")
endif (OIS_FOUND)

set (BOOST_ROOT "e:/dev/tools/OGRE-SDK-1.9.0-vc140-x86-12.03.2016/boost/")
find_package (Boost REQUIRED)
if (Boost_FOUND)
	include_directories ("${Boost_INCLUDE_DIRS}")
	link_directories ("${Boost_LIBRARY_DIRS}")
	message (STATUS "================= Boost found ================== ")
else ()
	message (FATAL_ERROR "Boost not found!")
endif (Boost_FOUND)

set (BULLET_ROOT "e:/dev/tools/bullet-2.82-r2704/")
find_package (BULLET REQUIRED)
if (BULLET_FOUND)
	include_directories ("${BULLET_INCLUDE_DIRS}")
	message (STATUS "================= Bullet found =================")
else ()
	message (FATAL_ERROR "Bullet not found!")
endif (BULLET_FOUND)

set (OPENAL_INCLUDE_DIR "e:/dev/tools/OpenAL/include/")
set (OPENAL_LIBRARY "e:/dev/tools/OpenAL/lib/")
find_package (OPENAL REQUIRED)
if (OPENAL_FOUND)
	include_directories ("${OPENAL_INCLUDE_DIR}")
	link_directories ("${OPENAL_LIBRARY}")
	message (STATUS "================= OpenAL found ================= ")
else ()
	message (FATAL_ERROR "OpenAL not found!")
endif (OPENAL_FOUND)

set (TINYXML_INCLUDE_DIR "e:/dev/tools/tinyxml/")
include_directories ("${TINYXML_INCLUDE_DIR}")
set (TINYXML_LIB_DEBUG "e:/dev/tools/tinyxml/Debugtinyxml/tinyxml.lib")
set (TINYXMLSTL_LIB_DEBUG "e:/dev/tools/tinyxml/DebugtinyxmlSTL/tinyxmlSTL.lib")

set (ADDITIONAL_LIBS_DEBUG "")
if (WIN32)
	set (ADDITIONAL_LIBS_DEBUG "winmm.lib")
endif ()

set (LIBRARIES_DEBUG 	"OgreMain_d.lib"
						"OgreOverlay_d.lib"
						"alut.lib"
						"OpenAL32.lib"
						"${OIS_LIBRARIES}"
						"${BULLET_LIBRARIES}"
						"${TINYXML_LIB_DEBUG}"
						"${TINYXMLSTL_LIB_DEBUG}"
						"${ADDITIONAL_LIBS_DEBUG}")

add_subdirectory (Engine)
add_subdirectory (ShooterGame)