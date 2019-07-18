#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    FileStorage fs;
    filename = "save.xml";
    ui->setupUi(this);
    fs.open(filename, FileStorage::READ);

    timer = new QTimer(this);

    QPixmap img ("Logopreto.jpg");

    ui->label->setPixmap(img.scaled(200,400,Qt::KeepAspectRatio));

    brilho = ui->box_brilho->text().toInt();
    cap.set(CV_CAP_PROP_BRIGHTNESS, brilho);

    contraste = ui->box_contraste->text().toInt();
    cap.set(CV_CAP_PROP_CONTRAST  , contraste);

    exposicao = ui->box_exposicao->text().toInt();
    cap.set(CV_CAP_PROP_EXPOSURE  , exposicao);

    saturacao = ui->box_saturacao->text().toInt();
    cap.set(CV_CAP_PROP_SATURATION, saturacao);

    focus = ui->box_foco->text().toInt();
    cap.set(CV_CAP_PROP_FOCUS, focus);

    somar = soma(somar);

   /* qDebug() << "Brilho: " << brilho;
    qDebug() << "Exposição: " << exposicao;
    qDebug() << "Saturação: " << saturacao;
*/

    //cv::Mat image = cv::imread("c://1.jpg", 1);
    //cv::namedWindow("Imagem");
    //cv::imshow("Imagem", image);
}

//void MainWindow::Start(){







        //ui->label_brilho->setText(QString::number(brilho));
        //ui->label_expo->setText(QString::number(exposicao));
        //ui->label_contraste->setText(QString::number(contraste));
        //ui->label_saturacao->setText(QString::number(saturacao));

        //if(brilho == 255){
          //  break;
        //}
   // }
//}

int MainWindow::soma(int a) {
    a = brilho + saturacao + contraste + exposicao;
    return a;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    breakLoop=true;
    this->destroy();
    cap.~VideoCapture();
    event->accept();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_horizontalSlider_B_valueChanged( )
{
    brilho = ui->box_brilho->text().toInt();
    cap.set(CV_CAP_PROP_BRIGHTNESS, brilho);
    somar = soma(somar);

}

void MainWindow::on_horizontalSlider_C_valueChanged( )
{
    contraste = ui->box_contraste->text().toInt();
    cap.set(CV_CAP_PROP_CONTRAST  , contraste);
    somar = soma(somar);
}

void MainWindow::on_horizontalSlider_E_valueChanged( )
{
    exposicao = ui->box_exposicao->text().toInt();
    cap.set(CV_CAP_PROP_EXPOSURE  , exposicao);
    somar = soma(somar);
}

void MainWindow::on_horizontalSlider_S_valueChanged( )
{
     saturacao = ui->box_saturacao->text().toInt();
     cap.set(CV_CAP_PROP_SATURATION, saturacao);
     somar = soma(somar);
}

void MainWindow::on_pushButton_open_webcam_clicked()
{
    cap.open(0);


    if(!cap.isOpened())  // Check if we succeeded
    {
        cout << "camera is not open" << endl;
    }
    else
    {
        cout << "camera is open" << endl;
        cap.set(CV_CAP_PROP_SETTINGS, 0);
        connect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
        timer->start(20);
    }
}

void MainWindow::on_pushButton_close_webcam_clicked()
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
    cap.release();

    Mat image = Mat::zeros(frame.size(),CV_8UC3);

    qt_image = QImage((const unsigned char*) (image.data), image.cols, image.rows, QImage::Format_RGB888);

    ui->label_I->setPixmap(QPixmap::fromImage(qt_image));

    ui->label_I->resize(ui->label_I->pixmap()->size());

    cout << "camera is closed" << endl;
}

void MainWindow::update_window()
{
    cap >> frame;

    cvtColor(frame, frame, CV_BGR2RGB);

    qt_image = QImage((const unsigned char*) (frame.data), frame.cols, frame.rows, QImage::Format_RGB888);

    ui->label_I->setPixmap(QPixmap::fromImage(qt_image));

    ui->label_I->resize(ui->label_I->pixmap()->size());
}

