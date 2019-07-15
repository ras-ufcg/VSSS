#ifndef CONFIGURACAO_H
#define CONFIGURACAO_H

#include "opencv2/opencv.hpp"
#include <iostream>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QMouseEvent>
#include <cstdlib>
#include <windows.h>


using namespace cv;
using namespace std;

namespace Ui {
class configuracao;
}

class configuracao
{
public:
    configuracao();

private:
    Ui::configuracao *ui;
};


#endif // CONFIGURACAO_H
