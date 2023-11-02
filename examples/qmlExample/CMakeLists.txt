find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Quick)

add_executable(StreamDeckLibQmlExample main.cpp
    QmlDeckExample.qml)
qt_add_qml_module(StreamDeckLibQmlExample
    URI QmlExampleProjectApp
    VERSION 1.0
    QML_FILES QmlDeckExample.qml
)

target_link_libraries(StreamDeckLibQmlExample PRIVATE Qt${QT_VERSION_MAJOR}::Quick StreamDeckQml StreamDeckLib)
target_compile_definitions(StreamDeckLibQmlExample PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
