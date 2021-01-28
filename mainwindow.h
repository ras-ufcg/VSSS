#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Qdebug>
#include <iostream>
#include <QPixmap>
#include <QtWidgets>
#include <QTimer>
#include <string>
#include <QFileDialog>
#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/core/types.hpp"


using namespace cv;
using namespace std;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    string filename;
    FileStorage fs;
    ofstream outSaveData;
    int contraste = 0;
    int exposicao = 0;
    int saturacao = 0;
    int brilho = 0;
    int focus = 0;
    int somar = 0;
    bool breakLoop= false;
    void GetAndSetSaveConfigurations();
    void setfilename(QString);
    //void Start();
    int soma(int a);
    explicit MainWindow(QWidget *parent = 0);
    void closeEvent (QCloseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    ~MainWindow();

private slots:

    void on_horizontalSlider_B_valueChanged();

    void on_horizontalSlider_C_valueChanged();

    void on_horizontalSlider_E_valueChanged();

    void on_horizontalSlider_F_valueChanged();

    void on_horizontalSlider_S_valueChanged();

    void on_pushButton_open_webcam_clicked();

    void on_pushButton_close_webcam_clicked();

    void update_window();

    void on_pushButton_save_data_clicked();

    void on_pushButton_carregar_clicked();

private:
    Ui::MainWindow *ui;

    QTimer *timer;
    cv::VideoCapture cap;

    cv::Mat frame;
    QImage qt_image;
};

#endif // MAINWINDOW_H
