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
    mining_mode = 0;
    miner = new single_threaded_miner();

    tcp_blockchain_channel = new QTcpSocket();
    tcp_blockchain_channel->connectToHost(QHostAddress(QString("127.0.0.1")), 2100);
    connect(tcp_blockchain_channel, SIGNAL(connected()), this, SLOT(new_connection()));
    connect(tcp_blockchain_channel, SIGNAL(readyRead()), this, SLOT(ready_read()));
    connect(tcp_blockchain_channel, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(errorOccurred(QAbstractSocket::SocketError)));

}

void Widget::new_connection()
{
    //Request next block to mine information
    QJsonObject request;
    request.insert(QString("request"), QJsonValue(QString("next_block")));
    QJsonDocument request_jdoc(request);
    QByteArray request_json_qbytearray = request_jdoc.toJson(QJsonDocument::Compact);
    qDebug() << request_json_qbytearray;
    tcp_blockchain_channel->write(request_json_qbytearray);
    qDebug() << "connection";
}

void Widget::ready_read()
{
    QByteArray message_raw = tcp_blockchain_channel->readAll();
    QJsonDocument message_jdoc = QJsonDocument::fromJson(message_raw);
    QJsonObject message_jobj = message_jdoc.object();
    QString largest_block_height = message_jobj.value(QString("largest_block_height")).toString();
    quint64 largest_block_height_uint64 = largest_block_height.toULongLong();
    if(largest_block_height_uint64 == 0)
    {
        //The blockchain is accepting an initial block.
        mining_mode = 1;
        miner->initialize();
        miner->mine_in_the_background();

    }else if(largest_block_height_uint64 > 0)
    {
        //The blockchain is only accepting next (valid) blocks.
        mining_mode = 2;
    }
}

void Widget::errorOccurred(QAbstractSocket::SocketError socketError)
{
    qDebug() << "ERROR";
}
