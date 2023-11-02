#ifndef SimpleExample_H
#define SimpleExample_H

#include <QObject>

namespace streamdeck {
    class Device;
}

class SimpleExample : public QObject
{
    Q_OBJECT
public:
    explicit SimpleExample(QObject *parent = Q_NULLPTR);
    ~SimpleExample(void);

    bool conneceted() const;
signals:
    void readyToClose();
private:
    QScopedPointer<streamdeck::Device> device;

    void onConnected();

};

#endif // SimpleExample_H
