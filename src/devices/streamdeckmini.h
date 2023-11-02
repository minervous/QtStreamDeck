#ifndef STREAMDACKMINI_H
#define STREAMDACKMINI_H

#include "basedeviceinterface.h"

namespace streamdeck {

class StreamDeckMini : public BaseDeviceInterface
{
public:
    enum {
        PID_MINI =  0x0063,
        PID_MINI_MK2 = 0x0090
    };
    explicit StreamDeckMini(quint16 pid);
    ~StreamDeckMini() override = default;

    const Configuration & getConfiguration() const override;
    bool setBrightness(int procentage) override;
    QString getFirmwareVersion() override;

    bool reset() override;
    int readButtonsStatus(QList<bool> &buttonsStates) override;
    bool sendImage(int keyIndex, const QByteArray& imageData) override;
private:
    Configuration m_configuration;
    StreamDeckMini() = delete;
};

} // namespace
#endif // STREAMDACKMINI_H
