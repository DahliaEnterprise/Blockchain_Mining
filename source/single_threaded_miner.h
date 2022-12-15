#ifndef SINGLE_THREADED_MINER_H
#define SINGLE_THREADED_MINER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCryptographicHash>
#include <QRandomGenerator64>
#include <QTimer>
class single_threaded_miner : public QObject
{
    Q_OBJECT
public:
    explicit single_threaded_miner(QObject *parent = nullptr);

    void initialize(QJsonObject set_initial_block_parameters);


private:
    QJsonObject block_parameters;
    int difficulty(QByteArray hash);

signals:
    void block_found(QByteArray, QByteArray);

public slots:
    void mine_in_the_background();

};

#endif // SINGLE_THREADED_MINER_H
