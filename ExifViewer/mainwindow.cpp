#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QPixmap>
#include <QDebug>
#include <QShortcut>
#include <QPainter>

#include <stdio.h>
#include "exif.h"


QStringList filePath;
int index;
int total;
int angle;
bool loaded=false;
easyexif::EXIFInfo result;


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget::setWindowTitle ("ExifViewer");
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_O), this, SLOT(on_pushButton_3_clicked()));
    new QShortcut(QKeySequence(Qt::Key_R), this, SLOT(on_pushButton_4_clicked()));
    new QShortcut(QKeySequence(Qt::Key_A), this, SLOT(on_pushButton_clicked()));
    new QShortcut(QKeySequence(Qt::Key_D), this, SLOT(on_pushButton_2_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_3_clicked()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::DontUseNativeDialog, true);
    dialog.setOption(QFileDialog::ShowDirsOnly, false);
    dialog.exec();
    QString path = dialog.directory().absolutePath();

    total=-1;
    QDir directory(path);
    QStringList images = directory.entryList(QStringList() << "*.jpg" << "*.JPG",QDir::Files);
    foreach(QString filename, images) {
        filePath.append(path+"/"+filename);
        total+=1;
    }
    qInfo()<<filePath;

    if(total!=-1){
        loaded=true;
        index=0;
        QPixmap pix(filePath[index]);
        ui->label_pic->setPixmap(pix.scaled(512,512,Qt::KeepAspectRatio));
    }
    ui->listWidget->clear();
    MainWindow::getExif();
}

void MainWindow::on_pushButton_2_clicked()
{
    if(loaded){
        if(index==total)
            index=0;
        else
            index+=1;
        QPixmap pix(filePath[index]);
        ui->label_pic->setPixmap(pix.scaled(512,512,Qt::KeepAspectRatio));
        ui->listWidget->clear();
        MainWindow::getExif();
    }
}

