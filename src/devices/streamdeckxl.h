#ifndef STREAMDECKXL_H
#define STREAMDECKXL_H

#include "basedeviceinterface.h"

namespace streamdeck {

class StreamDeckXL : public BaseDeviceInterface
{
public:
    enum {
        PID_XL =  0x006c,
        PID_XL_V2 = 0x008f
    };
    explicit StreamDeckXL(quint16 pid);
    ~StreamDeckXL() override = default;

    const Configuration & getConfiguration() const override;
    bool setBrightness(int procentage) override;
    QString getFirmwareVersion() override;

    bool reset() override;
    int readButtonsStatus(QList<bool> &buttonsStates) override;
    bool sendImage(int keyIndex, const QByteArray& imageData) override;
private:
    Configuration m_configuration;
    StreamDeckXL() = delete;
};

} // namespace

#endif // STREAMDECKXL_H
