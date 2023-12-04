#ifndef SERIAL_PORT_TOOL_WIDGET_H
#define SERIAL_PORT_TOOL_WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <QMessageBox>
#include <QDateTime>
#include "ui_widget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class widget; }
QT_END_NAMESPACE

class widget : public QWidget {
Q_OBJECT

public:
    explicit widget(QWidget *parent = nullptr);
    ~widget() override;

protected:
    void findFreePorts();
    bool initSerialPort();
    void sendMsg(const QString& msg);

public slots:
    void recvMsg();

private:
    Ui::widget *ui;
    QSerialPort* serialPort;
};

#endif //SERIAL_PORT_TOOL_WIDGET_H