void MainWindow::on_pushButton_clicked()
{
    if (loaded){
        if(index==0)
            index=total;
        else
            index-=1;
        QPixmap pix(filePath[index]);
        ui->label_pic->setPixmap(pix.scaled(512,512,Qt::KeepAspectRatio));
        ui->listWidget->clear();
        MainWindow::getExif();
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    if(loaded){
        QPixmap ship(filePath[index]);
        QPixmap rotate(ship.size());
        QPainter p(&rotate);
        p.setRenderHint(QPainter::Antialiasing);
        p.setRenderHint(QPainter::SmoothPixmapTransform);
        p.translate(rotate.size().width() /2, rotate.size().height()/2);
        angle=(angle+90)%360;
        p.rotate(angle);
        p.translate(-rotate.size().width() /2, -rotate.size().height()/2);
        p.drawPixmap(0,0,ship);
        p.end();
        ui->label_pic->setPixmap(rotate.scaled(512,512,Qt::KeepAspectRatio));
    }
}


int MainWindow::getExif() {
  // Read the JPEG file into a buffer
  QByteArray ba = filePath[index].toLocal8Bit();
  char *path = ba.data();
  FILE *fp = fopen(path, "rb");
  if (!fp) {
    printf("Can't open file.\n");
    return -1;
  }
  fseek(fp, 0, SEEK_END);
  unsigned long fsize = ftell(fp);
  rewind(fp);
  unsigned char *buf = new unsigned char[fsize];
  if (fread(buf, 1, fsize, fp) != fsize) {
    printf("Can't read file.\n");
    delete[] buf;
    return -2;
  }
  fclose(fp);

  // Parse EXIF
  int code = result.parseFrom(buf, fsize);
  delete[] buf;
  if (code) {
    printf("Error parsing EXIF: code %d\n", code);
    return -3;
  }

  // Dump EXIF information

  QString str=result.Make.c_str();
  if(str!="")
    ui->listWidget->addItem("Camera make: "+str);

  str=result.Model.c_str();
  if(str!="")
    ui->listWidget->addItem("Camera model: "+str);

  str=result.Software.c_str();
  if(str!="")
    ui->listWidget->addItem("Software: "+str);

  int i=result.BitsPerSample;
  if(i!=0)
    ui->listWidget->addItem("Bits per sample: "+QString::number(i));

  i=result.ImageWidth;
  if(i!=0)
    ui->listWidget->addItem("Image width: "+QString::number(i));

  i=result.ImageHeight;
  if(i!=0)
    ui->listWidget->addItem("Image height: "+QString::number(i));

  str=result.ImageDescription.c_str();
  if(str!="")
    ui->listWidget->addItem("Image description: "+str);

  i=result.Orientation;
  if(i!=0)
    ui->listWidget->addItem("Image orientation: "+QString::number(i));

  str=result.Copyright.c_str();
  if(str!="")
    ui->listWidget->addItem("Image copyright: "+str);

  str=result.DateTime.c_str();
  if(str!="")
    ui->listWidget->addItem("Image date/time: "+str);

  str=result.DateTimeOriginal.c_str();
  if(str!="")
    ui->listWidget->addItem("Original date/time: "+str);

  str=result.DateTimeDigitized.c_str();
  if(str!="")
    ui->listWidget->addItem("Digitalize date/time: "+str);

  str=result.SubSecTimeOriginal.c_str();
  if(str!="")
    ui->listWidget->addItem("Subsecond time: "+str);

  i=result.ExposureTime;
  if(i!=0)
    ui->listWidget->addItem("Exposure time: 1/"+QString::number(i)+" s");

  i=result.FNumber;
  if(i!=0)
    ui->listWidget->addItem("F-stop : f/"+QString::number(i)+".1f");

  i=result.ExposureProgram;
  if(i!=0)
    ui->listWidget->addItem("Exposure program: "+QString::number(i));

  i=result.ISOSpeedRatings;
  if(i!=0)
    ui->listWidget->addItem("ISO speed: "+QString::number(i));

  i=result.SubjectDistance;
  if(i!=0)
    ui->listWidget->addItem("Subject distance: "+QString::number(i)+" m");

  i=result.ExposureBiasValue;
  if(i!=0)
    ui->listWidget->addItem("Exposure bias: "+QString::number(i)+" EV");

  i=result.Flash;
  if(i!=0)
    ui->listWidget->addItem("Flash used?: "+QString::number(i));

  i=result.FlashReturnedLight;
  if(i!=0)
    ui->listWidget->addItem("Flash returned light: "+QString::number(i));

  i=result.FlashMode;
  if(i!=0)
    ui->listWidget->addItem("Flash mode: "+QString::number(i));

  i=result.MeteringMode;
  if(i!=0)
    ui->listWidget->addItem("Metering mode: "+QString::number(i));

  i=result.FocalLength;
  if(i!=0)
    ui->listWidget->addItem("Lens focal length: "+QString::number(i)+" mm");

  i=result.FocalLengthIn35mm;
  if(i!=0)
    ui->listWidget->addItem("35mm focal length: "+QString::number(i)+" mm");

  if(result.GeoLocation.Latitude != 0 && result.GeoLocation.Longitude != 0)
    ui->listWidget->addItem("Google Map Position");

  i=result.GeoLocation.Altitude;
  if(i!=0)
    ui->listWidget->addItem("GPS Altitude: "+QString::number(i)+" m");

  i=result.GeoLocation.DOP;
  if(i!=0)
    ui->listWidget->addItem("GPS Precision (DOP): "+QString::number(i));

  i=result.LensInfo.FocalLengthMin;
  if(i!=0)
    ui->listWidget->addItem("Lens min focal length: "+QString::number(i)+" mm");

  i=result.LensInfo.FocalLengthMax;
  if(i!=0)
    ui->listWidget->addItem("Lens max focal length: "+QString::number(i)+" mm");

  i=result.LensInfo.FStopMin;
  if(i!=0)
    ui->listWidget->addItem("Lens f-stop min: f/"+QString::number(i)+".1f");

  i=result.LensInfo.FStopMax;
  if(i!=0)
    ui->listWidget->addItem("Lens f-stop max: f/"+QString::number(i)+".1f");

  str=result.LensInfo.Make.c_str();
  if(str!="")
    ui->listWidget->addItem("Lens make: "+str);

  str=result.LensInfo.Model.c_str();
  if(str!="")
    ui->listWidget->addItem("Lens model: "+str);

  i=result.LensInfo.FocalPlaneXResolution;
  if(i!=0)
    ui->listWidget->addItem("Focal plane XRes: "+QString::number(i));

  i=result.LensInfo.FocalPlaneYResolution;
  if(i!=0)
    ui->listWidget->addItem("Focal plane YRes: "+QString::number(i));

  return 0;
}
void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if(item->text()=="Google Map Position"){
            QString position="start https://www.google.com/maps/search/?api=1^&query="+QString::number(result.GeoLocation.Latitude)+","+QString::number(result.GeoLocation.Longitude);
            system(position.toStdString().c_str());
    }
}
