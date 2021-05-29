#include "tcpclient.h"
#include "ui_tcpclient.h"

TcpClient::TcpClient(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    qTcpSocket = new QTcpSocket(this);
    in.setDevice(qTcpSocket);
    // connect pushButton to quit
    connect(ui->btn_close, &QAbstractButton::clicked, this, &QWidget::close);
    // connect request message
    connect(ui->btn_request, &QAbstractButton::clicked, this, &TcpClient::requestMessage);
    // connect read message
    connect(qTcpSocket, &QIODevice::readyRead, this, &TcpClient::readMessage);
    // connect show error
    connect(qTcpSocket, &QAbstractSocket::errorOccurred, this, &TcpClient::showError);
}

TcpClient::~TcpClient()
{
    delete ui;
}

void TcpClient::requestMessage()
{
    qInfo() << "request Message";
    QString ipAddress = ui->le_ip->text();
    QString port = ui->le_port->text();
    qTcpSocket->connectToHost(ipAddress, port.toInt());
    qInfo() << "ip: " + ipAddress + ", port:" + port + "\n";

}

void TcpClient::readMessage()
{
    qInfo() << "read Message";
    in.startTransaction();
    QString message;
    in >> message;
    if(!in.commitTransaction()){
        qInfo() << "Error in datastream.\n";
        return;
    }
    ui->lbl_message->setText(message);
}

void TcpClient::showError(QAbstractSocket::SocketError socketError)
{
    qInfo() << "show Error";
    switch(socketError){
    case 0:
        qInfo() << "Conexión rechazada";
        break;
    case 1:
        qInfo() << "El servidor cerró la conexión";
        break;
    default:
        qInfo() << "Error no detectado";
    }
}

