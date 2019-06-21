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


    ///////////////////////////////////
    /////////  Debug Routin  //////////
    ///////////////////////////////////

    on_openInImage_triggered();

    //////// End Debug Routin ////////


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

    connect(&socThread      , &QThread   ::finished       , sw   , &QObject ::deleteLater     );
    connect(this            , &MainWindow::askCon      , sw   , &SocWorker  ::connectServer   );
    connect(sw              , &SocWorker ::sendMsg      , this , &MainWindow::putStatus       );
    connect(sw              , &SocWorker ::resultReady  , this , &MainWindow::handleResults   );
    connect(this            , &MainWindow::askTask     , sw   , &SocWorker  ::onAskTask       );
    connect(ui->label_paint , &PaintLabel::roiSelect   , this , &MainWindow ::on_RoiSelect    );

}




/////////////////////////////////////////
///////    MainWindow Slots      ////////
/////////////////////////////////////////


void MainWindow::handleResults(QByteArray _recvImg ){
    qDebug() << __func__;

    lab_pix(pix_img(img_ba(_recvImg)), ui->label_img);

    qDebug() << "end recv...";

}

void MainWindow::putStatus(QString _text){

    ui->statusBar->showMessage(_text,10000);

}


/////////////////////////////////////////
///////    MainWindow Events     ////////
/////////////////////////////////////////

void MainWindow::on_openInImage_triggered()
{
    QTime time;
    time.start();

    ///////////////////////////////////
    /////////  Debug Routin  //////////
    ///////////////////////////////////



    QImage* img     = img_path("/home/hyeok/Pictures/Images/Pet_PNG/Pet_PNG(512x512)/cat01_512.png");
    qDebug() << img->sizeInBytes();

    QGenericMatrix<512, 512, unsigned char> R;
    QGenericMatrix<512, 512, unsigned char> G;
    QGenericMatrix<512, 512, unsigned char> B;


    for ( int row = 0; row < img->height() ; ++row )
        for ( int col = 0; col < img->width(); ++col )
        {

            QColor clrCurrent( img->pixel( col, row ) );
            *(R.data() + (img->height() * row) + col) = clrCurrent.red();
            *(G.data() + (img->height() * row) + col) = clrCurrent.green();
            *(B.data() + (img->height() * row) + col) = clrCurrent.blue();

        }


    QByteArray arr_R = QByteArray::fromRawData(reinterpret_cast<char*>(R.data()), 512 * 512);
    QByteArray arr_G = QByteArray::fromRawData(reinterpret_cast<char*>(R.data()), 512 * 512);
    QByteArray arr_B = QByteArray::fromRawData(reinterpret_cast<char*>(R.data()), 512 * 512);

    lab_pix(pix_img(img_ba(arr_R)), ui->label_img);

    qDebug() << time.elapsed();


    //////// End Debug Routin ////////

//    current_path = path_dial(this);
//    emit askTask( current_path , 0);
//    ui->tab_right->setCurrentIndex(1);

}


void MainWindow::on_btn_connect_clicked()
{
    emit askCon(ui->edit_host->text(),  ui->edit_port->text().toInt());
}

void MainWindow::on_btn_bright_clicked()
{
    emit askTask( current_path , 1, ui->edit_bright->text() );
}

void MainWindow::on_btn_reverse_clicked()
{
    emit askTask( current_path , 2);
}

void MainWindow::on_btn_bin_clicked()
{
    emit askTask( current_path , 3, ui->edit_Thresh->text());
}


void MainWindow::on_btn_para_clicked()
{
    emit askTask( current_path , 4  );
}

void MainWindow::on_btn_rotate_clicked()
{
    emit askTask( current_path , 5 , ui->edit_rotate->text() );
}

void MainWindow::on_RoiSelect(QVector<QPoint> _points){
    qDebug() << "points : " << _points;
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

PaintLabel::PaintLabel( QWidget * parent)
    : QLabel( parent ) {}

void PaintLabel::mousePressEvent ( QMouseEvent* e )
{
    p = e->pos();
    if(points.size() > 4) points.clear();

    if(e -> button() == Qt::LeftButton){
        points.push_back(p);

        if(points.size() > 3){
            emit roiSelect(points);
            points.push_back(points.at(0));
        }

    }else if(e->button() == Qt::RightButton){
        points.clear();
    }

    update();

}

void PaintLabel::paintEvent( QPaintEvent * e ){

    QPainter painter(this);
    QPen paintpen(Qt::red);
    paintpen.setWidth(5);

    painter.setPen(paintpen);
//    painter.drawPoint(p);

    painter.drawPolyline(points);


}

