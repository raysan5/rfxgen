cmake_minimum_required(VERSION 3.11)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
project(rfxgen C)

# Options
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(RFXGEN_IS_MAIN TRUE)
else()
    set(RFXGEN_IS_MAIN FALSE)
endif()
option(BUILD_RFXGEN "Build ${PROJECT_NAME}" ${RFXGEN_IS_MAIN})

# Directory Variables
set(RFXGEN_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(RFXGEN_SRC ${RFXGEN_ROOT}/src)

# Application
if(${BUILD_RFXGEN})
    find_package(Raylib)

    add_executable(${PROJECT_NAME}
        ${RFXGEN_SRC}/rfxgen.c
        ${RFXGEN_SRC}/external/tinyfiledialogs.c
    )

    target_include_directories(${PROJECT_NAME} PUBLIC ${RFXGEN_SRC}/external)
    target_link_libraries(${PROJECT_NAME} PUBLIC raylib)

    # Web Configurations
    if (${PLATFORM} STREQUAL "Web")
        set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    endif()

    # MacOS
    if (APPLE)
        target_link_libraries(${PROJECT_NAME} "-framework IOKit")
        target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
        target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
    endif()
endif()