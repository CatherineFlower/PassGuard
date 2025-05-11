#pragma once

#include <QDialog>

class QLineEdit;
class QPushButton;

class AuthDialog : public QDialog {
    Q_OBJECT

public:
    explicit AuthDialog(QWidget *parent = nullptr);
    QString getPassword() const;

private:
    QLineEdit *passwordEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
};
