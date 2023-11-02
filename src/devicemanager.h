#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include <QQmlEngine>
#include <QtUsb/QUsb>
#include <QList>
#include "QtQml/qqmlengine.h"
#include "deviceid.h"
#include "StreamDeckLib_global.h"

namespace streamdeck {

class STREAMDECKLIB_EXPORT DeviceManager: public QObject
{
    Q_OBJECT

    Q_PROPERTY(DeviceIdList devices READ devices NOTIFY devicesChanged FINAL)

public:
    static DeviceManager* instance();
    static DeviceManager* create(QQmlEngine *, QJSEngine *);
    typedef QList<DeviceId> DeviceIdList;

    DeviceIdList devices();

    static Q_INVOKABLE DeviceId createDeviceId(DeviceType type, QString serialNumber = {});
    static DeviceType convert(quint16 vid, quint16 pid);
signals:
    void devicesChanged();
    void inserted(DeviceId);
    void removed(DeviceId);

private:
    DeviceManager();
    Q_DISABLE_COPY_MOVE(DeviceManager)

    DeviceId getDeviceId(QUsb::Id id) const;
    void onDevInserted(QUsb::Id id);
    void onDevRemoved(QUsb::Id id);
    QUsb m_usb;
    DeviceIdList m_deviceList;
};

//property Device device;

//Connections {
//    target: DeviceManager

//    onDeviceChanged:
//    {
//        device = DeviceManager.connectedDevices[0];
//    }
//}

//StreamDeck {
//    id: myDeck

//    service: DeviceManager.connectedDevices[0]
//                  name
//                  keysCount
//    Item {
//        id: _statePressedButton3
//            Image {}
//        Text {}
//    }

//    buttonsModel: StreamDeckGridMOdel {
//        id: modelState1
//        StremDeckGridEntry {
//            imageNormal:
//            imagePressed: imageNormal
//            itemNormal: imageNormal
//            itemPressed: imagePressed
//            pressedAction:
//            releasedAction: myDeck.buttonsModel = modelState2;
//        }
//    }


//    onPressed(index): {

//    }
//}


} //namespace
#endif // DEVICEMANAGER_H
