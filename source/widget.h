#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void initialize();

private:
    QTcpSocket * tcp_blockchain_channel;

private slots:
    void new_connection();
    void ready_read();
    void errorOccurred(QAbstractSocket::SocketError socketError);

};
#endif // WIDGET_H
