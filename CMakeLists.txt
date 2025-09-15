cmake_minimum_required(VERSION 3.16)

project(CutScene VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)
find_package(FFMPEG REQUIRED)

qt_standard_project_setup()

qt_add_executable(appCutScene
    src/main.cpp
    src/VideoThumbnailLoader.cpp
    src/VideoThumbnailLoader.h
    src/MediaItemModel.h
    src/MediaItemModel.cpp
    src/MediaImageProvider.h
)

qt_add_qml_module(appCutScene
    URI CutScene
    VERSION 1.0
    QML_FILES
	qml/Main.qml
	QML_FILES qml/components/ToolBar.qml
	RESOURCES qml.qrc
)

qt_add_resources(appCutScene "icons"
	PREFIX "/"
	FILES qml/assets/icons/folder-open.svg
	FILES qml/assets/icons/save.svg
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appCutScene PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appCutScene
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_include_directories(appCutScene PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_directories(appCutScene PRIVATE ${FFMPEG_LIBRARY_DIRS})


target_link_libraries(appCutScene
    PRIVATE Qt6::Quick
    PRIVATE avcodec
    PRIVATE avformat
    PRIVATE avutil
    PRIVATE swscale
)

include(GNUInstallDirs)
install(TARGETS appCutScene
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
