find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Quick)

add_executable(stream-deck-qml-example
    src/main.cpp
    qml/minervous/streamdeck/example/QmlDeckExample.qml
)
qt_add_qml_module(stream-deck-qml-example
    URI minervous.streamdeck.example
    VERSION 1.0
    QML_FILES qml/minervous/streamdeck/example/QmlDeckExample.qml
)

target_link_libraries(stream-deck-qml-example PRIVATE Qt${QT_VERSION_MAJOR}::Quick ext-minervous-streamdeck lib-streamdeck)
target_compile_definitions(stream-deck-qml-example PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
