#include "mainwindow.h"
#include "ui_mainwindow.h"



/////////////////////////////////////////
///////    Constant Values    ///////////
/////////////////////////////////////////

const int TIMEOUT = 3 * 1000; // 3 seconds


/////////////////////////////////////////
////////     MainWindow        //////////
/////////////////////////////////////////


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createThreads();
    makeConnections();

}

MainWindow::~MainWindow()
{
    delete ui;
}



/////////////////////////////////////////
///////        Threads        ///////////
/////////////////////////////////////////

void MainWindow::createThreads(){

    sw = new SocWorker();
    sw->moveToThread(&socThread);
    socThread.start();

}



/////////////////////////////////////////
///////     Connections       ///////////
/////////////////////////////////////////

void MainWindow::makeConnections(){

    connect(&socThread , &QThread::finished       , sw   , &QObject::deleteLater       );
    connect(this       , &MainWindow::askCon     , sw    , &SocWorker::connectServer   );
    connect(sw         , &SocWorker::sendMsg      , this , &MainWindow::putStatus      );
    connect(sw         , &SocWorker::resultReady  , this , &MainWindow::handleResults  );
    connect(this       , &MainWindow::askTask     , sw   , &SocWorker::onAskTask       );

}




/////////////////////////////////////////
///////   MainWindow Funtions    ////////
/////////////////////////////////////////

void MainWindow::on_openInImage_triggered()
{
    QString path = path_dial(this);
    emit askTask(path, 0);
    ui->tab_right->setCurrentIndex(1);

}


void MainWindow::on_btn_connect_clicked()
{
    emit askCon(ui->edit_host->text(),  ui->edit_port->text().toInt());
}


/// 구현 예정 //
void MainWindow::on_btn_bright_clicked()
{

}




void MainWindow::handleResults(QByteArray _recvImg ){
    qDebug() << __func__;
    lab_pix(pix_img(img_ba(_recvImg)), ui->label_img);

    qDebug() << "end recv...";

}

void MainWindow::putStatus(QString _text){

    ui->statusBar->showMessage(_text,10000);

}





/////////////////////////////////////////
///////  Socket Worker Class  ///////////
/////////////////////////////////////////

SocWorker::SocWorker(){

    sendSock = new QTcpSocket(this);
    connect(sendSock, &QTcpSocket::connected     , this, &SocWorker::onServerConnected);

}
void SocWorker::onServerConnected(){

    qDebug() << __func__;

    emit sendMsg("conection to server sucessed...!");
}


void SocWorker::connectServer(const QString _addr, const int _port){
    qDebug() << __func__;

    sendSock->connectToHost(_addr, _port);


    if(!sendSock->waitForConnected(TIMEOUT)){
        emit sendMsg("server is not responding");
    }

}



void SocWorker::onAskTask(QString _fPath, int _mode, QString _option){
    qDebug() << __func__;
    QByteArray proc = createProc(_fPath, _mode, _option);

    sendSock->write(proc);
    sendSock->waitForBytesWritten(1000);
    sendSock->waitForReadyRead(3000);

    qDebug() << "Reading: " << sendSock->bytesAvailable();

    QByteArray recvData = sendSock->readAll();
    emit resultReady(recvData);

}

