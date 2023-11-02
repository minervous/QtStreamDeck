#ifndef DEVICEINTERFACE_H
#define DEVICEINTERFACE_H

#include <QtCore/qtmetamacros.h>
#include <QString>
#include "../StreamDeckLib_global.h"

namespace streamdeck {

class STREAMDECKLIB_EXPORT DeviceInterface
{
public:
    DeviceInterface() = default;
    virtual ~DeviceInterface() = default;

    enum {
        VID =  0x0fd9
    };

    struct Configuration {
        int pid {0};
        int vid {0};
        int keyColumns {0};
        int keyRows {0};
        bool hasDisplay {false};
        int imageWidth {0};
        int imageHeight {0};
        QString imageFormat;
        bool imageHorizontalFlip {false};
        bool imageVerticalFlip {false};
        int imageRotation {0};

        Configuration (int pid = 0,
                      int vid = 0,
                      int keyColumns = 0,
                      int keyRows = 0,
                      bool hasDisplay = false,
                      int imageWidth = 0,
                      int imageHeight = 0,
                      QString imageFormat = {},
                      bool imageHorizontalFlip = false,
                      bool imageVerticalFlip = false,
                      int imageRotation = 0)
            : pid(pid),
            vid(vid),
            keyColumns(keyColumns),
            keyRows(keyRows),
            hasDisplay(hasDisplay),
            imageWidth(imageWidth),
            imageHeight(imageHeight),
            imageFormat(imageFormat),
            imageHorizontalFlip(imageHorizontalFlip),
            imageVerticalFlip(imageVerticalFlip),
            imageRotation(imageRotation)
        {}
    };

	virtual bool open(const QString &serial) = 0;
	virtual void close() = 0;
	virtual bool isOpen() const = 0;
	virtual QString manufacturer() const = 0;
	virtual QString serialNumber() const = 0;
	virtual QString product() const = 0;

    virtual const Configuration & getConfiguration() const = 0;
    virtual bool setBrightness(int procentage) = 0;
    virtual QString getFirmwareVersion() = 0;

    virtual bool reset() = 0;
    virtual int readButtonsStatus(QList<bool> &buttonsStates) = 0;

    virtual bool sendImage(int keyIndex, const QByteArray& imageData) = 0;

protected:
    Q_DISABLE_COPY_MOVE(DeviceInterface)
};

} // namespace

#endif // DEVICEINTERFACE_H

