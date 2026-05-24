cmake_minimum_required(VERSION 3.16)

project(mplayer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

option(MPLAYER_BUILD_UI_TESTS "Build QML UI tests (tst_qml_*)" ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS
    Quick
    QuickControls2
    Multimedia
    Sql
    LinguistTools
)

qt_policy(SET QTP0004 NEW)
qt_standard_project_setup(REQUIRES 6.5)

set(SOURCES
    main.cpp
    src/audioengine.cpp
    src/playlistmanager.cpp
    src/filebrowser.cpp
    src/databasemanager.cpp
    src/appcontroller.cpp
)

set(HEADERS
    src/audioengine.h
    src/playlistmanager.h
    src/filebrowser.h
    src/databasemanager.h
    src/appcontroller.h
)

qt_add_executable(appmplayer ${SOURCES} ${HEADERS})

qt_add_qml_module(appmplayer
    URI mplayer
    VERSION 1.0
    QML_FILES
        Main.qml
        qml/shared/PlayerControls.qml
        qml/shared/PlaylistView.qml
        qml/shared/TrackDelegate.qml
        qml/shared/AlbumCover.qml
        qml/shared/ErrorDialog.qml
        qml/android/AndroidMainPage.qml
        qml/android/NavigationDrawer.qml
        qml/ios/IosMainPage.qml
        qml/ios/NowPlayingView.qml
        qml/linux/LinuxMainPage.qml
        qml/linux/MenuBar.qml
)

qt_add_translations(appmplayer
    TS_FILES
        locales/mplayer_en.ts
        locales/mplayer_ru.ts
        locales/mplayer_be.ts
)

target_link_libraries(appmplayer
    PRIVATE
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Multimedia
        Qt6::Sql
)

set_target_properties(appmplayer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appmplayer
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
if(ANDROID)
    set_target_properties(appmplayer PROPERTIES
        QT_ANDROID_MIN_SDK_VERSION 24
        QT_ANDROID_TARGET_SDK_VERSION 36
    )
endif()

install(TARGETS appmplayer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Android")
    enable_testing()
    add_subdirectory(tests)

    if(MPLAYER_BUILD_UI_TESTS)
        set(_ctest_command ${CMAKE_CTEST_COMMAND} --output-on-failure)
    else()
        set(_ctest_command
            ${CMAKE_COMMAND} -E env QT_QPA_PLATFORM=offscreen
            ${CMAKE_CTEST_COMMAND} --output-on-failure)
    endif()
    add_custom_target(run_tests
        COMMAND ${_ctest_command}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    set(_run_tests_deps
        tst_audioengine
        tst_playlistmanager
        tst_filebrowser
        tst_database
        tst_integration
    )
    if(MPLAYER_BUILD_UI_TESTS)
        list(APPEND _run_tests_deps
            tst_qml_controls
            tst_qml_playlist
            tst_qml_delegate
        )
    endif()
    add_dependencies(run_tests appmplayer ${_run_tests_deps})
endif()
