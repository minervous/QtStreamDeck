set(QT_QML_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR})

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Qml Core Quick)

qt_add_qml_module(StreamDeckQml
    #PLUGIN_TARGET StreamDeckQml
    URI StreamDeckQml
    VERSION 1.0
    #NO_GENERATE_PLUGIN_SOURCE
    #NO_PLUGIN_OPTIONAL
    QML_FILES
    SOURCES streamdeckqmlplugin.cpp streamdeckqmlplugin.h

)

set_target_properties(StreamDeckQml PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(StreamDeckQml
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(StreamDeckQml
    PRIVATE Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Qml Qt${QT_VERSION_MAJOR}::Core StreamDeckLib)

target_include_directories(StreamDeckQml
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
  )

target_include_directories(StreamDeckQml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
