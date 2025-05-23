#include "passwordmanager.h"
#include "cryptomanager.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

void PasswordManager::addEntry(const PasswordEntry &entry) {
    entries.append(entry);
}

void PasswordManager::removeEntry(int index) {
    if (index >= 0 && index < entries.size())
        entries.removeAt(index);
}

void PasswordManager::updateEntry(int index, const PasswordEntry &entry) {
    if (index >= 0 && index < entries.size())
        entries[index] = entry;
}

const QVector<PasswordEntry> &PasswordManager::getEntries() const {
    return entries;
}

bool PasswordManager::saveToFile(const QString &filePath, const QString &masterPassword) {
    QJsonArray array;
    for (const auto &entry : entries) {
        QJsonObject obj;
        obj["service"] = entry.service;
        obj["username"] = entry.username;
        obj["password"] = entry.password;
        obj["note"] = entry.note;
        array.append(obj);
    }

    QJsonObject root;
    root["entries"] = array;

    QJsonDocument doc(root);
    QByteArray jsonData = doc.toJson();

    QByteArray encrypted = CryptoManager::encrypt(jsonData, masterPassword);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) return false;
    file.write(encrypted);
    return true;
}

bool PasswordManager::loadFromFile(const QString &filename, const QString &password) {
    QFile file(filename);
    if (!file.exists()) {
        entries.clear();
        return true;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray encryptedData = file.readAll();
    file.close();

    if (encryptedData.isEmpty()) {
        entries.clear();
        return true;
    }

    QByteArray decrypted = CryptoManager::decrypt(encryptedData, password);
    if (decrypted.isEmpty()) {
        qDebug() << "[ERROR] Decryption failed — пустой результат";
        return false;
    }

    qDebug() << "[INFO] Decrypted JSON:" << decrypted;

    QJsonDocument doc = QJsonDocument::fromJson(decrypted);
    if (!doc.isObject()) {
        return false;
    }

    QJsonObject root = doc.object();
    QJsonArray arr = root["entries"].toArray();

    entries.clear();
    for (const QJsonValue &val : arr) {
        QJsonObject obj = val.toObject();
        PasswordEntry entry;
        entry.service = obj["service"].toString();
        entry.username = obj["username"].toString();
        entry.password = obj["password"].toString();
        entry.note = obj["note"].toString();
        entries.append(entry);
    }

    return true;
}
