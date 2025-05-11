#pragma once

#include <QDialog>
#include <QString>

class QLabel;

class QRDialog : public QDialog {
    Q_OBJECT

public:
    explicit QRDialog(const QString &textToEncode, QWidget *parent = nullptr);

private:
    QLabel *qrLabel;
};
