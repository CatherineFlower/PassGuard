#include "qrdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QImage>
#include "qrcodegen.hpp"

using qrcodegen::QrCode;

QRDialog::QRDialog(const QString &textToEncode, QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("QR-код пароля");
    setFixedSize(300, 300);

    qrLabel = new QLabel(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(qrLabel);

    // Генерация QR-кода
    QrCode qr = QrCode::encodeText(textToEncode.toUtf8().constData(), QrCode::Ecc::LOW);
    int size = qr.getSize();

    QImage image(size, size, QImage::Format_RGB888);
    for (int y = 0; y < size; ++y)
        for (int x = 0; x < size; ++x)
            image.setPixel(x, y, qr.getModule(x, y) ? qRgb(0, 0, 0) : qRgb(255, 255, 255));

    QImage scaled = image.scaled(256, 256, Qt::KeepAspectRatio);
    qrLabel->setPixmap(QPixmap::fromImage(scaled));
}
