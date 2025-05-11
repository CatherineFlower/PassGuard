#pragma once

#include <QString>
#include <QByteArray>

class CryptoManager {
public:
    static QByteArray encrypt(const QByteArray &data, const QString &password);
    static QByteArray decrypt(const QByteArray &data, const QString &password);
};
