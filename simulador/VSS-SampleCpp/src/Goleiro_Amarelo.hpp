#ifndef GOLEIRO_AMARELO_H
#define GOLEIRO_AMARELO_H

#include <Communications/StateReceiver.h>
#include <Communications/CommandSender.h>

#include "utils.hpp"
#include "Calculos.hpp"

#define X_GLRO_AMRL 151

Utils::Posture yellowObjective(vss::State state, int index)
{
	// Variáveis
	/*
	 * xb,yb - coordenadas da bola
	 * vxb, vyb - representação de um vetor unitário no sentido de variação de movimento da bola
	 * xg, yg - coordenadas do goleiro
	 * d1 e d2 limites estratégicos, 40 3 70 cm respec.
	 * ymax, ymin - limites dá área do goleiro
	 */

	vss::Robot robot = state.teamYellow[index];

	// Obtenção de valores para as variáveis
	printf("< Goleiro Amarelo >\n");
	// Obtem as coordenadas da bola

	bolinha.xb = state.ball.x;
	bolinha.yb = state.ball.y;

	// Obtem o vetor variação de acordo com os últimos dois estados recebidos
	float vxb = calcula_variacao_x(bolinha.xb, bolinha.old_xb) * (-1);
	float vyb = calcula_variacao_y(bolinha.yb, bolinha.old_yb) * (-1);
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

	// a constante d1, marca uma linha em y à 80.77 cm do gol do time amarelo
	float d1 = 88.16;

	// a constante d1, marca uma linha em y à 108.53 cm do gol do time amarelo
	float d2 = 61.60;

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
		estado = ((bolinha.xb < d1) && (vxb < 0)) ? 'b' : 'a';
		// Regra 2
		estado = ((bolinha.xb > d2) && (vxb >= 0)) ? 'c' : 'a';
	}

	if(estado == 'b')
	{
		// Regra 2
		estado = ((bolinha.xb > d2) && (vxb >= 0)) ? 'c' : 'b';
	}

	if(estado == 'c')
	{
		// Regra 1
		estado = ((bolinha.xb < d1) && (vxb < 0)) ? 'b' : 'c';
		// Regra 3
		estado = (bolinha.xb <= d2) ? 'a' : 'c';
	}

	printf("Estado: %c \n", estado);


	switch (estado)
	{
	case 'a':
		// Goleiro no centro do gol
		return Utils::Posture(X_GLRO_AMRL, 65.30, M_PI / 4);
		break;

	case 'b':
		// Goleiro na posição estimada
		return Utils::Posture(X_GLRO_AMRL, y_inters, M_PI / 4);
		break;

	case 'c':
		// Goleiro seguindo o y da bola
		// Dentro dos limites min e max da área
bolinha.yb = (bolinha.yb > y_min) ? (bolinha.yb < y_max) ? bolinha.yb : y_max : y_min;

		return Utils::Posture(X_GLRO_AMRL, bolinha.yb, M_PI / 4);
		break;

	default:
		// Goleiro no centro do gol
		return Utils::Posture(X_GLRO_AMRL, 65.30, M_PI / 4);
		break;
	}
}

#endif
