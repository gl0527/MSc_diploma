#Force static runtime libraries - egyelore csak igy mukodik, de ennek nem kene a kodban szerepelnie
FOREACH (flag
 CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_RELWITHDEBINFO
 CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_DEBUG_INIT
 CMAKE_CXX_FLAGS_RELEASE  CMAKE_CXX_FLAGS_RELWITHDEBINFO
 CMAKE_CXX_FLAGS_DEBUG  CMAKE_CXX_FLAGS_DEBUG_INIT)
 STRING (REPLACE "/MD"  "/MT" "${flag}" "${${flag}}")
 SET ("${flag}" "${${flag}} /EHsc")
ENDFOREACH ()

# setting the target name same as the current folder
get_filename_component (TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string (REPLACE " " "_" TARGET_NAME ${TARGET_NAME})

include_directories ("Include")
						
FILE (GLOB_RECURSE HEADER_FILES "Include/*.h")
FILE (GLOB_RECURSE SOURCE_FILES "Source/*.cpp")

add_library (${TARGET_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})

set_target_properties (${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/../Bin/${TARGET_NAME}")
set_target_properties (${TARGET_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/../Lib/${TARGET_NAME}")
set_target_properties (${TARGET_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/../Lib/${TARGET_NAME}")

add_custom_command (TARGET ${TARGET_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy
					"${PROJECT_SOURCE_DIR}/../Bin/${TARGET_NAME}/${CMAKE_BUILD_TYPE}/${TARGET_NAME}.dll"
					"${PROJECT_SOURCE_DIR}/../Bin/ShooterGame/${CMAKE_BUILD_TYPE}/${TARGET_NAME}.dll")

target_link_libraries (${TARGET_NAME} ${LIBRARIES_DEBUG})