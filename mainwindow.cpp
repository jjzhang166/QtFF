#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "videoplayer.h"
#include <QPainter>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
   // ui->setupUi(this);




  // ff.set

    //connect(&ff,SIGNAL(sig_GetOneFrame(QImage img)),this,SLOT(soltGetOneFrame(QImage img)));

    connect(&ff, &MyFFmpeg::sig_GetOneFrame,this,  &MainWindow::soltGetOneFrame);

   ff.start();

}


MainWindow::~MainWindow()
{
    //delete ui;
}

void MainWindow::soltGetOneFrame(QImage img)
{

    //cout<<"a img"<<endl;
        mImage = img;
        update();

}

void MainWindow::paintEvent(QPaintEvent *event)
{
     QPainter painter(this);
     painter.setBrush(Qt::black);
     painter.drawRect(0,0,this->width(),this->height());
     if(mImage.size().width() <= 0)
         return;
     QImage img = mImage.scaled(this->size(),Qt::KeepAspectRatio);
     int x = this->width() - img.width();
     int y = this->height() - img.height();
     x/=2;
     y/=2;
     painter.drawImage(QPoint(x,y),img);
}
