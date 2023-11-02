#ifndef STREAMDECKPEDAL_H
#define STREAMDECKPEDAL_H

#include "basedeviceinterface.h"

namespace streamdeck {

class StreamDeckPedal : public BaseDeviceInterface
{
public:
    enum {
        PID =  0x0086
    };
    StreamDeckPedal() = default;
    ~StreamDeckPedal() override = default;

    const Configuration & getConfiguration() const override;
    bool setBrightness(int procentage) override;
    QString getFirmwareVersion() override;

    bool reset() override;
    int readButtonsStatus(QList<bool> &buttonsStates) override;
    bool sendImage(int keyIndex, const QByteArray& imageData) override;
};

} // namespace

#endif // STREAMDECKPEDAL_H
