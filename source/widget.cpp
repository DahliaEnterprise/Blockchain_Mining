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
    connect(miner, SIGNAL(block_found(QByteArray, QByteArray)), this, SLOT(block_found(QByteArray, QByteArray)));

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
    //Incoming raw message
    QByteArray message_raw = tcp_blockchain_channel->readAll();
        //Convert to JSON parseable object.
        QJsonDocument message_jdoc = QJsonDocument::fromJson(message_raw);
        QJsonObject message_jobj = message_jdoc.object();

        miner->initialize(message_jobj);
        miner->mine_in_the_background();
}

void Widget::errorOccurred(QAbstractSocket::SocketError socketError)
{
    qDebug() << "ERROR";
}

void Widget::block_found(QByteArray block_hash, QByteArray exact_message)
{
    QJsonObject block_found;
    block_found.insert(QString("request"), QJsonValue(QString("block_found")));
    QString block_hash_hex;
    block_hash_hex.append(block_hash.toHex());
    block_found.insert(QString("hash"), QJsonValue(block_hash_hex));

    QString message;
    message.append(exact_message);
    block_found.insert(QString("message"), QJsonValue(message));

    QByteArray tcp_message = QJsonDocument(block_found).toJson(QJsonDocument::Compact);
    tcp_blockchain_channel->write(tcp_message);
}
