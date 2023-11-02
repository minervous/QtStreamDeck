#ifndef STREAMDECKORIGINALV2_H
#define STREAMDECKORIGINALV2_H

#include "basedeviceinterface.h"

namespace streamdeck {

class StreamDeckOriginalV2 : public BaseDeviceInterface
{
public:
    enum {
        PID_ORIGINAL_V2 =  0x006d,
        PID_MK2 =  0x0080
    };
    explicit StreamDeckOriginalV2(quint16 pid);
    ~StreamDeckOriginalV2() override = default;

    const Configuration & getConfiguration() const override;
    bool setBrightness(int procentage) override;
    QString getFirmwareVersion() override;

    bool reset() override;
    int readButtonsStatus(QList<bool> &buttonsStates) override;
    bool sendImage(int keyIndex, const QByteArray& imageData) override;
private:
    Configuration m_configuration;
    StreamDeckOriginalV2() = delete;
};

} // namespace

#endif // STREAMDECKORIGINALV2_H
