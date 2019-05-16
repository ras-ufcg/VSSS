//#-------------------------------------------------
//#
//# Project created by QtCreator 2016-05-30T18:42:28
//# Author: Nauroze Hoath
//#
//#-------------------------------------------------


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtWidgets>

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Default object of lime color for test purposes, uncomment when needed.
    //objects.push_back(setUpObject("lime", 32,72, 105, 205, 18, 167));

    //open capture object at location zero (default location for webcam)
    capture.open(0);
    //set height and width of capture frame
    capture.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);
    //capture.set(CV_CAP_PROP_AUTOFOCUS, 0);

    namedWindow(imageWindow);
    namedWindow(hsvWindow);
    namedWindow(thresholdWindow);

    //To avoid windows stacking over each other,
    //these adjustmets could differ with screen sizes.
    moveWindow(imageWindow,0,0);
    moveWindow(hsvWindow, 600,0);
    moveWindow(thresholdWindow, 600,0);

    //novo objeto de Dialog
    d = new Dialog();



}

void MainWindow::closeEvent(QCloseEvent *event)
{
    breakLoop=true;
    destroyAllWindows();
    capture.~VideoCapture();
    this->destroy();
    event->accept();
}

MainWindow::~MainWindow()
{

    delete ui;
    delete d;
}
// calcula(MainWindow w

void MainWindow::Start(){

    while (true){
        //Dialog *d = new Dialog d;
        if(breakLoop==true){
            return;
        }
        H_MIN = ui->hminBox->text().toInt();
        H_MAX = ui->hmaxBox->text().toInt();
        S_MIN = ui->sminBox->text().toInt();
        S_MAX = ui->smaxBox->text().toInt();
        V_MIN = ui->vminBox->text().toInt();
        V_MAX = ui->vmaxBox->text().toInt();


        capture.read(cameraFeed);
        testObject.name=" ";
        cvtColor(cameraFeed,HSV, COLOR_BGR2HSV);
        inRange(HSV, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), testObject.threshold);
        morphObject(testObject.threshold);
        trackObject(x, y, testObject, cameraFeed);


        for (unsigned int i = 0; i<objects.size(); i++) {

            inRange(HSV, Scalar(objects[i].H_MIN, objects[i].S_MIN, objects[i].V_MIN), Scalar(objects[i].H_MAX, objects[i].S_MAX, objects[i].V_MAX), objects[i].threshold);
            morphObject(objects[i].threshold);
            trackObject(x, y, objects[i], cameraFeed);

            if(x <= 320)
                flag = true;
            else
                flag = false;

            if (flag == true && aux == false){
               d->updateLED(QString("L%1")); //orienta LED a partir da coordenada X a ligar
               aux = true;
            }
            if (flag == false && aux == true){
                d->updateLED(QString("D%1")); //orienta LED a partir da coordenada X a desligar
                aux = false;
            }
        }


        imshow(imageWindow, cameraFeed);
        imshow(hsvWindow, HSV);
        imshow(thresholdWindow,testObject.threshold);


        //delay 30ms so that screen can refresh.
        waitKey(30);
    }


}

string MainWindow::numberToString(int number){


    std::stringstream ss;
    ss << number;
    return ss.str();
}

void MainWindow::drawObject(int x, int y, Mat &frame, item tempItem){
    //qDebug() << "entrei aqui quarto";

    //rectangle(frame,Point(x-(sideLength/2),y-(sideLength/2)), Point(x+(sideLength/2),y+(sideLength/2)),Scalar(0,255,0),2);
    circle(frame,Point(x, y), sideLength/2,Scalar(0,255,0),2);



    //drawContours(frame, contours, -1, (0,255,0), 3)
    //putText(frame, numberToString(x) + "," + numberToString(y), Point(x, y + 30), 1, 1, Scalar(0,72, 255), 1);
    putText(frame, tempItem.name , Point(x, y + 50), 2, 1, Scalar(0,72, 255), 1);
}
void MainWindow::morphObject(Mat &thresh){

    //qDebug() << "entrei aqui terceiro";

    //create structuring element that will be used to "dilate" and "erode" image.
    //the element chosen here is a 3px by 3px rectangle
    Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));

    //dilate with larger element so make sure object is nicely visible
    Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));

    erode(thresh, thresh, erodeElement);
    erode(thresh, thresh, erodeElement);
    dilate(thresh, thresh, dilateElement);
    dilate(thresh, thresh, dilateElement);



}
void MainWindow::trackObject(int &x, int &y, item tempItem, Mat &cameraFeed){
    //qDebug() << "entrei aqui primeiro";
    Mat temp;
    tempItem.threshold.copyTo(temp);

    //these two vectors needed for output of findContours
    vector<vector<Point>> contours;
    qDebug() << x << "," << y;
    vector<Vec4i> hierarchy;

    //find contours of filtered image using openCV findContours function
    findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0,0));

    //use moments method to find our filtered object
    double refArea = 0;
    bool objectFound = false;


    if (hierarchy.size() > 0) {
        int numObjects = hierarchy.size();
        //if number of objects greater than MAX_OBJECTS we have a noisy filter
       if (numObjects<MAX_OBJECTS){
            for (int index = 0; index >= 0; index = hierarchy[index][0]) {

                Moments moment = moments(contours[index]);
                area = moment.m00;
                sideLength= sqrt(area);

                //if the area is less than 25 px by 25px then it is probably just noise
                //if the area is the same as the 1/2 of the image size, probably just a bad filter
                //we only want the object with the largest area so we safe a reference area each
                //iteration and compare it to the area in the next iteration.
               if (area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA) { // && area>refArea){
                    x = moment.m10 / area;
                    y = moment.m01 / area;
                    drawObject(x, y, cameraFeed, tempItem);
                    objectFound = true;
                    refArea = area;
                }
                else objectFound = false;


            }
            //let user know you found an object
           // if (objectFound == true){
                //draw object location on screen
             //   drawObject(x, y, cameraFeed, tempItem);


            //}

      }
   }
}


MainWindow::item MainWindow::setUpObject(string name, int hmin, int hmax, int smin, int smax, int vmin, int vmax){

    item temp;
    temp.name = name;
    //temp.x = x;
    //temp.y = y;
    temp.H_MIN = hmin;
    temp.H_MAX = hmax;
    temp.S_MIN = smin;
    temp.S_MAX = smax;
    temp.V_MIN = vmin;
    temp.V_MAX = vmax;

    return temp;

}

void MainWindow::on_addButton_clicked()
{
    objects.push_back(setUpObject(ui->nameLineEdit->text().toStdString(),H_MIN,H_MAX,S_MIN,S_MAX,V_MIN,V_MAX));
    ui->nameLineEdit->clear();

    ui->hminBox->setValue(0);
    ui->hmaxBox->setValue(255);
    ui->sminBox->setValue(0);
    ui->smaxBox->setValue(255);
    ui->vminBox->setValue(0);
    ui->vmaxBox->setValue(255);
}

void MainWindow::on_defaultButton_clicked()
{
    ui->hminBox->setValue(0);
    ui->hmaxBox->setValue(255);
    ui->sminBox->setValue(0);
    ui->smaxBox->setValue(255);
    ui->vminBox->setValue(0);
    ui->vmaxBox->setValue(255);

}
