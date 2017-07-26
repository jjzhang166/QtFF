#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "test.h"
#include "myffmpeg.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void paintEvent(QPaintEvent *event);

private:
    QImage mImage;
    Ui::MainWindow *ui;
   // Test *t;
    MyFFmpeg ff;
public slots:
    void soltGetOneFrame(QImage img);
};

#endif // MAINWINDOW_H
