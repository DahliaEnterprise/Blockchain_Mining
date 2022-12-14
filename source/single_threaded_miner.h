#ifndef SINGLE_THREADED_MINER_H
#define SINGLE_THREADED_MINER_H

#include <QObject>

class single_threaded_miner : public QObject
{
    Q_OBJECT
public:
    explicit single_threaded_miner(QObject *parent = nullptr);

    void initialize();
    void mine_in_the_background();

private:

signals:

};

#endif // SINGLE_THREADED_MINER_H
