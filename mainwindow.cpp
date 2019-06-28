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

    dbList = new dbDialog(this);
    on_check_Log_clicked(false);
    makeConnections();


    ///////////////////////////////////
    /////////  Debug Routin  //////////
    ///////////////////////////////////

    on_btn_connect_clicked();





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

    connect(&socThread      , &QThread     :: finished        , sw     , &QObject    :: deleteLater        );
    connect(this            , &MainWindow  :: askCon          , sw     , &SocWorker  :: connectServer      );
    connect(this            , &MainWindow  :: askDisCon       , sw     , &SocWorker  :: disConnectServer   );
    connect(this            , &MainWindow  :: askTask         , sw     , &SocWorker  :: onAskTask          );
    connect(this            , &MainWindow  :: presetChecked   , sw     , &SocWorker  :: onPresetChecked    );
    connect(sw              , &SocWorker   :: sendMsg         , this   , &MainWindow :: putStatus          );
    connect(sw              , &SocWorker   :: resultReady     , this   , &MainWindow :: handleResults      );
    connect(ui->label_paint , &PaintLabel  :: roiSelect       , this   , &MainWindow :: on_RoiSelect       );
    connect(ui->label_paint , &PaintLabel  :: roiRelesed      , this   , &MainWindow :: on_RoiRelese       );
    connect(ui->label_paint , &PaintLabel  :: roiRelesed      , this   , &MainWindow :: on_RoiRelese       );
    connect(this            , &MainWindow  :: setdbList       , dbList , &dbDialog   :: onListRecv         );
    connect(dbList          , &dbDialog    :: dbItemSelected  , this   , &MainWindow :: onDbItemSelected   );
    connect(this            , &MainWindow  :: saveTempImage   , sw     , &SocWorker  :: onSaveTempImage    );

}



/////////////////////////////////////////
///////    MainWindow Slots      ////////
/////////////////////////////////////////


void MainWindow::handleResults(QByteArray _recvImg ){
    qDebug() << __func__;

    if(_recvImg[0] == '-'){

        QStringList dbDatas = QString::fromLocal8Bit(_recvImg).split("-");
        dbDatas.removeAt(0);

        emit setdbList(dbDatas);
        dbDatas.clear();
        dbList->setModal(true);
        dbList->show();


    }else{

        ui->label_img->clear();

        lab_pix(pix_img(colImg_ba(&_recvImg, imgWidth, imgHeight)), ui->label_img);
        qDebug() << "recv size : " << _recvImg.length();

    }



}

void MainWindow::putStatus(QString _text){

    ui->statusBar->showMessage(_text,10000);

}



/////////////////////////////////////////
///////  Socket Worker Class  ///////////
/////////////////////////////////////////

SocWorker::SocWorker(){

    sendSock  = new QTcpSocket(this);
    connect(sendSock,  &QTcpSocket::connected     , this, &SocWorker::onServerConnected);
    connect(sendSock,  &QTcpSocket::readyRead     , this, &SocWorker::onRecv           );

}

