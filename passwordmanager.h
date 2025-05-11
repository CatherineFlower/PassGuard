#pragma once

#include "passwordentry.h"
#include <QVector>
#include <QString>

class PasswordManager {
public:
    bool loadFromFile(const QString &filePath, const QString &masterPassword);
    bool saveToFile(const QString &filePath, const QString &masterPassword);

    void addEntry(const PasswordEntry &entry);
    void removeEntry(int index);
    void updateEntry(int index, const PasswordEntry &entry);
    const QVector<PasswordEntry> &getEntries() const;

private:
    QVector<PasswordEntry> entries;
};
