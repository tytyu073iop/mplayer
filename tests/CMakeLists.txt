find_package(Qt6 6.5 REQUIRED COMPONENTS
    Test
    Quick
    Multimedia
    Sql
)
if(MPLAYER_BUILD_UI_TESTS)
    find_package(Qt6 6.5 REQUIRED COMPONENTS QuickTest)
endif()

# ---- Unit Tests (C++ QtTest) ----

function(add_qt_test name)
    qt_add_executable(${name} ${name}.cpp ${ARGN})
    target_link_libraries(${name} PRIVATE Qt6::Test Qt6::Quick Qt6::Multimedia Qt6::Sql)
    add_test(NAME ${name} COMMAND ${name})
    if(NOT MPLAYER_BUILD_UI_TESTS)
        set_tests_properties(${name} PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
    endif()
endfunction()

add_qt_test(tst_audioengine
    ../src/audioengine.cpp
)
target_include_directories(tst_audioengine PRIVATE ../src)

add_qt_test(tst_playlistmanager
    ../src/playlistmanager.cpp
)
target_include_directories(tst_playlistmanager PRIVATE ../src)

add_qt_test(tst_filebrowser
    ../src/filebrowser.cpp
    ../src/playlistmanager.cpp
)
target_include_directories(tst_filebrowser PRIVATE ../src)

add_qt_test(tst_database
    ../src/databasemanager.cpp
)
target_include_directories(tst_database PRIVATE ../src)

if(MPLAYER_BUILD_UI_TESTS)
    file(COPY
        tst_qml_controls.qml
        tst_qml_playlist.qml
        tst_qml_delegate.qml
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
    )

    # Symlink qml dir so QML tests can resolve ../qml/shared relative paths
    set(qml_target ${CMAKE_CURRENT_BINARY_DIR}/../qml)
    if(NOT EXISTS ${qml_target})
        file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/../qml ${qml_target} SYMBOLIC)
    endif()
endif()

# ---- QML UI Tests ----

if(MPLAYER_BUILD_UI_TESTS)
    qt_add_executable(tst_qml_controls
        tst_qml_controls.cpp
    )
    target_link_libraries(tst_qml_controls PRIVATE Qt6::QuickTest Qt6::Quick)
    target_compile_definitions(tst_qml_controls PRIVATE
        TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
    add_test(NAME tst_qml_controls COMMAND tst_qml_controls)

    qt_add_executable(tst_qml_playlist
        tst_qml_playlist.cpp
    )
    target_link_libraries(tst_qml_playlist PRIVATE Qt6::QuickTest Qt6::Quick)
    target_compile_definitions(tst_qml_playlist PRIVATE
        TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
    add_test(NAME tst_qml_playlist COMMAND tst_qml_playlist)

    qt_add_executable(tst_qml_delegate
        tst_qml_delegate.cpp
    )
    target_link_libraries(tst_qml_delegate PRIVATE Qt6::QuickTest Qt6::Quick)
    target_compile_definitions(tst_qml_delegate PRIVATE
        TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
    add_test(NAME tst_qml_delegate COMMAND tst_qml_delegate)
endif()

# ---- Integration Tests ----

add_qt_test(tst_integration
    ../src/audioengine.cpp
    ../src/playlistmanager.cpp
    ../src/filebrowser.cpp
    ../src/databasemanager.cpp
)
target_include_directories(tst_integration PRIVATE ../src)
