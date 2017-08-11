#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QLabel>
#include<QMovie>
#include<QFileDialog>
#include<QDebug>
#include<QMessageBox>
#include<QFile>
#include<QBuffer>
#include<QPrintDialog>
#include<QPrinter>
#include<QPrintPreviewDialog>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->image =new QImage();
    this->qstrl=new QStringList();
    this->qlqa=ui->menurecent_file->actions();//动态创建的速度慢于静态创建
    connect(ui->actionopen,&QAction::triggered,this,&MainWindow::openFileSlot);
    connect(ui->actionsave,&QAction::triggered,this,&MainWindow::saveFileSlot);
    connect(ui->actionsave_as,&QAction::triggered,this,&MainWindow::saveAsFileSlot);
    connect(ui->actionprint,&QAction::triggered,this,&MainWindow::printSlot);
    connect(ui->actionprint_preview,&QAction::triggered,this,&MainWindow::printPreviewSlot);
    connect(ui->actionprint_setting,&QAction::triggered,this,&MainWindow::printSettingSlot);
    connect(ui->menurecent_file,&QMenu::hovered,this,&MainWindow::recentFileSlot);
    connect(ui->actionexit,&QAction::triggered,this,&MainWindow::exitSlot);
    connect(this,&MainWindow::mySignal,this,&MainWindow::activeRctFileSlot);
    readFromRecentFile(*qstrl);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFileSlot()
{
    qDebug("width:%d",ui->centralWidget->size().width());

    ui->graphicsView->resize(ui->centralWidget->width()/2,ui->centralWidget->height()-20);
    QString fileName = QFileDialog::getOpenFileName(
                this, "open image file",
                ".",
                "Image files (*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");
            image=open(fileName);
            * image = image->scaled(ui->centralWidget->width()/2,ui->centralWidget->height()-20,Qt::KeepAspectRatio);
            scene = new QGraphicsScene;
            scene->addPixmap(QPixmap::fromImage(*image));
            ui->graphicsView->setScene(scene);
            ui->graphicsView->resize(image->width() + 10, image->height() + 10);
            ui->graphicsView->show();


            \
            //        switch(image->format()){
            //        case QImage::Format_Indexed8:
            //            qDebug()<<"indexed8";
            //            break;
            //        case QImage::Format_RGB32:
            //            qDebug()<<"RGB32";
            //            break;
            //        case QImage::Format_ARGB32:
            //            qDebug()<<"ARGB32";
            //            break;
            //        case QImage::Format_ARGB32_Premultiplied:
            //            qDebug()<<"ARGB32_pre";
            //            break;
            //        case QImage::Format_RGB16:
            //            qDebug()<<"RGB16";
            //            break;
            //        case QImage::Format_RGB666:
            //            qDebug()<<"RGB666";
            //            break;
            //        default:
            //            qDebug()<<"no match";
            //            QString qs = (QString)image->format();
            //            qDebug()<<qs;
            //            break;
            //        }

    return;
}

void MainWindow::saveFileSlot()
{
    if(saveFileName.isEmpty()){
        this->saveAsFileSlot();
    }
    outputFile();
}


bool MainWindow::saveAsFileSlot()
{
    saveFileName = QFileDialog::getSaveFileName(this, "save file", QDir::currentPath());
    if(saveFileName.isEmpty()){
        return false;
    }
    outputFile();
    return true;
}

void MainWindow::printSlot()
{
    QPrinter printer;
    QString printerName = printer.printerName();
    QPrintDialog dlg(&printer, this);
    if(dlg.exec() == QDialog::Accepted){
        QSize s = QSize(printer.logicalDpiX()*2, printer.logicalDpiY()*6);
        printer.setOutputFormat(QPrinter::NativeFormat);
    }
//    }
    return;
}

void MainWindow::printPreviewSlot()
{
          QPrintPreviewDialog dlg;
          connect(&dlg, SIGNAL(paintRequested(QPrinter*)), this, SLOT(paintRequestedSlot(QPrinter*)));
          dlg.exec();
    return;
}

void MainWindow::paintRequestedSlot(QPrinter *printer)
{
    QPainter painter(printer);
    scene->render(&painter);
}
void MainWindow::printSettingSlot()
{
    return;
}

void MainWindow::recentFileSlot()
{


    return;
}

void MainWindow::exitSlot()
{
    saveRecentFile(*qstrl);
    this->close();
    return;
}

void MainWindow::activeRctFileSlot(bool x)
{
    qDebug()<<x<<endl;
    ui->menurecent_file->setEnabled(x);
}

//输出图像文件 供save和saveAs调用，成功返回true,失败返回false
bool MainWindow::outputFile()
{  QFile* file = new QFile;
    file->setFileName(saveFileName);
    bool ok =  file->open(QIODevice::WriteOnly);
    if(!ok){
        return false;
    }
    else{
        image->save(file,"JPG",100);

    }
    return true;

}
//保存最近打开的文件列表,当一开始运行程序的时候，先从文件中读取
void MainWindow::saveRecentFile( const QStringList & qsl)
{
    QFile file("./rctFile.txt");
    if(file.open(QIODevice::WriteOnly)){
        QTextStream out(&file);
        for(int i=0;i<qsl.length();i++){
            out<<qsl.at(i)<<endl;
        }
        file.close();
    }
}
void MainWindow::readFromRecentFile( QStringList & qsl)
{
      qsl.clear();
     QFile file("./rctFile.txt");
     if(file.open(QIODevice::ReadOnly)){
         QTextStream in(&file);
         QString str;
         while((str=in.readLine())!=NULL)
         {
            qsl.append(str);
         }
         file.close();
     }
}
//通过文件名打开文件终极版
QImage *  MainWindow::open(QString opFileName)
{

    QFile fl(opFileName);
    if(!fl.exists()){
        QMessageBox::about(NULL,"fail","file does not exist");
        return NULL;
    }
    else
    {
        if(!image->load(opFileName))
        {
           return NULL;
        }
        qstrl->append(opFileName);
//        ui->menurecent_file->addAction(opFileName);
           recentFileChanged();
        return image;
    }
}
//这个函数直接操作ui,可以考虑放在mainWindow中,或者传递一个MainWindow指针
void MainWindow::recentFileChanged()
{
    ui->menurecent_file->clear();
    ui->menurecent_file->addAction("clearAll");
    if(qstrl->length()>0){
        while(qstrl->length()>8){
             qDebug()<<qstrl->takeFirst()<<endl;
        }
        for(int i=0;i<qstrl->length();i++){
           ui->menurecent_file->addAction(qstrl->at(i));
        }
    }
    this->qlqa=ui->menurecent_file->actions();
    //qlqa为QList<QAction*>
    if(qlqa.length()==2){
        qDebug()<<"发射信号"<<endl;
        emit mySignal(false);
    }
    else{
        emit mySignal(true);
    }

}
