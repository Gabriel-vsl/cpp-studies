#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QImage>
#include <QVector>
#include <QQueue>
#include <QFileInfo>
#include <QColor>
#include <algorithm>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->slider_brilho->setRange(-100, 100);
    ui->slider_contraste->setRange(0, 200);
    ui->slider_brilho->setEnabled(false);
    ui->slider_contraste->setEnabled(false);
    ui->imagem->setMouseTracking(true);
    ui->imagem->installEventFilter(this);

    connect(ui->slider_brilho, &QSlider::valueChanged, this, &MainWindow::updateImage);
    connect(ui->slider_contraste, &QSlider::valueChanged, this, &MainWindow::updateImage);
    connect(ui->lista_imagens, &QListWidget::currentRowChanged, this, &MainWindow::loadImage);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_carregar_imagem_clicked()
{
    QStringList arquivos = QFileDialog::getOpenFileNames(this, tr("Abrir Arquivo"), QDir::homePath(), tr("Imagens (*.png *.jpg *.jpeg *.bmp)"));

    for (const QString &file_name : std::as_const(arquivos))
    {
        QImage img(file_name);

        if (img.isNull())
            continue;

        QFileInfo fileInfo(file_name);

        ImageData data;
        data.name = fileInfo.fileName();

        data.original = img;
        data.processada = img;

        imagens.push_back(data);
        ui->lista_imagens->addItem(data.name);
    }

    if (!imagens.empty() && currentIndex == -1)
    {
        ui->lista_imagens->setCurrentRow(0);
    }
}
/*
    if (!imagem.isEmpty && currentIndex == -1())
    {
        QMessageBox::information(this, "...", file_name);

        imagem = QImage(file_name);
        QPixmap pix = QPixmap::fromImage(imagem);

        if (imagem.isNull())
        {
            QMessageBox::warning(this, "Erro", "Falha ao carregar imagem");
            return;
        }


        QFileInfo fileInfo(file_name);
        QString short_name = fileInfo.fileName();
        ui->valor_nome_imagem->setText(short_name);


        ui->slider_brilho->setEnabled(true);
        ui->slider_contraste->setEnabled(true);
        ui->slider_brilho->setValue(0);
        ui->slider_contraste->setValue(100);


        int w = ui->imagem->width();
        int h = ui->imagem->height();
        ui->imagem->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));


        unsigned int cols = imagem.width();
        unsigned int rows = imagem.height();
        unsigned int numBlackPixels = 0;
        QVector<QVector<int>> imgArray(rows,QVector<int>(cols, 0));

        for (unsigned int i = 0; i < rows; i++)
        {
            for (unsigned int j = 0; j < cols; j++)
            {
                QColor clrCurrent(imagem.pixel( j, i ));
                int r = clrCurrent.red();
                int g = clrCurrent.green();
                int b = clrCurrent.blue();
                int a = clrCurrent.alpha();

                if (r+g+b < 20 && a > 240)
                {
                    imgArray[i][j] = 1;
                    numBlackPixels+=1;
                }
            }
        }

        ui->valor_dimensoes_imagem->setText(QString::fromStdString("W: " + std::to_string(cols) + "  H: " + std::to_string(rows)));
    }
}
*/

void MainWindow::loadImage(int index)
{
    if (index < 0 || index >= imagens.size())
        return;

    currentIndex = index;

    imagem = imagens[index].original;
    imagem_processada = imagens[index].processada;

    ui->slider_brilho->setEnabled(true);
    ui->slider_contraste->setEnabled(true);

    ui->slider_brilho->setValue(0);
    ui->slider_contraste->setValue(100);

    int w = ui->imagem->width();
    int h = ui->imagem->height();

    ui->imagem->setPixmap(
        QPixmap::fromImage(imagem).scaled(w, h, Qt::KeepAspectRatio)
        );

    int cols = imagem.width();
    int rows = imagem.height();

    ui->valor_dimensoes_imagem->setText(
        QString("W: %1  H: %2").arg(cols).arg(rows)
        );
}

void MainWindow::updateImage()
{
    //qDebug() << ui->slider_brilho->value();
    //qDebug() << ui->slider_contraste->value();

    if (currentIndex < 0)
        return;

    int brightness = ui->slider_brilho->value();
    double contrast = ui->slider_contraste->value() / 100.0;

    QImage img = imagens[currentIndex].original.convertToFormat(QImage::Format_ARGB32);

    for (int y = 0; y < img.height(); y++)
    {
        for (int x = 0; x < img.width(); x++)
        {
            QColor c = QColor::fromRgb(img.pixel(x, y));

            int r = std::clamp(int(contrast * c.red() + brightness), 0, 255);
            int g = std::clamp(int(contrast * c.green() + brightness), 0, 255);
            int b = std::clamp(int(contrast * c.blue() + brightness), 0, 255);

            img.setPixel(x, y, qRgb(r, g, b));
        }
    }

    imagens[currentIndex].processada = img;

    int w = ui->imagem->width();
    int h = ui->imagem->height();

    ui->imagem->setPixmap(
        QPixmap::fromImage(img).scaled(w, h, Qt::KeepAspectRatio));

    //qDebug() << "Imagem atualizada";
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->imagem)
    {

        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *e = static_cast<QMouseEvent*>(event);
            dragging = true;
            lastMousePos = e->pos();
            return true;
        }

        if (event->type() == QEvent::MouseButtonRelease)
        {
            dragging = false;
            return true;
        }

        if (event->type() == QEvent::MouseMove && dragging)
        {
            QMouseEvent *e = static_cast<QMouseEvent*>(event);

            QPoint delta = e->pos() - lastMousePos;

            int brightness = ui->slider_brilho->value();
            int contrast   = ui->slider_contraste->value();

            brightness += delta.x() / 2;

            contrast -= delta.y() / 2;

            brightness = std::clamp(brightness, -100, 100);
            contrast   = std::clamp(contrast, 0, 200);

            ui->slider_brilho->setValue(brightness);
            ui->slider_contraste->setValue(contrast);

            lastMousePos = e->pos();

            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}


