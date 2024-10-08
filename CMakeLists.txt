cmake_minimum_required(VERSION 3.16)

project(nterm VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Widgets)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appnterm
    source/main.cpp
)

qt_policy(SET QTP0004 NEW)
qt_add_qml_module(appnterm
    URI nterm
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/NoteView.qml
        qml/EditorBtn.qml
        qml/LeftContainer.qml
        qml/FolderList.qml
        qml/NoteList.qml
        qml/EditorToolbar.qml

    SOURCES
        source/EventHandler.h source/EventHandler.cpp
        source/Filedb.h source/Filedb.cpp
        source/PopupHandler.h source/PopupHandler.cpp
        source/Constants.h source/Constants.cpp

    RESOURCES
        assets/logo.png
        fonts.qrc
        icons.qrc
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appnterm PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appnterm
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appnterm
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Widgets
)
target_include_directories(appnterm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source)

include(GNUInstallDirs)
install(TARGETS appnterm
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
