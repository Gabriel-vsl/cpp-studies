#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct ImageData {
    QString name;
    QImage original;
    QImage processada;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_carregar_imagem_clicked();
    void updateImage();
    void loadImage(int index);

private:
    Ui::MainWindow *ui;
    QImage imagem;
    QImage imagem_processada;

    bool dragging = false;
    QPoint lastMousePos;

    std::vector<ImageData> imagens;
    int currentIndex = -1;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

};
#endif // MAINWINDOW_H
