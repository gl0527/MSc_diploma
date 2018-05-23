get_filename_component (TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string (REPLACE " " "_" TARGET_NAME ${TARGET_NAME})

include_directories ("Include")
include_directories ("${PROJECT_SOURCE_DIR}/Engine/Include")

FILE (GLOB_RECURSE HEADER_FILES "Include/*.h")
FILE (GLOB_RECURSE SOURCE_FILES "Source/*.cpp")

add_executable (${TARGET_NAME} ${SOURCE_FILES} ${HEADER_FILES})

set_target_properties (${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/../Bin/${TARGET_NAME}")
set_target_properties (${TARGET_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/../Lib/${TARGET_NAME}")
set_target_properties (${TARGET_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/../Lib/${TARGET_NAME}")
set_target_properties (${TARGET_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_link_libraries (${TARGET_NAME} ${LIBS} ${DBLIBS} ${RELLIBS})
target_link_libraries (${TARGET_NAME} "${PROJECT_SOURCE_DIR}/../Lib/Engine/$(Configuration)/Engine$<$<CONFIG:Debug>:${CMAKE_DEBUG_POSTFIX}>.lib")

add_dependencies(${TARGET_NAME} Engine)