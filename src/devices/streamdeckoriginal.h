#ifndef STREAMDECKORIGINAL_H
#define STREAMDECKORIGINAL_H

#include "basedeviceinterface.h"

namespace streamdeck {

class StreamDeckOriginal : public BaseDeviceInterface
{
public:
    enum {
        PID =  0x0060
    };
    StreamDeckOriginal() = default;
    ~StreamDeckOriginal() override = default;

    const Configuration & getConfiguration() const override;
    bool setBrightness(int procentage) override;
    QString getFirmwareVersion() override;

    bool reset() override;
    int readButtonsStatus(QList<bool> &buttonsStates) override;
    bool sendImage(int keyIndex, const QByteArray& imageData) override;
};

} // namespace
#endif // STREAMDECKORIGINAL_H
