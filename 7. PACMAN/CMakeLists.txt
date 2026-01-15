cmake_minimum_required(VERSION 3.16)
project(pacman)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt packages (optional)
find_package(Qt6 COMPONENTS Core Gui Widgets QUIET)
if (NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Core Gui Widgets QUIET)
endif()

if (Qt6_FOUND OR Qt5_FOUND)
    message(STATUS "Qt found, building with Qt support")

    # Qt Configuration
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    include_directories(${CMAKE_SOURCE_DIR}/include)

    set(SOURCES
            src/main.cpp
            src/Game.cpp
            src/Ghost.cpp
            src/Items.cpp
    )

    set(HEADERS
            include/Game.h
            include/Ghost.h
            include/Items.h
    )

    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

    if (Qt6_FOUND)
        target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Widgets)
    else()
        target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets)
    endif()

    # Windows runtime deployment helper
    if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        set(DEBUG_SUFFIX)
        if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
            set(DEBUG_SUFFIX "d")
        endif ()
        if (Qt5_FOUND)
            set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
            set(QT_PLAT_DIR "${QT_INSTALL_PATH}/plugins/platforms")
            set(QT_BIN_DIR "${QT_INSTALL_PATH}/bin")
            set(QT_DLL_PREFIX "Qt5")
        else()
            set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
            set(QT_PLAT_DIR "${QT_INSTALL_PATH}/plugins/platforms")
            set(QT_BIN_DIR "${CMAKE_PREFIX_PATH}/bin")
            set(QT_DLL_PREFIX "Qt6")
        endif()

        if (EXISTS "${QT_PLAT_DIR}/qwindows${DEBUG_SUFFIX}.dll")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    "${QT_PLAT_DIR}/qwindows${DEBUG_SUFFIX}.dll"
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        endif ()

        foreach (QT_LIB Core Gui Widgets)
            if (EXISTS "${QT_BIN_DIR}/${QT_DLL_PREFIX}${QT_LIB}${DEBUG_SUFFIX}.dll")
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy
                        "${QT_BIN_DIR}/${QT_DLL_PREFIX}${QT_LIB}${DEBUG_SUFFIX}.dll"
                        "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
            endif()
        endforeach()
    endif ()
else()
    message(WARNING "Qt not found! This project requires Qt5 or Qt6 to build.")
    message(WARNING "Please install Qt and set CMAKE_PREFIX_PATH to Qt installation directory.")
    # https://www.youtube.com/watch?v=VYnU-DoIlzY
    message(WARNING "Example: cmake -DCMAKE_PREFIX_PATH=C:/Qt/6.9.2/mingw_64")
    message(FATAL_ERROR "Cannot continue without Qt")
endif()
