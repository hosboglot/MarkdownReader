cmake_minimum_required(VERSION 3.16)

project(MarkdownReader VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Core Quick)

set(PROJECT_SOURCES
        main.cpp
        qml.qrc
)

if(ANDROID)
    add_library(MarkdownReader SHARED
        ${PROJECT_SOURCES}
        src/repositoryhandler.h src/repositoryhandler.cpp
        src/repositorylistmodel.h src/repositorylistmodel.cpp
        src/repositoryloaders/abstractrepositoryloader.h
        src/repositoryloaders/localrepositoryloader.h src/repositoryloaders/localrepositoryloader.cpp

    )
    # Define properties for Android with Qt 5 after find_package() calls as:
    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    find_package(Qt5 REQUIRED AndroidExtras)
    target_link_libraries(MarkdownReader PRIVATE Qt5::AndroidExtras)
else()
    add_executable(MarkdownReader
      ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(MarkdownReader
  PRIVATE Qt5::Core Qt5::Quick)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MarkdownReader)
set_target_properties(MarkdownReader PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MarkdownReader
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

# if(QT_VERSION_MAJOR EQUAL 6)
    # qt_import_qml_plugins(MarkdownReader)
    # qt_finalize_executable(MarkdownReader)
# endif()
