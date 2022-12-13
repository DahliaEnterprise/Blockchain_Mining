#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
}

Widget::~Widget()
{
}

void Widget::initialize()
{
    tcp_blockchain_channel = new QTcpSocket();
    tcp_blockchain_channel->connectToHost(QHostAddress(QString("127.0.0.1")), 2100);

    /*connect(tcp_blockchain_channel, SIGNAL(connected()), this, SLOT(new_connection()));
    connect(tcp_blockchain_channel, SIGNAL(readyRead()), this, SLOT(ready_read()));
    connect(tcp_blockchain_channel, SIGNAL(errorOccurred(QAbstractSocket::SocketError )), this, SLOT(errorOccurred(QAbstractSocket::SocketError)));
    */
}

void Widget::new_connection()
{
    qDebug() << "NEW CONECTION';";
}

void Widget::ready_read()
{
    qDebug() << tcp_blockchain_channel->readAll();
}

void Widget::errorOccurred(QAbstractSocket::SocketError socketError)
{
    qDebug() << "ERROR";
}
