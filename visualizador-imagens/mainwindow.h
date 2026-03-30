#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_carregar_imagem_clicked();
    void updateImage();

private:
    Ui::MainWindow *ui;
    QImage originalImage;
    QImage imagem;
    QImage processedImage;

};
#endif // MAINWINDOW_H
