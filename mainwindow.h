#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include "passwordmanager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    void setMasterPassword(const QString &password);
    explicit MainWindow(const QString &masterPassword, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addPassword();
    void editPassword();
    void removePassword();
    void importPasswords();
    void exportPasswords();
    void showQrCode();

private:
    QString masterPassword;
    QTableWidget *table;
    PasswordManager manager;

    void setupUI();
    void refreshTable();
};
