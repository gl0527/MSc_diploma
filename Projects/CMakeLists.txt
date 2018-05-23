cmake_minimum_required (VERSION 3.6.2 FATAL_ERROR)
project (MSC_diploma)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX _d)
endif()

if (MSVC)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /W4 /EHc")
	set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
endif ()

set_property (GLOBAL PROPERTY DEBUG_CONFIGURATIONS "Debug")

add_definitions (-DEXPMODULE)
add_definitions (-D_WINDLL)

set (BOOST_INCLUDE_DIR 				"" CACHE STRING "e:/dev/tools/OGRE-SDK-1.9.0-vc140-x86-12.03.2016/boost/")
set (BOOST_LIBRARY_DIR 				"" CACHE STRING "e:/dev/tools/OGRE-SDK-1.9.0-vc140-x86-12.03.2016/boost/lib/")
set (BULLET_INCLUDE_DIR				"" CACHE STRING "e:/dev/tools/bullet-2.82-r2704/src/")
set (BULLET_LIBRARY_DIR				"" CACHE STRING "e:/dev/tools/bullet-2.82-r2704/lib/")
set (OGRE_INCLUDE_DIR 				"" CACHE STRING "e:/dev/tools/OGRE-SDK-1.9.0-vc140-x86-12.03.2016/include/OGRE/")
set (OGRE_LIBRARY_DIR 				"" CACHE STRING "e:/dev/tools/OGRE-SDK-1.9.0-vc140-x86-12.03.2016/lib/")
set (OIS_INCLUDE_DIR 				"" CACHE STRING "e:/dev/tools/OGRE-SDK-1.9.0-vc140-x86-12.03.2016/include/OIS/")
set (OPENAL_INCLUDE_DIR 			"" CACHE STRING "e:/dev/tools/OpenAL/include/")
set (OPENAL_LIBRARY 				"" CACHE STRING "e:/dev/tools/OpenAL/lib/")
set (TINYXML_INCLUDE_DIR 			"" CACHE STRING "e:/dev/tools/tinyxml/")
set (MYGUI_MYGUIENGINE_INCLUDE_DIR	"" CACHE STRING "E:/dev/tools/MyGUI_3.2.0/MyGUIEngine/include/")
set (MYGUI_OGREPLATFORM_INCLUDE_DIR	"" CACHE STRING "e:/dev/tools/MyGUI_3.2.0/Platforms/Ogre/OgrePlatform/include/")
set (MYGUI_LIBRARY_DIR				"" CACHE STRING "e:/dev/tools/MyGUI_3.2.0/build/lib/")
set (FREETYPE_LIBRARY_DIR			"" CACHE STRING "e:/dev/tools/MyGUI_3.2.0/Dependencies/freetype-2.8.1/cmake_out/")

include_directories (${OGRE_INCLUDE_DIR})
include_directories (${OIS_INCLUDE_DIR})
include_directories (${BOOST_INCLUDE_DIR})
include_directories (${BULLET_INCLUDE_DIR})
include_directories (${OPENAL_INCLUDE_DIR})
include_directories (${TINYXML_INCLUDE_DIR})
include_directories (${MYGUI_MYGUIENGINE_INCLUDE_DIR})
include_directories (${MYGUI_OGREPLATFORM_INCLUDE_DIR})

link_directories (${BOOST_LIBRARY_DIR})
link_directories (${OPENAL_LIBRARY})
link_directories (${BULLET_LIBRARY_DIR})
link_directories (${OGRE_LIBRARY_DIR})
link_directories (${TINYXML_INCLUDE_DIR}/$(Configuration)tinyxml)
link_directories (${TINYXML_INCLUDE_DIR}/$(Configuration)tinyxmlSTL)
link_directories (${MYGUI_LIBRARY_DIR})
link_directories (${FREETYPE_LIBRARY_DIR})

set (LIBS
	"alut.lib"
	"OpenAL32.lib"
	"tinyxml.lib"
	"tinyxmlSTL.lib"
	"winmm.lib")

set (DBLIBS	
	debug "OgreMain_d.lib"
	debug "OIS_d.lib"
	debug "OgreOverlay_d.lib"
	debug "BulletCollision_Debug.lib"
	debug "BulletDynamics_Debug.lib"
	debug "BulletSoftBody_Debug.lib"
	debug "LinearMath_Debug.lib"
	debug "MyGUIEngine_d.lib"
	debug "MyGUI.OgrePlatform_d.lib"
	debug "freetyped.lib")

set (RELLIBS	
	optimized "OgreMain.lib"
	optimized "OIS.lib"
	optimized "OgreOverlay.lib"
	optimized "BulletCollision.lib"
	optimized "BulletDynamics.lib"
	optimized "BulletSoftBody.lib"
	optimized "LinearMath.lib"
	optimized "MyGUIEngine.lib"
	optimized "MyGUI.OgrePlatform.lib"
	optimized "freetype.lib")

add_subdirectory (Engine)
add_subdirectory (ShooterGame)