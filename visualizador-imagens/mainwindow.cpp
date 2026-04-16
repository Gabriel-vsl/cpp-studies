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
#include <QPainter>
#include "imageprocessor.h"

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
    ui->imagem->setScaledContents(false);
    ui->imagem->setAlignment(Qt::AlignCenter);
    ui->modo->addItem("Brilho e Contraste");
    ui->modo->addItem("Pan");
    ui->modo->addItem("Zoom");
    ui->modo->addItem("Rotação");

    connect(ui->slider_brilho, &QSlider::valueChanged, this, &MainWindow::updateImage);
    connect(ui->slider_contraste, &QSlider::valueChanged, this, &MainWindow::updateImage);
    connect(ui->lista_imagens, &QListWidget::currentRowChanged, this, &MainWindow::loadImage);
    connect(ui->modo,
            QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {mode = static_cast<InteractionMode>(index);});
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

void MainWindow::loadImage(int index)
{
    if (index < 0 || index >= imagens.size())
        return;

    currentIndex = index;

    imagem = imagens[index].original;
    imagem_processada = imagens[index].processada;

    scaleFactor = 1.0;
    rotationAngle = 0.0;
    panOffset = QPoint(0, 0);

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
    if (currentIndex < 0)
        return;

    int brightness = ui->slider_brilho->value();
    double contrast = ui->slider_contraste->value() / 100.0;

    imagens[currentIndex].processada =
        ImageProcessor::applyBrightnessContrast(
            imagens[currentIndex].original,
            brightness,
            contrast
            );

    renderImage();
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

            switch (mode)
            {
                case Adjust:
            {
                    int brightness = ui->slider_brilho->value();
                    int contrast   = ui->slider_contraste->value();

                    brightness += delta.x() / 2;
                    contrast   -= delta.y() / 2;

                    brightness = std::clamp(brightness, -100, 100);
                    contrast   = std::clamp(contrast, 0, 200);

                    ui->slider_brilho->setValue(brightness);
                    ui->slider_contraste->setValue(contrast);
                    break;
            }

                case PanMode:
            {
                    panOffset += delta;
                    renderImage();
                    break;
            }

                case ZoomMode:
            {
                    scaleFactor *= std::pow(1.001, -delta.y());
                    scaleFactor = std::clamp(scaleFactor, 0.1, 5.0);
                    renderImage();
                    break;
            }

                case RotateMode:
            {
                    rotationAngle += delta.x() * 0.5;
                    renderImage();
                    break;
            }

                default:
                    break;
        }
            lastMousePos = e->pos();
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::renderImage()
{
    if (currentIndex < 0) return;

    QImage img = imagens[currentIndex].processada;
    QPixmap pix = QPixmap::fromImage(img);

    int w = ui->imagem->width();
    int h = ui->imagem->height();

    double scaleX = (double)w / pix.width();
    double scaleY = (double)h / pix.height();
    double baseScale = std::min(scaleX, scaleY);

    QTransform transform;

    transform.translate(w / 2, h / 2);

    transform.scale(baseScale, baseScale);
    transform.scale(scaleFactor, scaleFactor);
    transform.rotate(rotationAngle);
    transform.translate(-pix.width() / 2, -pix.height() / 2);
    transform.translate(panOffset.x(), panOffset.y());

    QPixmap result(w, h);
    result.fill(Qt::black);

    QPainter painter(&result);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setTransform(transform);
    painter.drawPixmap(0, 0, pix);

    ui->imagem->setPixmap(result);
}

QPixmap MainWindow::getRenderedPixmap()
{
    if (currentIndex < 0)
        return QPixmap();

    QImage img = imagens[currentIndex].processada;
    QPixmap pix = QPixmap::fromImage(img);

    int w = ui->imagem->width();
    int h = ui->imagem->height();

    double scaleX = (double)w / pix.width();
    double scaleY = (double)h / pix.height();
    double baseScale = std::min(scaleX, scaleY);

    QTransform transform;

    transform.translate(w / 2, h / 2);
    transform.scale(baseScale, baseScale);
    transform.scale(scaleFactor, scaleFactor);
    transform.rotate(rotationAngle);
    transform.translate(-pix.width() / 2, -pix.height() / 2);
    transform.translate(panOffset.x(), panOffset.y());

    QPixmap result(w, h);
    result.fill(Qt::black);

    QPainter painter(&result);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setTransform(transform);
    painter.drawPixmap(0, 0, pix);

    return result;
}

void MainWindow::on_salvar_imagem_clicked()
{
    if (currentIndex < 0)
        return;

    QString fileName = QFileDialog::getSaveFileName(
        this, "Salvar Imagem", QDir::homePath() + "/imagem_editada.png", "PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp)");

    if (fileName.isEmpty()) return;

    QPixmap pix = getRenderedPixmap();

    if (!pix.save(fileName))
    {
        QMessageBox::warning(this, "Erro", "Falha ao salvar imagem");
    }
    else
    {
        QMessageBox::information(this, "Sucesso", "Imagem salva com sucesso!");
    }
}


void MainWindow::setBrightness(int value)
{
    ui->slider_brilho->setValue(value);
}

void MainWindow::setContrast(int value)
{
    ui->slider_contraste->setValue(value);
}

