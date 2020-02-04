#ifndef GOLEIRO_AZUL_H
#define GOLEIRO_AZUL_H

#include <Communications/StateReceiver.h>
#include <Communications/CommandSender.h>

#include "utils.hpp"
#include "Calculos.hpp"

#define X_GLRO_AZUL 17

Utils::Posture blueObjective(vss::State state, int index)
{
	// Variáveis
	/*
	 * xb,yb - coordenadas da bola
	 * vxb, vyb - representação de um vetor unitário no sentido de variação de movimento da bola
	 * xg, yg - coordenadas do goleiro
	 * d1 e d2 limites estratégicos, 40 3 70 cm respec.
	 * ymax, ymin - limites dá área do goleiro
	 */

	vss::Robot robot = state.teamBlue[index];

	// Obtenção de valores para as variáveis
	printf("< Goleiro Azul >\n");
	// Obtem as coordenadas da bola
	bolinha.xb = state.ball.x;
	bolinha.yb = state.ball.y;

	// Obtem o vetor variação de acordo com os últimos dois estados recebidos
	float vxb = calcula_variacao_x(bolinha.xb, bolinha.old_xb);
	float vyb = calcula_variacao_y(bolinha.yb, bolinha.old_yb);
	// Atualização dos valores antigos  o_0

	bolinha.old_xb = bolinha.xb;
	bolinha.old_yb = bolinha.yb;

	// DEBUGG LINES
	printf("Variacao em X: %f \n", vxb);
	printf("Variacao em Y: %f \n", vyb);

	// Obtem as coordenadas do goleiro;
	float xg = robot.x;
	//float yg = robot.y;

	// Definição de constantes

	// a constante d1, marca uma linha em y à 40 cm do gol do nosso time
	// nas coordenadas do campo da simulação, esse valor equivale á 50.77
	float d1 = 80.77;

	// a constante d2 também demarca uma linha só que numa distância de
	// 70 cm do nosso gol. o que na simulação equivale à 88.53
	float d2 = 108.53;

	// limites das áreas dos goleiros
	float y_min = 40.00;
	float y_max = 90.00;


	// Calculos geométricos

	// Calcula a tinha de interceção entre a provável trajetória da bola e
	// a linha de ação do goleiro
	float y_inters = bolinha.yb - ((vyb * (bolinha.xb - xg)) / (vxb));

	// Máquina de estados

	volatile char estado = 'a';

	// Regras de transição

	if(estado == 'a')
	{
		// Regra 1
		estado = ((bolinha.xb > d1) && (vxb < 0)) ? 'b' : 'a';
		// Regra 2
		estado = ((bolinha.xb < d2) && (vxb >= 0)) ? 'c' : 'a';
	}

	if(estado == 'b')
	{
		// Regra 2
		estado = ((bolinha.xb < d2) && (vxb >= 0)) ? 'c' : 'b';
	}

	if(estado == 'c')
	{
		// Regra 1
		estado = ((bolinha.xb > d1) && (vxb < 0)) ? 'b' : 'c';
		// Regra 3
		estado = (bolinha.xb >= d2) ? 'a' : 'c';
	}
	printf("Estado: %c \n", estado);

	switch (estado)
	{
	case 'a':
		// Goleiro no centro do gol
		return Utils::Posture(X_GLRO_AZUL, 65.30, M_PI / 4);
		break;

	case 'b':
		// Goleiro na posição estimada
		return Utils::Posture(X_GLRO_AZUL, y_inters, M_PI / 4);
		break;

	case 'c':
		// Goleiro seguindo o y da bola
		// Dentro dos limites min e max da área
bolinha.yb = (bolinha.yb > y_min) ? (bolinha.yb < y_max) ? bolinha.yb : y_max : y_min;

		return Utils::Posture(X_GLRO_AZUL, bolinha.yb, M_PI / 4);
		break;

	default:
		// Goleiro no centro do gol
		return Utils::Posture(X_GLRO_AZUL, 65.30, M_PI / 4);
		break;
	}
}

#endif
