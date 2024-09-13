cmake_minimum_required(VERSION 3.16)

project(nterm VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(Qt6_DIR "/home/nihshreyas/Qt/6.5.3/gcc_64/lib/cmake/Qt6")

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Widgets)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appnterm
    main.cpp
)

qt_add_qml_module(appnterm
    URI nterm
    VERSION 1.0
    QML_FILES
        Main.qml
        Notes.qml
        EditorBtn.qml
        LeftContainer.qml
    SOURCES
        EventHandler.h
        EventHandler.cpp
        filedb.h
        filedb.cpp
    RESOURCES

    RESOURCES
        assets/delete.svg
        assets/create.svg
        assets/heading.svg
        assets/bold.svg
        assets/underline.svg

        assets/headingActive.svg
        assets/logo.png

        assets/italic.svg assets/italicActive.svg
        assets/underlineActive.svg assets/boldActive.svg
        assets/paragraph.svg assets/paragraphActive.svg
        assets/fonts/inter/InterRegular.ttf
        assets/fonts/inter/InterBlack.ttf
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

include(GNUInstallDirs)
install(TARGETS appnterm
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
