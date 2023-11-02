# These variables hold all files:
set(STREAMDECK_SOURCES
    devicemanager.cpp
    device.cpp
    devices/basedeviceinterface.cpp
    devices/streamdeckoriginalv2.cpp
    devices/streamdeckmini.cpp
    devices/streamdeckoriginal.cpp
    devices/streamdeckpedal.cpp
    devices/streamdeckxl.cpp)
set(STREAMDECK_PUBLIC_HEADERS
    devicemanager.h
    device.h
    devicetype.h
    deviceid.h
)
set(STREAMDECK_PRIVATE_HEADERS
    StreamDeckLib_global.h
    devices/deviceinterface.h
    devices/dummydeviceinterface.h
    devices/basedeviceinterface.h
    devices/streamdeckoriginalv2.h
    devices/streamdeckmini.h
    devices/streamdeckoriginal.h
    devices/streamdeckpedal.h
    devices/streamdeckxl.h)

add_library(StreamDeckLib SHARED ${STREAMDECK_SOURCES} ${STREAMDECK_PUBLIC_HEADERS} ${STREAMDECK_PRIVATE_HEADERS})

target_link_libraries(StreamDeckLib PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Qml Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Usb)

target_compile_definitions(StreamDeckLib PRIVATE STREAMDECKLIB_LIBRARY)

install(TARGETS StreamDeckLib LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/install) #${CMAKE_INSTALL_LIBDIR})
install(FILES ${STREAMDECK_PUBLIC_HEADERS} DESTINATION ${CMAKE_BINARY_DIR}/install/include/StreamDeckLib) #include/StreamDeckLib)
