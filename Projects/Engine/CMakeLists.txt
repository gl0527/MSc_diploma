macro (create_vs_filters file_list)
	foreach(_header IN ITEMS ${ARGV})
		get_filename_component(_header_path "${_header}" PATH)
		file(RELATIVE_PATH _header_path_rel "${PROJECT_SOURCE_DIR}/Engine" "${_header_path}")
		string(REPLACE "/" "\\" _group_path "${_header_path_rel}")
		source_group("${_group_path}" FILES "${_header}")
	endforeach()
endmacro ()

#Force static runtime libraries
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

set (EXTENDED_TARGET_NAME ${TARGET_NAME}${CONFIG_SUFFIX})

include_directories ("Include")
						
FILE (GLOB_RECURSE HEADER_FILES LIST_DIRECTORIES false "Include/*.h")
FILE (GLOB_RECURSE SOURCE_FILES LIST_DIRECTORIES false "Source/*.cpp")

add_library (${EXTENDED_TARGET_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})

create_vs_filters (${HEADER_FILES})
create_vs_filters (${SOURCE_FILES})

set_target_properties (${EXTENDED_TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/../Bin/${TARGET_NAME}")
set_target_properties (${EXTENDED_TARGET_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/../Lib/${TARGET_NAME}")
set_target_properties (${EXTENDED_TARGET_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/../Lib/${TARGET_NAME}")

add_custom_command (TARGET ${EXTENDED_TARGET_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy
					"${PROJECT_SOURCE_DIR}/../Bin/${TARGET_NAME}/${CMAKE_BUILD_TYPE}/${EXTENDED_TARGET_NAME}.${DYNAMIC_LIB_EXTENSION}"
					"${PROJECT_SOURCE_DIR}/../Bin/ShooterGame/${CMAKE_BUILD_TYPE}/${EXTENDED_TARGET_NAME}.${DYNAMIC_LIB_EXTENSION}")

target_link_libraries (${EXTENDED_TARGET_NAME} ${LIBRARIES_DEBUG})