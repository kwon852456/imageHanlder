#ifndef IMGHANDLER_STT_H
#define IMGHANDLER_STT_H
#include "QFileDialog"
#include "QMainWindow"
#include "QDebug"
#include "QImage"
#include "QLabel"
#include <QPixmap>
#include <math.h>


using namespace std;

extern int (*inImg5)[512];
extern int (*inImg2)[256];
extern int (*inImg1)[128];

extern int (*outImg5)[512];
extern int (*outImg2)[256];
extern int (*outImg1)[128];

void loadImg(int _size, QByteArray b_img){

}

QFile* path_dial(QMainWindow* _this ){

    QString strFilter = "bitmap file (*.raw) ;; text file (*.txt) ;; All files (*.*)";
    QString strFileName = QFileDialog::getOpenFileName(_this, "Open a file", QDir::homePath() , strFilter);


    QFile* file = new QFile(strFileName);

    return file;
}




QImage* img_file(QFile* _file){


    if( !_file->open(QIODevice::ReadOnly) ){

        qDebug() << __func__;
        qDebug() << "!_file.open(QIODevice::ReadOnly)";
        return nullptr;

    }

    QByteArray inImg = _file->readAll();

    int size = sqrt(inImg.length());
    QImage* image = new QImage( reinterpret_cast<unsigned char*> ( inImg.data() ), size, size, QImage::Format_Grayscale8);


    return image;

}

QPixmap* pix_img(QImage* _img){

    QPixmap *pixmap_ = new QPixmap();

    *pixmap_ = QPixmap::fromImage(*_img);
    *pixmap_ = pixmap_->scaled(_img->width(),_img->height());

    return pixmap_;

}


QPixmap* pix_dial(QMainWindow* _this){
    QPixmap* buffer = pix_img( img_file((path_dial(_this)))) ;
}

void lab_dial(QMainWindow* _this, QLabel* _label ){

    QPixmap* buffer = pix_img( img_file((path_dial(_this)))) ;
    _label  ->  resize     (  buffer->width(),buffer->height()  );
    _label  ->  setPixmap  (  *buffer                           );

}






#endif // IMGHANDLER_STT_H
