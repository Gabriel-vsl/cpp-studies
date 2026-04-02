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

enum InteractionMode {
    Adjust = 0,
    PanMode,
    ZoomMode,
    RotateMode
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
    void renderImage();
    void on_salvar_imagem_clicked();
    QPixmap getRenderedPixmap();

private:
    Ui::MainWindow *ui;
    QImage imagem;
    QImage imagem_processada;

    bool dragging = false;
    QPoint lastMousePos;

    std::vector<ImageData> imagens;
    int currentIndex = -1;

    InteractionMode mode = Adjust;

    double scaleFactor = 1.0;
    double rotationAngle = 0.0;
    QPoint panOffset;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

};
#endif // MAINWINDOW_H
