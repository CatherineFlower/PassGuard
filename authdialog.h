#pragma once

#include <QDialog>
#include <QString>

class QLineEdit;
class QPushButton;

class AuthDialog : public QDialog {
    Q_OBJECT

public:
    explicit AuthDialog(QWidget *parent = nullptr);
    QString getLogin() const;
    QString getPassword() const;

private slots:
    void handleLogin();
    void handleRegister();
    void handleDelete();

private:
    QLineEdit *loginEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QPushButton *deleteButton;

    QString currentLogin;
    QString currentPassword;

    void setupUI();
    QString hashPassword(const QString &password) const;
    bool loadUsers(QMap<QString, QString> &users) const;
    bool saveUser(const QString &login, const QString &hashedPassword);
    bool deleteUser(const QString &login);
};