void MainWindow::on_pushButton_save_data_clicked()
{
           FileStorage fs (filename, FileStorage :: WRITE);
           fs.open (filename, FileStorage::WRITE);
           fs << "Dados";
           fs << "{";
           fs << "Brightness"<< brilho;
           fs << "Exposure"<< exposicao;
           fs << "Saturation"<< saturacao;
           fs << "Contrast"<< contraste;
           fs << "Focus"<< focus;
           fs << "}";
           fs.release();

}

void MainWindow::on_horizontalSlider_F_valueChanged( )
{
    focus = ui->box_foco->text().toInt();
    cap.set(CV_CAP_PROP_FOCUS, focus);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        int x = event->localPos().x();
        int y = event->localPos().y();
        cout << x << "," << y << endl;
    }
}

void MainWindow::GetAndSetSaveConfigurations( )
{
    if(fs.isOpened())
    {
        FileNode data = fs["Dados"];
        cap.set(CV_CAP_PROP_EXPOSURE  , (int)(data["Exposure"]));
        cap.set(CV_CAP_PROP_BRIGHTNESS, (int)(data["Brightness"])+1);
        Sleep(30);
        cap.set(CV_CAP_PROP_BRIGHTNESS, (int)(data["Brightness"])  );
        Sleep(30);
        cap.set(CV_CAP_PROP_CONTRAST  , (int)(data["Contrast"])    );
        cap.set(CV_CAP_PROP_SATURATION, (int)(data["Saturation"])  );
        cap.set(CV_CAP_PROP_EXPOSURE  , (int)(data["Exposure"])    );
        cap.set(CV_CAP_PROP_BRIGHTNESS, (int)(data["Brightness"])+1);
        Sleep(30);
        cap.set(CV_CAP_PROP_BRIGHTNESS, (int)(data["Brightness"])  );
        Sleep(30);

        //FALTA O FOCO

        brilho = (int)(data["Brightness"]);
        exposicao = (int)(data["Exposure"]);
        saturacao = (int)(data["Saturation"]);
        contraste = (int)(data["Contrast"]);
        focus = (int)(data["Focus"]);

        ui->horizontalSlider_B->sliderMoved(data["Brightness"]);
        ui->box_brilho->valueChanged(data["Brightness"]);

        ui->horizontalSlider_S->sliderMoved(data["Saturation"]);
        ui->box_saturacao->valueChanged(data["Saturation"]);

        ui->horizontalSlider_C->sliderMoved(data["Contrast"]);
        ui->box_contraste->valueChanged(data["Contrast"]);

        ui->horizontalSlider_E->sliderMoved(data["Exposure"]);
        ui->box_exposicao->valueChanged(data["Exposure"]);

        ui->horizontalSlider_F->sliderMoved(data["Focus"]);
        ui->box_foco->valueChanged(data["Focus"]);

    }
    else
    {
        cap.set(CV_CAP_PROP_CONTRAST  , contraste);
        cap.set(CV_CAP_PROP_SATURATION, saturacao);
        cap.set(CV_CAP_PROP_EXPOSURE  , exposicao);
        cap.set(CV_CAP_PROP_BRIGHTNESS, brilho+1);
        Sleep(30);
        cap.set(CV_CAP_PROP_BRIGHTNESS, brilho);
        Sleep(30);
        //FALTA FOCO;
   }
}

void MainWindow::on_pushButton_carregar_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Abrir arquivo"),"C:", tr("XML (*.xml)"));
    if(!fileName.isNull())
    {
        setfilename(fileName);
        filename = fileName.toStdString();
    }
}

void MainWindow::setfilename(QString name)
{
    filename = name.toStdString();
    fs.release();
    fs.open(filename, FileStorage::READ);
    GetAndSetSaveConfigurations();
}
