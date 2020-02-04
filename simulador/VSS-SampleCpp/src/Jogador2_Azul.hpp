#ifndef JOGADOR2_AZUL_H
#define JOGADOR2_AZUL_H

#include <Communications/StateReceiver.h>
#include <Communications/CommandSender.h>

#include "utils.hpp"
#include "Calculos.hpp"


Utils::Posture blueObjective2(vss::State state, int index, vss::Robot goleiro, vss::Robot outro)
{
	// Variáveis
	/*
	 * xb,yb - coordenadas da bola
	 * vxb, vyb - representação de um vetor unitário no sentido de variação de movimento da bola
	 * xg, yg - coordenadas do goleiro
	 * xd, yd - coordenadas do defensor
	 * xa, ya - coordenadas do atacante
	 * d1, d2, d3 e d4 limites estratégicos, 4 20 4 8 cm respec.
	 * ymax, ymin - limites dá área do goleiro
	 */

	//vss::Robot robot = (this->_teamType == vss::TeamType::Blue) ? state.teamBlue[index] : state.teamYellow[index];

	vss::Robot robot = state.teamBlue[index];

	// Obtenção de valores para as variáveis
	printf("< Jogador 2 Azul >\n");
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

	// Obtem as coordenadas do defensor;
	float xd = robot.x;
	float yd = robot.y;

	// Obtem as coordenadas dos outros jogadores
	float xa = outro.x;
	float ya = outro.y;
	float xg = goleiro.x;
	//float yg = goleiro.y;

	// Definição de constantes

	// Area de transição

	// a constante dxa, marca a area de transição de 20 cm, nas coordenadas
	// do campo da simulação, esse valor equivale à 40.38
	//int dxa = 20;
	// a constante d1, marca uma linha em y à 40 cm do gol do nosso time
	// nas coordenadas do campo da simulação, esse valor equivale á 50.77
	//float d1 = 80.77;
	//
	// a constante d2 também demarca uma linha só que numa distância de
	// 70 cm do nosso gol. o que na simulação equivale à 88.53
	//float d2 = 108.53;
	//
	// constantes

	//d1 é 4 cm
	short int d1 = 4;

	// d2 é 20 cm
	short int d2 = 20;

	// d3 é 4 cm
	short int d3 = 4;

	// d4 é 8 cm
	short int d4 = 8;

	// limites das áreas dos goleiros
	float y_min = 40.00;
	float y_max = 90.00;


	// Calculos geométricos

	// Calcula a linha de interceção entre a provável trajetória da bola e
	// a linha de ação do defensor
	float y_inters = bolinha.yb - ((vyb * (bolinha.xb - xd)) / (vxb));

	// Máquina de estados

	volatile char estado = 'a';

	// Valores para área de transição

	Pontos resultado = area_transicao(bolinha.xb, bolinha.yb);

	printf("Ponto 1:  (%f, %f) \n", resultado.x1, resultado.y1);
	printf("Ponto 2:  (%f, %f) \n", resultado.x2, resultado.y2);

	// Regras de transição de estados

	if(estado == 'a')
	{
		// Regra 2
		estado = ((bolinha.xb + d1) < xd) ? 'b' : 'a';

		// Regra 4 (permuta de posições)

		estado = ((bolinha.xb - xd) < d3) && (y_min <= (bolinha.yb + (((yd - bolinha.yb) * (bolinha.xb - xg)) / (xd - bolinha.xb)))) && (y_max <= (bolinha.yb + (((yd - bolinha.yb) * (bolinha.xb - xg)) / (xd - bolinha.xb)))) ? 'd' : 'a';
	}

	if(estado == 'b')
	{
		// Regra 1
		estado = ((bolinha.xb - d1) > xd) ? 'a' : 'b';
		// Regra 3
		estado = ((xa < bolinha.xb) && (bolinha.xb < xd) && ((ya - bolinha.yb) < d2) && ((yd - bolinha.yb) < d2)) ? 'c' : 'b';
	}

	if(estado == 'c')
	{
		// Regra 1
		estado = ((bolinha.xb - d1) > xd) ? 'a' : 'c';
	}

	if(estado == 'd')
	{
		// Regra 6
		estado = ((xa - bolinha.xb) > d4) ? 'e' : 'd';
	}

	if(estado == 'e')
	{
		// Regra 4 (permuta de posições)
		estado = ((bolinha.xb - xd) < d3) && (y_min <= (bolinha.yb + (((yd - bolinha.yb) * (bolinha.xb - xg)) / (xd - bolinha.xb)))) && (y_max <= (bolinha.yb + (((yd - bolinha.yb) * (bolinha.xb - xg)) / (xd - bolinha.xb)))) ? 'b' : 'e';
		// Regra 5
		estado = ((bolinha.xb - xa) < d3) && (y_min <= (bolinha.yb + (((ya - bolinha.yb) * (bolinha.xb - xg)) / (xa - bolinha.xb)))) && (y_max <= (bolinha.yb + (((ya - bolinha.yb) * (bolinha.xb - xg)) / (xa - bolinha.xb)))) ? 'd' : 'e';
	}


	printf("Estado: %c \n", estado);

	switch (estado)
	{
	case 'a':
		// FEITO
		// Impedindo a aproximação da bola
		return Utils::Posture(xd, bolinha.yb, M_PI / 4);
		break;

	case 'b':
		// FEITO
		// Impedindo que o adversário chute a bola
		return Utils::Posture(xd, y_inters, M_PI / 4);
		break;

	case 'c':
		// FEITO
		if(yd <= 65) return Utils::Posture(xd, yd + 20, M_PI / 4);
		else return Utils::Posture(xd, yd - 20, M_PI / 4);
		break;

	case 'd':
		// Levando a bola em direção ao ataque

		// TODO: REVISAR POSICIONAMENTO DO JOGADOR

		if(yd != bolinha.yb) return Utils::Posture(xd, bolinha.yb, M_PI / 4);
		else return Utils::Posture(150, 65, M_PI / 4);


		break;

	case 'e':
		// Posicionando o robô atrás da bola
		return Utils::Posture(bolinha.xb - 4.5 , bolinha.yb, M_PI / 4);
		break;


	default:
		// Posição
		return Utils::Posture(xd, yd, M_PI / 4);
		break;
	}

	return Utils::Posture(75, 5, M_PI / 4);
}
#endif // JOGADOR2_AZUL_H
