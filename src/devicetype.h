#ifndef DEVICETYPE_H
#define DEVICETYPE_H

#include "QtCore/qmetaobject.h"
#include "StreamDeckLib_global.h"
#include <QObject>
#include <QDebug>

namespace streamdeck {

Q_NAMESPACE

class STREAMDECKLIB_EXPORT DeviceTypeGadget
{
    Q_GADGET

public:
    enum Type {
        UNKNOWN_DEVICE = 0,
        STREAMDECK_ORIGINAL,
        STREAMDECK_ORIGINAL_V2,
        STREAMDECK_MK2,
        STREAMDECK_MINI,
        STREAMDECK_MINI_MK2,
        STREAMDECK_XL,
        STREAMDECK_XL_V2,
        STREAMDECK_PEDAL,
        STREAMDECK_ANY
    };
    Q_ENUM(Type)
};

using DeviceType = DeviceTypeGadget::Type;

} // namespace

Q_DECLARE_METATYPE(streamdeck::DeviceTypeGadget::Type)

inline QTextStream &operator <<(QTextStream& outStream, const streamdeck::DeviceTypeGadget::Type& value)
{
    const QString notValidValue = QStringLiteral("Not valid value");
    QString enumToString {QMetaEnum::fromType<streamdeck::DeviceTypeGadget::Type>().valueToKey(value)};
    outStream << (enumToString.isEmpty() ? notValidValue : enumToString);
    return outStream;
}

inline QDebug &operator <<(QDebug& outStream, const streamdeck::DeviceTypeGadget::Type& value)
{
    const QString notValidValue = QStringLiteral("Not valid value");
    QString enumToString {QMetaEnum::fromType<streamdeck::DeviceTypeGadget::Type>().valueToKey(value)};
    outStream << (enumToString.isEmpty() ? notValidValue : enumToString);
    return outStream;
}

#endif // DEVICETYPE_H
