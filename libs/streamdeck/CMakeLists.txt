# These variables hold all files:
set(STREAMDECK_SOURCES
    src/DeviceManager.cpp
    src/Device.cpp
    src/Devices/AbstractDeviceBase.cpp
    src/Devices/StreamDeckOriginalv2.cpp
    src/Devices/StreamDeckMini.cpp
    src/Devices/StreamDeckOriginal.cpp
    src/Devices/StreamDeckPedal.cpp
    src/Devices/StreamDeckXL.cpp)
set(STREAMDECK_PUBLIC_HEADERS
    include/minervous/stream-deck/DeviceManager.hpp
    include/minervous/stream-deck/Device.hpp
    include/minervous/stream-deck/DeviceType.hpp
    include/minervous/stream-deck/DeviceId.hpp
)
set(STREAMDECK_PRIVATE_HEADERS
    include/minervous/stream-deck/StreamDeckLib_global.hpp
    src/Devices/IDevice.hpp
    src/Devices/DummyDevice.hpp
    src/Devices/AbstractDeviceBase.hpp
    src/Devices/StreamDeckOriginalv2.hpp
    src/Devices/StreamDeckMini.hpp
    src/Devices/StreamDeckOriginal.hpp
    src/Devices/StreamDeckPedal.hpp
    src/Devices/StreamDeckXL.hpp)

add_library(StreamDeckLib SHARED ${STREAMDECK_SOURCES} ${STREAMDECK_PUBLIC_HEADERS} ${STREAMDECK_PRIVATE_HEADERS})

target_link_libraries(StreamDeckLib PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Qml Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Usb)

target_compile_definitions(StreamDeckLib PRIVATE STREAMDECKLIB_LIBRARY)

target_include_directories(StreamDeckLib PRIVATE include/minervous/stream-deck)

install(TARGETS StreamDeckLib LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/install) #${CMAKE_INSTALL_LIBDIR})
install(FILES ${STREAMDECK_PUBLIC_HEADERS} DESTINATION ${CMAKE_BINARY_DIR}/install/include/StreamDeckLib) #include/StreamDeckLib)
