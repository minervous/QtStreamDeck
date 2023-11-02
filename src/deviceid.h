#ifndef DEVICEID_H
#define DEVICEID_H

#include "StreamDeckLib_global.h"
#include <QObject>
#include <QDebug>
#include "devicetype.h"

namespace streamdeck {

class STREAMDECKLIB_EXPORT DeviceId
{
    Q_GADGET

    Q_PROPERTY(streamdeck::DeviceTypeGadget::Type type MEMBER type FINAL)
    Q_PROPERTY(QString serialNumber MEMBER serialNumber FINAL)

public:
    DeviceId() = default;
    ~DeviceId() = default;
    DeviceId(const DeviceId &rth) = default;
    DeviceId &operator=(const DeviceId &) = default;
    DeviceId(const DeviceType type, const QString serialNumber = {})
        : type(type)
        , serialNumber(serialNumber)
    {}

    bool operator==(const DeviceId &rth) const
    {
        return type == rth.type && serialNumber == rth.serialNumber;
    }

    bool operator!=(const DeviceId &rth) const
    {
        return type != rth.type || serialNumber != rth.serialNumber;
    }

    DeviceType type {DeviceType::UNKNOWN_DEVICE};
    QString serialNumber;
};

} // namespace

Q_DECLARE_METATYPE(streamdeck::DeviceId)

inline QTextStream &operator <<(QTextStream& outStream, const streamdeck::DeviceId& value)
{
    outStream << QMetaType::fromType<streamdeck::DeviceId>().name() << "(type:" << value.type << ", serialNumber:" << value.serialNumber << ")";
    return outStream;
}

inline QDebug &operator <<(QDebug& outStream, const streamdeck::DeviceId& value)
{
    outStream << QMetaType::fromType<streamdeck::DeviceId>().name() << "(type:" << value.type << ", serialNumber:" << value.serialNumber << ")";
    return outStream;
}

#endif // DEVICEID_H
