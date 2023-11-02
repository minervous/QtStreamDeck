add_executable(StreamDeckLibSimpleExample main.cpp simpleexample.cpp simpleexample.h)

target_link_libraries(StreamDeckLibSimpleExample PRIVATE Qt${QT_VERSION_MAJOR}::Core StreamDeckLib)
target_include_directories(StreamDeckLibSimpleExample
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
  )
qt_add_resources(StreamDeckLibSimpleExample "app_images"
    PREFIX "/"
    FILES
        images/Pressed.png
        images/Released.png
        images/Exit.png
)
