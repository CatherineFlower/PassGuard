#include "authdialog.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QMap>

AuthDialog::AuthDialog(QWidget *parent)
    : QDialog(parent) {
    setupUI();
}

void AuthDialog::setupUI() {
    setWindowTitle("Вход / Регистрация");

    auto *layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Логин:"));
    loginEdit = new QLineEdit(this);
    layout->addWidget(loginEdit);

    layout->addWidget(new QLabel("Пароль:"));
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordEdit);

    loginButton = new QPushButton("Войти", this);
    registerButton = new QPushButton("Зарегистрироваться", this);
    deleteButton = new QPushButton("Удалить аккаунт", this);

    layout->addWidget(loginButton);
    layout->addWidget(registerButton);
    layout->addWidget(deleteButton);

    connect(loginButton, &QPushButton::clicked, this, &AuthDialog::handleLogin);
    connect(registerButton, &QPushButton::clicked, this, &AuthDialog::handleRegister);
    connect(deleteButton, &QPushButton::clicked, this, &AuthDialog::handleDelete);
}

QString AuthDialog::getLogin() const {
    return currentLogin;
}

QString AuthDialog::getPassword() const {
    return currentPassword;
}

QString AuthDialog::hashPassword(const QString &password) const {
    return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}

bool AuthDialog::loadUsers(QMap<QString, QString> &users) const {
    QFile file("users.json");
    if (!file.open(QIODevice::ReadOnly)) return false;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject obj = doc.object();
    for (const QString &key : obj.keys()) {
        users[key] = obj[key].toString();
    }

    return true;
}

bool AuthDialog::saveUser(const QString &login, const QString &hashedPassword) {
    QMap<QString, QString> users;
    loadUsers(users);

    users[login] = hashedPassword;

    QJsonObject obj;
    for (const auto &key : users.keys()) {
        obj[key] = users[key];
    }

    QFile file("users.json");
    if (!file.open(QIODevice::WriteOnly)) return false;

    file.write(QJsonDocument(obj).toJson());
    file.close();
    return true;
}

bool AuthDialog::deleteUser(const QString &login) {
    QMap<QString, QString> users;
    if (!loadUsers(users)) return false;

    if (!users.contains(login)) return false;
    users.remove(login);

    QJsonObject obj;
    for (const auto &key : users.keys()) {
        obj[key] = users[key];
    }

    QFile file("users.json");
    if (!file.open(QIODevice::WriteOnly)) return false;

    file.write(QJsonDocument(obj).toJson());
    file.close();
    return true;
}

void AuthDialog::handleLogin() {
    QMap<QString, QString> users;
    if (!loadUsers(users)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить пользователей.");
        return;
    }

    QString login = loginEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (!users.contains(login)) {
        QMessageBox::warning(this, "Ошибка", "Такой пользователь не зарегистрирован.");
        return;
    }

    if (users[login] != hashPassword(password)) {
        QMessageBox::warning(this, "Ошибка", "Неверный пароль.");
        return;
    }

    currentLogin = login;
    currentPassword = password;
    accept();
}

void AuthDialog::handleRegister() {
    QMap<QString, QString> users;
    loadUsers(users);

    QString login = loginEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Поля не должны быть пустыми.");
        return;
    }

    if (users.contains(login)) {
        QMessageBox::warning(this, "Ошибка", "Такой пользователь уже существует.");
        return;
    }

    if (!saveUser(login, hashPassword(password))) {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить пользователя.");
        return;
    }

    currentLogin = login;
    currentPassword = password;
    accept();
}

void AuthDialog::handleDelete() {
    QString login = loginEdit->text().trimmed();

    if (login.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите логин для удаления.");
        return;
    }

    if (QMessageBox::question(this, "Подтвердите", "Удалить аккаунт " + login + "?") != QMessageBox::Yes)
        return;

    if (deleteUser(login)) {
        QFile::remove("users/" + login + ".dat");
        QMessageBox::information(this, "Удалено", "Пользователь удалён.");
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось удалить пользователя.");
    }
}
