#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include "passwordmanager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    // void setMasterPassword(const QString &password);
    explicit MainWindow(const QString &login, const QString &password, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addPassword();
    void editPassword();
    void removePassword();
    void importPasswords();
    void exportPasswords();
    void showQrCode();
    void logout();
    void deleteAccount();
    void quitApp();

private:
    QString currentLogin;
    QString masterPassword;
    QTableWidget *table;
    PasswordManager manager;

    void setupUI();
    void createSettingsMenu();
    void refreshTable();
};
