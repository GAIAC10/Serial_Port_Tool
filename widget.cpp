#include "widget.h"
#include "ui_widget.h"

widget::widget(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::widget)
{
    ui->setupUi(this);

    this->serialPort = new QSerialPort;
    findFreePorts();

    connect(ui->openCom, &QCheckBox::toggled, [=](bool checked) {
        if (checked) {
            initSerialPort();
            ui->btnSend->setEnabled(true);
        }
        else {
            this->serialPort->close();
            ui->btnSend->setEnabled(false);
            ui->openCom->setChecked(false);
        }
    });

    // 接收到串口信息
    connect(this->serialPort, SIGNAL(readyRead()), this, SLOT(recvMsg()));
    // 发送数据
    connect(ui->btnSend, &QPushButton::clicked, [=]() {
        sendMsg(ui->message->toPlainText());
    });
}

widget::~widget()
{
    delete ui;
}

// 寻找空闲状态串口
void widget::findFreePorts() {
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for (int i = 0; i < ports.size(); ++i) {
        if (ports.at(i).isBusy()) {
            ports.removeAt(i);
            continue;
        }
        ui->portName->addItem(ports.at(i).portName());
    }
    if (!ports.size()) {
        QMessageBox::warning(NULL, "Tip", QStringLiteral("Port Not Find"));
        return;
    }
}

// 初始化串口
bool widget::initSerialPort() {
    // 设置串口端口号
    this->serialPort->setPortName(ui->portName->currentText());
    if (!this->serialPort->open(QIODevice::ReadWrite)) {
        QMessageBox::warning(NULL, "Tip", QStringLiteral("Open Serial Port Error"));
        return false;
    }
    // 设置波特率
    this->serialPort->setBaudRate(ui->baudRate->currentText().toInt());

    // 设置数据位
    if (ui->dataBits->currentText().toInt() == 8) {
        this->serialPort->setDataBits(QSerialPort::Data8);
    }
    else if (ui->dataBits->currentText().toInt() == 7) {
        this->serialPort->setDataBits(QSerialPort::Data7);
    }
    else if (ui->dataBits->currentText().toInt() == 6) {
        this->serialPort->setDataBits(QSerialPort::Data6);
    }
    else if (ui->dataBits->currentText().toInt() == 5) {
        this->serialPort->setDataBits(QSerialPort::Data5);
    }

    // 设置停止位
    if (ui->stopBits->currentText().toInt() == 1) {
        this->serialPort->setStopBits(QSerialPort::OneStop);
    }
    else if (ui->stopBits->currentText().toInt() == 2) {
        this->serialPort->setStopBits(QSerialPort::TwoStop);
    }

    // 设置检验位
    if (ui->parity->currentText() == "NoParity") {
        this->serialPort->setParity(QSerialPort::NoParity);
    }
    else if (ui->parity->currentText() == "EvenParity") {
        this->serialPort->setParity(QSerialPort::EvenParity);
    }
    else if (ui->parity->currentText() == "OddParity") {
        this->serialPort->setParity(QSerialPort::OddParity);
    }
    return true;
}

// 向串口发送信息
void widget::sendMsg(const QString& msg) {
    // Hex -> QByteArray
    this->serialPort->write(QByteArray::fromHex(msg.toLatin1()));
    ui->comLog->insertPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " [send] " + msg + "\n");
}

// 接受来自串口的信息
void widget::recvMsg() {
    QByteArray msg = this->serialPort->readAll();
    // QByteArray -> Hex
    ui->comLog->insertPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " [recieve] " + msg.toHex().data() + "\n");
}


