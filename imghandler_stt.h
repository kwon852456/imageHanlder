#ifndef IMGHANDLER_STT_H
#define IMGHANDLER_STT_H
#include "QFileDialog"
#include "QMainWindow"
#include "QDebug"
#include "QImage"
#include "QLabel"
#include <QPixmap>
#include <math.h>
#include "KH_02.hpp"
#include "QThread"
#include <QTcpSocket>
#include <QWaitCondition>
#include <QTcpServer>
#include <QMessageBox>
#include <QNetworkInterface>

/////////////////////////////////////////
///////    static functions    //////////
/////////////////////////////////////////


using namespace std;

extern const int (*inImg5)[512];
extern const int (*inImg2)[256];
extern const int (*inImg1)[128];

extern const int (*outImg5)[512];
extern const int (*outImg2)[256];
extern const int (*outImg1)[128];


inline QString path_dial(QMainWindow* _this){

    QString strFilter = "bitmap file (*.RAW) ;; bitmap file (*.raw) ;; All files (*.*)";
    QString strFileName = QFileDialog::getOpenFileName(_this, "Open a file", QDir::homePath() , strFilter);

    return strFileName;
}

inline QFile* file_path(QString _path ){

    QFile* file = new QFile(_path);

    return file;
}


inline QByteArray ba_file(QFile* _file){

    if( !_file->open(QIODevice::ReadOnly) ){

        qDebug() << "!_file.open(QIODevice::ReadOnly)" << __func__;

        return nullptr;

    }

    QByteArray inImg = _file->readAll();

    delete _file;

    return inImg;
}



inline QImage* img_ba(QByteArray _inImg){

    int size = sqrt(_inImg.length());
    QImage* image = new QImage( reinterpret_cast<unsigned char*> ( _inImg.data() ), size, size, QImage::Format_Grayscale8);


    return image;

}

inline QPixmap* pix_img(QImage* _img){

    QPixmap *pixmap_ = new QPixmap();

    *pixmap_ = QPixmap::fromImage(*_img);
    *pixmap_ = pixmap_->scaled(_img->width(),_img->height());

    delete _img;

    return pixmap_;

}

inline void lab_pix(QPixmap* _pix, QLabel* _label){
    _label  ->  resize       (  _pix->width(),_pix->height()  );
    _label  ->  setPixmap    (  *_pix                         );

    delete _pix;
}


inline QPixmap* pix_dial(QMainWindow* _this){
    QPixmap* buffer = pix_img( img_ba(ba_file((file_path( path_dial(_this) ))))) ;
}


inline void lab_dial(QMainWindow* _this, QLabel* _label ){

    QPixmap* buffer = pix_img(  img_ba(ba_file(file_path( path_dial(_this))))  );
    _label  ->  resize       (  buffer->width(),buffer->height()               );
    _label  ->  setPixmap    (  *buffer                                        );

}

inline QByteArray ba_qs(QString _qstring, int size = 0){

    QByteArray b_out;
    b_out.clear();
    b_out.append(_qstring.toUtf8());
    b_out.resize(size);

    qDebug() << "size of b_out :" << b_out.length();


    return b_out;
}

inline QByteArray head_enc(QString _fPath, int _mode, QString _option, int imgSize){
    qDebug() << __func__;

    QByteArray metaData = ba_qs(_fPath + ":" + QString::number(_mode) + ":" + _option , 200);

    QByteArray header;

    QDataStream headerOut(&header, QIODevice::WriteOnly);
    headerOut.setVersion(QDataStream::Qt_5_12);

    int totalLength = sizeof (int) + 200 + imgSize;

    headerOut << totalLength;

    header.append(metaData);

    qDebug() << "size of image           : " << imgSize             ;
    qDebug() << "size of image height    : " << sqrt(imgSize)       ;
    qDebug() << "size of totalLength     : " << totalLength         ;
    qDebug() << "size of metaData        : " << metaData.length   ();
    qDebug() << "size of header          : " << header.length     ();

    return header;
}


inline QByteArray createProc(QString _fPath, int _mode, QString _option = "None"){
    qDebug() << __func__;
    QByteArray b_Img    = ba_file(file_path(_fPath));
    QByteArray b_header = head_enc( _fPath, _mode,_option, b_Img.length() );

    return b_header.append(b_Img);

}





#endif // IMGHANDLER_STT_H