SocWorker::~SocWorker(){
    sendSock->close();
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

void SocWorker::disConnectServer(){

    sendSock->close();
    emit sendMsg("Disconected from server..!");

}

void SocWorker::onAskTask(QString _fPath, int _mode, bool compress, QString _option ){
    qDebug() << __func__;
    QByteArray proc;

    qDebug() <<"is_presetSaving" << is_presetSaving;

    //이전 데이터를 다시 보낼때.
    if (is_presetSaving){

        proc = createProc(_fPath, _mode, compress, _option, tempData);

    }else{

        proc = createProc(_fPath, _mode, compress, _option);

    }

    qDebug() << "send data size : " << proc.length();

    sendSock->flush();

    sendSock->write(proc);
    sendSock->waitForBytesWritten(1000);

}

void SocWorker::onRecv(){

    if(is_recvStart != true){

        char header[4];
        sendSock -> read(header, 4);

        targetLength = *reinterpret_cast<int*>(header);
        is_recvStart = true;

        recvData.clear();
    }

    recvData.push_back(sendSock->readAll());

    if(recvData.length() == targetLength){

        emit resultReady( recvData );

        preData = recvData;
        if (is_presetSaving){
            tempData = recvData;
        }

        is_recvStart = false;

        targetLength = -1;

    }

}

void SocWorker::onPresetChecked(bool checked){
    is_presetSaving = checked;
    if(!checked){
        tempData.clear();
    }
}

void SocWorker::onSaveTempImage(QString _path){
    QPixmap* pixmap = pix_img(colImg_ba(&preData,512, 512));

    QFile file(_path + "/" + "yourFile.png");
    file.open(QIODevice::WriteOnly);
    pixmap -> save(&file, "PNG");

    delete pixmap;
}



/////////////////////////////////////////
///////   PaintLaBel  Class   ///////////
/////////////////////////////////////////

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
        emit roiRelesed();
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


/////////////////////////////////////////
///////    MainWindow Events     ////////
/////////////////////////////////////////




void MainWindow::on_openInImage_triggered()
{

    current_path = path_dial(this);
    QImage* tempImg = img_path(current_path);


    imgWidth    = tempImg->width()  ;
    imgHeight   = tempImg->height() ;

    sizeOption  = "|" + QString::number(imgWidth) + "|" + QString::number(imgHeight) + "|";

    qDebug() << current_path;

    emit askTask( current_path , 0, is_compressing ,sizeOption + "|" + is_compressing ) ;

    delete tempImg;

    ui->tab_right->setCurrentIndex(1);

}

void MainWindow::on_check_Log_clicked(bool checked)
{
    if(checked){
        ui->edit_log->show();
        this->setFixedHeight(713);
    }else{
        ui->edit_log->hide();
        this->setFixedHeight(583);
    }
}



void MainWindow::on_btn_connect_clicked()
{
    emit askCon(ui->edit_host->text(),  ui->edit_port->text().toInt());
}

void MainWindow::on_btn_bright_clicked()
{
    qDebug() << ui->edit_bright->text();
    emit askTask( current_path , 1,is_compressing ,
                  sizeOption + "|" + is_compressing + "|" + ui->edit_bright->text() );
}

void MainWindow::on_btn_reverse_clicked()
{
    emit askTask( current_path , 2,is_compressing,
                  sizeOption + "|" + is_compressing);
}

void MainWindow::on_btn_bin_clicked()
{
    emit askTask( current_path , 3, is_compressing,
                  sizeOption + "|" + is_compressing+ "|" + ui->edit_Thresh->text());
}

void MainWindow::on_btn_para_clicked()
{
    emit askTask( current_path,  4, is_compressing,
                  sizeOption + "|" + is_compressing   );
}

void MainWindow::on_btn_rotate_clicked()
{
    emit askTask( current_path,  5,is_compressing ,
                  sizeOption + "|" + is_compressing + "|" + ui->edit_rotate->text() );
}


void MainWindow::on_check_compress_clicked(bool checked)
{
    if(checked){ is_compressing = true; }
    else{  is_compressing = false;  }
}

void MainWindow::on_btn_zoomOut_clicked()
{
    emit askTask( current_path,  6,is_compressing ,
                  sizeOption + "|" + is_compressing );

}

void MainWindow::on_btn_zoomIn_clicked()
{
    is_zooming = true;
    rois.clear();
}


void MainWindow::on_btn_hist_clicked()
{
    emit askTask( current_path,  8,is_compressing ,
                  sizeOption + "|" + is_compressing );
}


void MainWindow::on_btn_emboss_clicked()
{
    emit askTask( current_path,  10,is_compressing ,
                  sizeOption + "|" + is_compressing );
}

void MainWindow::on_btn_homography_clicked()
{
    is_homoing = true;
    rois.clear();
}

void MainWindow::on_RoiSelect(QVector<QPoint> _points){

    rois.push_back(_points);

    if(rois.size() >0 && is_zooming){

        QString points = qs_rois(rois);

        emit askTask( current_path,  7,is_compressing ,
                      sizeOption + "|" + is_compressing + "|" + "0" + "|Z" + points);
        is_zooming = false;

    }

    if( rois.size() > 1 && is_homoing){
       QString points = qs_rois(rois);

       emit askTask( current_path,  9,is_compressing ,
                     sizeOption + "|" + is_compressing + "|" + "0"  + "|" + points );
       is_homoing = false;

    }

}

void MainWindow::on_RoiRelese(){

    rois.clear();
    is_homoing = false;
    is_zooming = false;

}


void MainWindow::on_btn_disconnect_clicked()
{
    emit askDisCon();
}

void MainWindow::on_btn_Perspectiv_clicked()
{
    QString rot;
    QString trans;
    QString scale;

    rot   += ui->edit_rotX->text() + "-" + ui->edit_rotY->text() + "-" + ui->edit_rotZ->text() + "-";
    trans += ui->edit_moveX->text() + "-" + ui->edit_moveY->text() + "-" + ui->edit_moveZ->text() + "-";
    scale += ui->edit_scaleX->text() + "-" + ui->edit_scaleY->text();

    emit askTask( current_path,  11,is_compressing ,
                  sizeOption + "|" + is_compressing + "|" + "0"  + "|" + "P" + rot + trans + scale + "|" );

}

void MainWindow::on_btn_conv_clicked()
{

    QString convMsg = "|C";

    convMsg += "_" + QString::number((ui->radio_rgb->isChecked() ? 1 : 0)) ;
    convMsg += "_" + QString::number((ui->radio_hsv->isChecked() ? 1 : 0)) ;
    convMsg += "_" + QString::number((ui->cb_first->isChecked()  ? 1 : 0)) ;
    convMsg += "_" + QString::number((ui->cb_second->isChecked() ? 1 : 0)) ;
    convMsg += "_" + QString::number((ui->cb_third->isChecked()  ? 1 : 0)) ;

    convMsg += "_" + QString(ui->edit_00->text());
    convMsg += "_" + QString(ui->edit_01->text());
    convMsg += "_" + QString(ui->edit_02->text());

    convMsg += "_" + QString(ui->edit_10->text());
    convMsg += "_" + QString(ui->edit_11->text());
    convMsg += "_" + QString(ui->edit_12->text());

    convMsg += "_" + QString(ui->edit_20->text());
    convMsg += "_" + QString(ui->edit_21->text());
    convMsg += "_" + QString(ui->edit_22->text());

    convMsg += "|";

    emit askTask( current_path,  12,is_compressing ,
                  sizeOption + "|" + is_compressing + "|" + "0" + convMsg );

}

void MainWindow::on_radio_rgb_clicked()
{
    ui->cb_first->setText("R");
    ui->cb_second->setText("G");
    ui->cb_third->setText("B");
}

void MainWindow::on_radio_hsv_clicked()
{
    ui->cb_first->setText("H");
    ui->cb_second->setText("S");
    ui->cb_third->setText("V");
}

void MainWindow::on_cb_preset_clicked(bool checked)
{
    emit presetChecked(checked);
    is_presetSaving = checked;
}


void MainWindow::on_btn_nomalization_clicked()
{
    emit askTask( current_path,  13,is_compressing ,
                  sizeOption + "|" + is_compressing );
}

void MainWindow::on_btn_end_in_clicked()
{

    QString endInMsg = "|E";

    endInMsg += ui->edit_end->text() + "_";
    endInMsg += ui->edit_in->text();



    emit askTask( current_path,  14,is_compressing ,
                  sizeOption + "|" + is_compressing + "|" + "0" + endInMsg );

}

void MainWindow::on_actionSaveImage_triggered()
{
    emit askTask( current_path,  15,is_compressing ,
                  sizeOption + "|" + is_compressing );

}

void MainWindow::on_actionLoadImage_triggered()
{
    emit askTask( current_path,  16,is_compressing ,
                  sizeOption + "|" + is_compressing );
}

void MainWindow::onDbItemSelected(int idx){
    emit askTask( current_path,  17,is_compressing ,
                  sizeOption + "|" + is_compressing + "|" + QString::number(idx) );
    dbList->hide();
}

void MainWindow::on_actionsave_as_triggered()
{

//    QString save_path = QFileDialog::getSaveFileName(this, "save dir, ",QDir::homePath());
    QString save_path = QFileDialog::getExistingDirectory(this, "save dir", QDir::homePath());

    emit saveTempImage(save_path);
}
