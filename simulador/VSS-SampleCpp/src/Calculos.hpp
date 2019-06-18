#ifndef CALCULOS_H
#define CALCULOS_H

#include <iostream>
#include <math.h>

struct ball_s
{
	float xb = 0;
	float yb = 0;
	float old_xb = 0;
	float old_yb = 0;
} bolinha;

struct Ponto
{
	float x, y;
};


bool area_t(float, float, float, float);

int calcula_variacao_x(float ball_pos, float _ball_pos)
{
	//
	// volatile float _ball_pos = 0;
	// DEBUGG LINES
	printf("Posição atual da bola(X): %f \n", ball_pos);
	printf("Posição antiga da bola(X): %f \n", _ball_pos);

	return (( ball_pos - _ball_pos) > 0) ? -1 : ((( ball_pos - _ball_pos) == 0) ? 0 : 1);
}

/**
 * @brief Calcula valores unitÃ¡rios representando a direÃ§Ã£o de deslocamento da bola em y.
 *
 *
 * @return int
 */
int calcula_variacao_y(float ball_pos, float _ball_pos)
{
	//
	// volatile float _ball_pos = 0;
	printf("Posição atual da bola(Y): %f \n", ball_pos);
	printf("Posição antiga da bola(Y): %f \n", _ball_pos);

	return (( ball_pos - _ball_pos) > 0) ? -1 : ((( ball_pos - _ball_pos) == 0) ? 0 : 1);
}

bool area_t(float xb, float yb, float xo, float yo){


// 	DA TRAVE SUPERIOR A BOLA
// 	Vetor diretor
Ponto vd_AC;
vd_AC.x = 150 - xb;
vd_AC.y = 85 - yb;

// 	Modulo do vetor diretor
float mvd_AC = sqrt(pow(vd_AC.x, 2) + pow(vd_AC.y, 2));


// 	DA TRAVE INFERIOR A BOLA
// 	Vetor diretor
Ponto vd_BC;
vd_BC.x = 150 - xb;
vd_BC.y = 45 - yb;

// 	Modulo do vetor diretor
float mvd_BC = sqrt(pow(vd_BC.x, 2) + pow(vd_BC.y, 2));


// 	DA BOLA PRO DELTA X
// 	Vetor diretor
Ponto vd_CD;
vd_CD.x = 20;
vd_CD.y = 0;

// Modulo do vetor diretor
float mvd_CD = 20;

// 	DA BOLA PRO DEFENSOR
// Vetor diretor
Ponto vd_CJ;
vd_CJ.x = xb - xo;
vd_CJ.y = yb - yo;

// Modulo do vetor diretor
float mvd_CJ = sqrt(pow(vd_CJ.x, 2) + pow(vd_CJ.y, 2));

// 	Cos do angulo formado por vd_AC e vd_CD
float cos_ang1;

// 	Cos do angulo formado por vd_BC e vd_CD
float cos_ang2;

// 	Cos do angulo formado por vd_CJ e vd_AC
float cos_ang3;

// 	Angulo formado porvd_AC e vd_CD
float ang1;

// 	Angulo formado por vd_BC e vd_CD
float ang2;

// 	Angulo formado por vd_CJ e vd_CD
float ang3;


cos_ang1 = (vd_CD.x * vd_AC.x) / (mvd_CD * mvd_AC);
cos_ang2 = (vd_CD.x * vd_BC.x) / (mvd_CD * mvd_BC);
cos_ang3 = (vd_CJ.x * vd_CD.x) / (mvd_CJ * mvd_CD);

ang1 = acos(cos_ang1);
ang2 = acos(cos_ang2);
ang3 = acos(cos_ang3);

if(yo < yb && xo <= xb && xo >= (xb - 20) )
{
	if(ang3 <= ang1)
		return true;
	else
		return false;
}

else if(yo > yb && xo <= xb && xo >= (xb - 20))
{
	if(ang3 <= ang2)
		return true;
	else
		return false;
}

else if (xo <= xb && xo >= (xb - 20))
	return true;

else
	return false;
};


#endif
