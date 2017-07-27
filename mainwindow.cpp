#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "videoplayer.h"
#include <QPainter>
#include <QThread>
#include<QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ff.set
    //connect(&ff,SIGNAL(sig_GetOneFrame(QImage img)),this,SLOT(soltGetOneFrame(QImage img)));

    connect(ui->btnPlay,&QToolButton::clicked, &ff,&MyFFmpeg::play);
    connect(ui->btnStop,&QToolButton::clicked, &ff,&MyFFmpeg::stop);

    connect(ui->btnOpen,&QToolButton::clicked, this,&MainWindow::openfile);

   connect(&ff, &MyFFmpeg::sig_GetOneFrame,this,  &MainWindow::soltGetOneFrame);

   //ff.start();

}


MainWindow::~MainWindow()
{
    ff.terminate();
    ff.wait();
    delete ui;
}

void MainWindow::soltGetOneFrame(QImage img)
{

    //cout<<"a img"<<endl;
        mImage = img;
        update();

}

void MainWindow::openfile()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this,"open video","","mp4 files(*.mp4);all file(*.*)");
    if(!filename.isNull())
    {

        cout << filename.toStdString() << endl;
        ff.stop();

        ff.setFilename(filename.toStdString());
       // while(ff.thread()->isFinished());
        //ff.play();
    }

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
