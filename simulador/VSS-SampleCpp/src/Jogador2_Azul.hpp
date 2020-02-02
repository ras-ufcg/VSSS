#ifndef JOGADOR2_AZUL_H
#define JOGADOR2_AZUL_H //Cor auxiliar roxo

#include <Communications/StateReceiver.h>
#include <Communications/CommandSender.h>

#include "utils.hpp"
#include "Calculos.hpp"


Utils::Posture blueObjective1(vss::State state, int index, vss::Robot goleiro, vss::Robot outro)
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
	float xa = robot.x;
	float ya = robot.y;

	printf("Atacante em X: %f \n", xa);
	printf("Atacante em Y: %f \n", ya);
	// Obtem as coordenadas dos outros jogadores
	float xd = outro.x;
	float yd = outro.y;
	float xg = goleiro.x;
	//float yg = goleiro.y;

	// Definição de constantes

	//d1 é 4 cm
	float d1 = 4;

	// d2 é 20 cm
	float d2 = 20;

	// d3 é 4 cm
	float d3 = 4;

	// d4 é 8 cm
	float d4 = 8;

	//d1 é 4 cm
	//float d1 = 8.07;

	// d2 é 20 cm
	//float d2 = 40.39;

	// d3 é 4 cm
	//float d3 = 8.07;

	// d4 é 8 cm
	//float d4 = 16.14;

	// limites das áreas dos goleiros
	float y_min = 40.00;
	float y_max = 90.00;


	// Calculos geométricos

	// Calcula a linha de interceção entre a provável trajetória da bola e
	// a linha de ação do defensor
	float y_inters = bolinha.yb - ((vyb * (bolinha.xb - xd)) / (vxb));

	// Máquina de estados

	volatile char estado = 'd';



	// Regras de transição de estados

	if(estado == 'a')
	{
		// Regra 2
		estado = ((bolinha.xb + d1) < xd) ? 'b' : 'a';

		// Regra 4 (permuta de posições)

		estado = (((bolinha.xb - xd) < d3) && (y_min <= (bolinha.yb + (((yd - bolinha.yb) * (bolinha.xb - xg)) / (xd - bolinha.xb)))) && (y_max <= (bolinha.yb + (((yd - bolinha.yb) * (bolinha.xb - xg)) / (xd - bolinha.xb)))) && area_t(bolinha.xb, bolinha.yb, xd, yd))? 'd' : 'a';
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
		estado = (((bolinha.xb - xd) < d3) && (y_min <= (bolinha.yb + (((yd - bolinha.yb) * (bolinha.xb - xg)) / (xd - bolinha.xb)))) && (y_max <= (bolinha.yb + (((yd - bolinha.yb) * (bolinha.xb - xg)) / (xd - bolinha.xb)))) && area_t(bolinha.xb, bolinha.yb, xa, ya)) ? 'b' : 'e';
		// Regra 5
		estado = ((bolinha.xb - xa) < d3) && (y_min <= (bolinha.yb + (((ya - bolinha.yb) * (bolinha.xb - xg)) / (xa - bolinha.xb)))) && (y_max <= (bolinha.yb + (((ya - bolinha.yb) * (bolinha.xb - xg)) / (xa - bolinha.xb)))) ? 'd' : 'e';
	}


	printf("Estado: %c \n", estado);

	switch (estado)
	{
	case 'a':
		// FEITO
		// Impedindo a aproximação da bola
		return Utils::Posture(xa, bolinha.yb, M_PI / 4);
		break;

	case 'b':
		// FEITO
		// Impedindo que o adversário chute a bola
		return Utils::Posture(xa, y_inters, M_PI / 4);
		break;

	case 'c':
		// FEITO
		if(yd <= 65) return Utils::Posture(xd, yd + 20, M_PI / 4);
		else return Utils::Posture(xa, ya - 20, M_PI / 4);
		break;

	case 'd':
		// Levando a bola em direção ao ataque

		// TODO: REVISAR POSICIONAMENTO DO JOGADOR

		if(ya != bolinha.yb && xa != bolinha.xb) return Utils::Posture(bolinha.xb, bolinha.yb, M_PI / 4);
		else return Utils::Posture(150, 65, M_PI / 4);


		break;

	case 'e':
		// Posicionando o robô atrás da bola
		return Utils::Posture(bolinha.xb, bolinha.yb, M_PI / 4);
		break;


	default:
		// Posição
		return Utils::Posture(xa, ya, M_PI / 4);
		break;
	}

	return Utils::Posture(75, 5, M_PI / 4);
}
#endif // JOGADOR2_AZUL_H