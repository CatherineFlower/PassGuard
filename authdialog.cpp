#include "authdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

AuthDialog::AuthDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Мастер-пароль");
    setFixedSize(300, 120);

    QLabel *label = new QLabel("Введите мастер-пароль:", this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);

    okButton = new QPushButton("ОК", this);
    cancelButton = new QPushButton("Отмена", this);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(okButton);
    btnLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(label);
    mainLayout->addWidget(passwordEdit);
    mainLayout->addLayout(btnLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

QString AuthDialog::getPassword() const {
    return passwordEdit->text();
}
