#include <QtTest/QTest>
#include "mainwindow.h"

class TestMainWindow : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testBrightnessContrast();
    void testZoom();
    void testRotation();
    void testRenderNotEmpty();
};

MainWindow *window;

void TestMainWindow::initTestCase()
{
    window = new MainWindow();
}

