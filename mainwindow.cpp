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


    connect(ui->btnPlay,&QToolButton::clicked, &ff,&MyFFmpeg::play);
    connect(ui->btnStop,&QToolButton::clicked, &ff,&MyFFmpeg::stop);
   // connect(ui->btnPlay,&QToolButton::clicked, this, &MainWindow::soltCtrlState);
  //  connect(ui->btnStop,&QToolButton::clicked, this, &MainWindow::soltCtrlState);


    connect(ui->btnOpen,&QToolButton::clicked, this,&MainWindow::soltOpenfile);



    connect(&ff, &MyFFmpeg::sig_GetOneFrame,this,  &MainWindow::soltGetOneFrame);

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
    update(); //让窗体失效

}

void MainWindow::soltOpenfile()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this,"open video","","mp4 files(*.mp4);all file(*.*)");
    this->setWindowTitle(filename);
    if(!filename.isNull())
    {

        cout << filename.toStdString() << endl;
        ff.stop(); //停止当前的播放

        ff.setFilename(filename.toStdString());

        ff.play();
    }
}

void MainWindow::soltCtrlState()
{
    if(ui->btnPlay == dynamic_cast<QToolButton*>(sender()))
    {

    }
    else if(ui->btnStop == dynamic_cast<QToolButton*>(sender()))
    {
        QPainter painter(this);
        painter.setBrush(Qt::black);
        painter.drawRect(0,0,this->width(),this->height());
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
