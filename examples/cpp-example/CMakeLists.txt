add_executable(stream-deck-cpp-example
  src/main.cpp
  src/SimpleExample.hpp
  src/SimpleExample.cpp
)

target_link_libraries(stream-deck-cpp-example PRIVATE Qt${QT_VERSION_MAJOR}::Core StreamDeckLib)
target_include_directories(stream-deck-cpp-example
  PUBLIC
  ${CMAKE_SOURCE_DIR}/src
)

qt_add_resources(stream-deck-cpp-example "app_images"
  PREFIX "/"
  FILES
    images/Exit.png
    images/Pressed.png
    images/Released.png
)
