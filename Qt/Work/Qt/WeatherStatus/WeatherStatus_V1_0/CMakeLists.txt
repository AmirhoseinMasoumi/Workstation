cmake_minimum_required(VERSION 3.16)

project(WeatherStatus_V1_0 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appWeatherStatus_V1_0
    main.cpp
)

qt_add_qml_module(appWeatherStatus_V1_0
    URI WeatherStatus_V1_0
    VERSION 1.0
    QML_FILES Main.qml
    RESOURCES Resources/Fonts/msgothic.ttc
    SOURCES weatherapi.h weatherapi.cpp
    RESOURCES Resources/Icons/drop.png
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appWeatherStatus_V1_0 PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appWeatherStatus_V1_0
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appWeatherStatus_V1_0
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appWeatherStatus_V1_0
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
