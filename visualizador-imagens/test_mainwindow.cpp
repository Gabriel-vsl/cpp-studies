#include <QTest>
#include <QImage>
#include <QColor>

#include "mainwindow.h"
#include "ui_mainwindow.h"

class TestMainWindow : public QObject
{

private:
    MainWindow *w;

private slots:

    void init()
    {
        w = new MainWindow();

        QImage img(1, 1, QImage::Format_RGB32);
        img.setPixel(0, 0, qRgb(100, 100, 100));

        ImageData data;
        data.name = "teste";
        data.original = img;
        data.processada = img;

        w->imagens.clear();
        w->imagens.push_back(data);
        w->currentIndex = 0;

        w->ui->slider_brilho->setEnabled(true);
        w->ui->slider_contraste->setEnabled(true);

        qDebug() << "Rodando teste...";
    }

    void cleanup()
    {
        delete w;
    }


    void teste_brilho()
    {
        w->ui->slider_brilho->setValue(50);
        w->ui->slider_contraste->setValue(100); // 1.0

        w->updateImage();

        QColor c(w->imagens[0].processada.pixel(0, 0));

        QCOMPARE(c.red(), 150);
        QCOMPARE(c.green(), 150);
        QCOMPARE(c.blue(), 150);
    }


    void teste_contraste()
    {
        w->ui->slider_brilho->setValue(0);
        w->ui->slider_contraste->setValue(200); // 2.0

        w->updateImage();

        QColor c(w->imagens[0].processada.pixel(0, 0));

        QCOMPARE(c.red(), 200);
        QCOMPARE(c.green(), 200);
        QCOMPARE(c.blue(), 200);
    }
};

int main(int argc, char *argv[])
{
    TestMainWindow tc;
    return QTest::qExec(&tc, argc, argv);
}