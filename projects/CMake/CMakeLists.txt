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

    if (APPLE)
        add_executable(${PROJECT_NAME}
                MACOSX_BUNDLE
                ${RFXGEN_SRC}/rfxgen.c
                ${RFXGEN_SRC}/external/tinyfiledialogs.c
                ${RFXGEN_ROOT}/logo/rfxgen.icns
                )

        set_property(SOURCE "${RFXGEN_ROOT}/logo/rfxgen.icns"
                PROPERTY MACOSX_PACKAGE_LOCATION Resources)

        set_target_properties(${PROJECT_NAME} PROPERTIES
                BUNDLE True
                MACOSX_BUNDLE_GUI_IDENTIFIER com.raylibtech.${PROJECT_NAME}
                MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
                MACOSX_BUNDLE_BUNDLE_VERSION "3.3"
                MACOSX_BUNDLE_ICON_FILE rfxgen.icns
                MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2015-2024 raylib technologies (@raylibtech) / Ramon Santamaria (@raysan5)"
                MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/macos/rfxgen.plist.in
                )

    else()
        add_executable(${PROJECT_NAME}
            ${RFXGEN_SRC}/rfxgen.c
            ${RFXGEN_SRC}/external/tinyfiledialogs.c
        )
    endif()

    target_include_directories(${PROJECT_NAME} PUBLIC ${RFXGEN_SRC}/external)
    target_link_libraries(${PROJECT_NAME} PUBLIC raylib)

    # Web Configurations
    if (${PLATFORM} STREQUAL "Web")
        set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    endif()
endif()
