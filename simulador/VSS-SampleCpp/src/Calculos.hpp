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

struct Pontos
{
	float x1, y1, x2, y2;
};

struct Ponto
{
	float x, y;
};

Pontos area_transicao(float, float);

int calcula_variacao_x(float ball_pos, float _ball_pos)
{
	//
	// volatile float _ball_pos = 0;
	// DEBUGG LINES
	printf("PosiÁ„o atual da bola(X): %f \n", ball_pos);
	printf("PosiÁ„o antiga da bola(X): %f \n", _ball_pos);

	return (( ball_pos - _ball_pos) > 0) ? -1 : ((( ball_pos - _ball_pos) == 0) ? 0 : 1);
}

/**
 * @brief Calcula valores unit√°rios representando a dire√ß√£o de deslocamento da bola em y.
 *
 *
 * @return int
 */
int calcula_variacao_y(float ball_pos, float _ball_pos)
{
	//
	// volatile float _ball_pos = 0;
	printf("PosiÁ„o atual da bola(Y): %f \n", ball_pos);
	printf("PosiÁ„o antiga da bola(Y): %f \n", _ball_pos);

	return (( ball_pos - _ball_pos) > 0) ? -1 : ((( ball_pos - _ball_pos) == 0) ? 0 : 1);
}

Pontos area_transicao(float xb, float yb)
{
	Pontos retornar;

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

	// 	Cos do angulo formado por vd_AC e vd_CD
	float cos_ang1;

	// 	Cos do angulo formado por vd_BC e vd_CD
	float cos_ang2;

	// 	Angulo formado porvd_AC e vd_CD
	float ang1;

	// 	Angulo formado por vd_BC e vd_CD
	float ang2;

	// 	Dist√¢ncia do xb para o x do ponto novo
	float m;

	// 	Dist√¢ncia do xb para o x do ponto novo
	float n;

	if(yb < 85 && yb > 45)
	{
		cos_ang1 = (vd_CD.x * vd_AC.x) / (mvd_CD * mvd_AC);
		cos_ang2 = (vd_CD.x * vd_BC.x) / (mvd_CD * mvd_BC);

		ang1 = acos(cos_ang1);
		ang2 = acos(cos_ang2);

		m = 20 * tan(ang1);
		n = 20 * tan(ang2);

		retornar.x1 = xb - 20;
		retornar.y1 = yb - m;
		retornar.x2 = xb - 20;
		retornar.y2 = yb + n;
	}

	else if(yb == 45)
	{
		cos_ang1 = (vd_CD.x * vd_AC.x) / (mvd_CD * mvd_AC);
		ang1 = acos(cos_ang1);
		m = 20 * tan(ang1);

		retornar.x1 = xb  - 20;
		retornar.y1 = yb - m;
		retornar.x2 = xb - 20;
		retornar.y2 = yb;
	}

	else if(yb == 85)
	{
		cos_ang2 = (vd_CD.x * vd_BC.x) / (mvd_CD * mvd_BC);
		ang2 = acos(cos_ang2);
		n = 20 * tan(ang2);

		retornar.x1 = xb - 20;
		retornar.y1 = yb;
		retornar.x2 = xb - 20;
		retornar.y2 = yb + n;
	}

	else if(yb > 85)
	{
		cos_ang1 = (vd_CD.x * vd_AC.x) / (mvd_CD * mvd_AC);
		cos_ang2 = (vd_CD.x * vd_BC.x) / (mvd_CD * mvd_BC);

		ang1 = acos(cos_ang1);
		ang2 = acos(cos_ang2);

		m = 20 * tan(ang1);
		n = 20 * tan(ang2);

		retornar.x1 = xb - 20;
		retornar.y1 = yb + m;
		retornar.x2 = xb - 20;
		retornar.y2 = yb + n;
	}

	else
	{
		cos_ang1 = (vd_CD.x * vd_AC.x) / (mvd_CD * mvd_AC);
		cos_ang2 = (vd_CD.x * vd_BC.x) / (mvd_CD * mvd_BC);

		ang1 = acos(cos_ang1);
		ang2 = acos(cos_ang2);

		m = 20 * tan(ang1);
		n = 20 * tan(ang2);

		retornar.x1 = xb - 20;
		retornar.y1 = yb - m;
		retornar.x2 = xb - 20;
		retornar.y2 = yb - n;
	};

	// Testa se os novos pontos est√£o dentro dos limites do campo
	retornar.y1 = (retornar.y1 < 0) ? 0 : retornar.y1;
	retornar.y2 = (retornar.y2 > 130) ? 130 : retornar.y2;


	return retornar;
};


#endif
