#include "single_threaded_miner.h"

single_threaded_miner::single_threaded_miner(QObject *parent)
    : QObject{parent}
{

}



void single_threaded_miner::initialize(QJsonObject set_initial_block_parameters)
{

    block_parameters = set_initial_block_parameters;
}

void single_threaded_miner::mine_in_the_background()
{
    uint this_block_found = 0;

    QJsonDocument copy = QJsonDocument(block_parameters);
    QJsonObject copy_ofinitial_block_parameters = copy.object();
    copy_ofinitial_block_parameters.insert("timestamp", QJsonValue(QString("%1").arg(QDateTime::currentMSecsSinceEpoch())));

    quint64 random_nonce = QRandomGenerator::global()->generate64();
    copy_ofinitial_block_parameters.insert("nonce", QJsonValue(QString("%1").arg(random_nonce)));
    QJsonDocument doc(copy_ofinitial_block_parameters);
    QString string_to_hash;
    string_to_hash.append(doc.toJson(QJsonDocument::Compact));

    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(string_to_hash.toLatin1());
    QByteArray result = hash.result();

    int difficulty = this->difficulty(result);
    if(difficulty > 0)
    {
        int blockchain_difficulty = block_parameters.value("difficulty").toInt();
        if(difficulty >= blockchain_difficulty)
        {
            emit block_found(hash.result(), string_to_hash.toLatin1());
            this_block_found = 0;
        }
    }


    if(this_block_found == 0){
        QTimer::singleShot(10, this, SLOT(mine_in_the_background()));
    }
}

int single_threaded_miner::difficulty(QByteArray hash)
{

    uint keep_searching = 1;
    uint index = 0;
    int difficulty = 0;
    while(keep_searching == 1)
    {
        QByteArray first_character;
        first_character.append(hash.toHex().at(index));

        QByteArray zero;
        zero.append((char)'0');
        if(strncmp((const char *)first_character.data(), (const char *)zero.data(), 1) == 0)
        {
           index += 1;
           difficulty += 1;

        }else{
            keep_searching = 0;
        }
    }
    return difficulty;

}
