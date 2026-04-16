#include <QtTest>
#include "imageprocessor.h"

class TestImageProcessor : public QObject
{
    Q_OBJECT

private slots:
    void testBrightness();
    void testContrast();
    void testClamp();
};

void TestImageProcessor::testBrightness()
{
    QImage img(1, 1, QImage::Format_RGB32);
    img.fill(QColor(100, 100, 100));

    QImage result = ImageProcessor::applyBrightnessContrast(img, 50, 1.0);

    QColor pixel(result.pixel(0, 0));

    QCOMPARE(pixel.red(), 150);
    QCOMPARE(pixel.green(), 150);
    QCOMPARE(pixel.blue(), 150);
}

void TestImageProcessor::testContrast()
{
    QImage img(1, 1, QImage::Format_RGB32);
    img.fill(QColor(100, 100, 100));

    QImage result = ImageProcessor::applyBrightnessContrast(img, 0, 2.0);

    QColor pixel(result.pixel(0, 0));

    QCOMPARE(pixel.red(), 200);
}

void TestImageProcessor::testClamp()
{
    QImage img(1, 1, QImage::Format_RGB32);
    img.fill(QColor(250, 250, 250));

    QImage result = ImageProcessor::applyBrightnessContrast(img, 50, 1.0);

    QColor pixel(result.pixel(0, 0));

    QCOMPARE(pixel.red(), 255);
}

QTEST_MAIN(TestImageProcessor)
#include "teste_imageprocessor.moc"