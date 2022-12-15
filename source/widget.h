#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>

#include "single_threaded_miner.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void initialize();

private:
    QTcpSocket * tcp_blockchain_channel;

    uint mining_mode;

    single_threaded_miner * miner;

private slots:
    void new_connection();
    void ready_read();
    void errorOccurred(QAbstractSocket::SocketError socketError);
    void block_found(QByteArray block_hash, QByteArray exact_message);

};
#endif // WIDGET_H
