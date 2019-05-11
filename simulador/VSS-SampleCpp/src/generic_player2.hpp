#ifndef GENERIC_PLAYER2_H
#define GENERIC_PLAYER2_H

#include <Communications/StateReceiver.h>
#include <Communications/CommandSender.h>
#include <math.h>
#include <iostream>
#include "utils.hpp"


/*
struct ball_pos{
    float xb = 0;
    float yb = 0;
    float old_xb = 0;
    float old_yb = 0;
}bolinha;
*/
/**
 * @brief Guarda a lógica de funcionamento do jogador de camisa roxa (zagueiro)
 *
 */
class GenericPlayer2 {
    private:
        vss::TeamType _teamType;

    public:

        GenericPlayer2(vss::TeamType type) {
            this->_teamType = type;
        }

        /**
         * @brief Função chamada a cada atualização com informações novas
         *
         * @param state Estado atual do jogo
         * @param index Indice do robo que se deseja atualizar
         * @return vss::WheelsCommand Comandos para as rodas do robo que se deseja atualizar
         */
        vss::WheelsCommand update(vss::State state, int index)
        {
            Utils::Posture objective = defineObjective(state, index);
            return motionControl(state, objective, index);
        }

        /**
         * @brief Define a postura final que se deseja chegar na iteração atual
         *
         * @param index Indice do robo que se deseja atualizar
         * @param state Estado atual do jogador
         * @return Posture Posiçãoque se deseja adquirir
         */
        Utils::Posture defineObjective(vss::State state, int index)
        {

            vss::Robot goleiro = (this->_teamType == vss::TeamType::Blue) ? state.teamBlue[0] : state.teamYellow[0];

            vss::Robot outro = (this->_teamType == vss::TeamType::Blue) ? state.teamBlue[1] : state.teamYellow[1];

            return (this->_teamType == vss::TeamType::Blue)? blueObjective(state, index, goleiro, outro) : yellowObjective(state, index, goleiro, outro);

        }


        Utils::Posture yellowObjective(vss::State state, int index, vss::Robot goleiro, vss::Robot outro)
        {
            //float xpos = 120;
            //float ypos = state.ball.y;


            // --- linha de treino de goleiro --- //
            //xpos = 40;
            //ypos = 150;

            return Utils::Posture(95, 150, M_PI/4);
        }

        Utils::Posture blueObjective(vss::State state, int index, vss::Robot goleiro, vss::Robot outro)
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

            vss::Robot robot = (this->_teamType == vss::TeamType::Blue) ? state.teamBlue[index] : state.teamYellow[index];

            // Obtenção de valores para as variáveis

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
            float y_inters = bolinha.yb - ((vyb * (bolinha.xb - xd))/(vxb));

            // Máquina de estados

            volatile char estado = 'a';

            // Regras de transição

            if(estado == 'a')
            {
                // Regra 2
                estado = ((bolinha.xb + d1) < xd)? 'b' : 'a';
                // Regra 4 (permuta de posições)

                estado = ((bolinha.xb - xd) < d3) && (y_min <=(bolinha.yb + (((yd - bolinha.yb)*(bolinha.xb - xg))/(xd - bolinha.xb)))) && (y_max <=(bolinha.yb + (((yd - bolinha.yb)*(bolinha.xb - xg))/(xd - bolinha.xb))))? 'd' : 'a';
            }

            if(estado == 'b')
            {
                // Regra 1
                estado = ((bolinha.xb - d1) > xd)? 'a' : 'b';
				// Regra 3
                estado = ((xa < bolinha.xb) && (bolinha.xb < xd) && ((ya - bolinha.yb) < d2) && ((yd - bolinha.yb) < d2))? 'c' : 'b';
            }

            if(estado == 'c')
            {
                // Regra 1
                estado = ((bolinha.xb - d1) > xd)? 'a' : 'c';
            }

            if(estado == 'd')
            {
				// Regra 6
                estado = ((xa - bolinha.xb) > d4)? 'e' : 'd';
            }

            if(estado == 'e'){
				// Regra 4 (permuta de posições)
                estado = ((bolinha.xb - xd) < d3) && (y_min <=(bolinha.yb + (((yd - bolinha.yb)*(bolinha.xb - xg))/(xd - bolinha.xb)))) && (y_max <=(bolinha.yb + (((yd - bolinha.yb)*(bolinha.xb - xg))/(xd - bolinha.xb))))? 'b' : 'e';
				// Regra 5
                estado = ((bolinha.xb - xa) < d3) && (y_min <=(bolinha.yb + (((ya - bolinha.yb)*(bolinha.xb - xg))/(xa - bolinha.xb)))) && (y_max <=(bolinha.yb + (((ya - bolinha.yb)*(bolinha.xb - xg))/(xa - bolinha.xb))))? 'd' : 'e';
			}


            switch (estado)
            {
                case 'a':
                    // FEITO
                    // Impedindo a aproximação da bola
                    return Utils::Posture(xd, bolinha.yb, M_PI/4);
                    break;

                case 'b':
                    // FEITO
                    // Impedindo que o adversário chute a bola
                    return Utils::Posture(xd, y_inters, M_PI/4);
                    break;

                case 'c':
                    // FEITO
                    if(yd <= 65) return Utils::Posture(xd, yd+20, M_PI/4);
                    else return Utils::Posture(xd, yd-20, M_PI/4);
                    break;

				case 'd':
					// Levando a bola em direção ao ataque

					// TODO: REVISAR POSICIONAMENTO DO JOGADOR

					if(yd != bolinha.yb) return Utils::Posture(xd, bolinha.yb, M_PI/4);
					else return Utils::Posture(150, 65, M_PI/4);


					break;

				case 'e':
					// Posicionando o robô atrás da bola
					return Utils::Posture(bolinha.xb - 4.5 ,bolinha.yb, M_PI/4);
					break;


                default:
                    // Posição
                    return Utils::Posture(xd, yd, M_PI/4);
                    break;
            }

            return Utils::Posture(75, 5, M_PI/4);
        }

        /**
         * @brief Calcula valores unitários representando a direção de deslocamento da bola em x.
         * @param bolinha.xb = state.ball.x;
           @param bolinha.yb = state.ball.y;

            // Obtem o vetor variação de acordo com os últimos dois estados recebidos
            float vxb = calcula_variacao_x(bolinha.xb, bolinha.old_xb);
            float vyb = calcula_variacao_y(bolinha.yb, bolinha.old_yb);
            // Atualização dos valores antigos  o_0

            bolinha.old_xb = bolinha.xb;
bolinha.old_yb = bolinha.yb;

            // Atualização dos valores antigos  o_0
            old_xb = xb;
            old_yb = yb;

         *
         * @return int
         */
        int calcula_variacao_x(float ball_pos, float _ball_pos)
        {
            //
            // volatile float _ball_pos = 0;
            // DEBUGG LINES
            printf("Posição atual da bola: %f \n", ball_pos);
            printf("Posição antiga da bola %f \n", _ball_pos);

            return (( ball_pos - _ball_pos) > 0)? 1 : (( ball_pos - _ball_pos) == 0)? 0 : -1;
        }

        /**
         * @brief Calcula valores unitários representando a direção de deslocamento da bola em y.
         *
         *
         * @return int
         */
        int calcula_variacao_y(float ball_pos, float _ball_pos)
        {
            //
            // volatile float _ball_pos = 0;

            return (( ball_pos - _ball_pos) > 0)? 1 : (( ball_pos - _ball_pos) == 0)? 0 : -1;
        }


        /**
         * @brief Traduz o objetivo desejado em velocidade das rodas utilizando o algoritmo Motion Control presente
         * no livro Autonomous Mobile Robots, Siegwart
         *
         * @param state Situação de jogo atual
         * @param objective Posição que se deseja chegar
         * @param index Indice do robo que se deseja atualizar
         * @return vss::WheelsCommand Comandos para as rodas do robo que se deseja atualizar
         */
        vss::WheelsCommand motionControl(vss::State state, Utils::Posture objective, int index)
        {
            vss::WheelsCommand result;
            double  alpha, beta, rho, lambda;
            double linearSpeed, angularSpeed;

            double kRho = 1.85, kAlpha = 9.7, kBeta = -0.01;

            vss::Robot robot = (this->_teamType == vss::TeamType::Blue) ? state.teamBlue[index] : state.teamYellow[index];

            robot.angle = Utils::to180range(robot.angle * M_PI / 180.0);

            rho = Utils::distance(doublePair(objective.x, objective.y), doublePair(robot.x, robot.y));

            lambda = atan2(objective.y - robot.y, objective.x - robot.x);

            if (rho < 3)
                lambda = 0;

            alpha = Utils::to180range(lambda - robot.angle);
            beta = -lambda + objective.angle;

            linearSpeed = -kRho * rho;
            angularSpeed = kAlpha * alpha + kBeta * beta;

            double constantSpeed = 60;
            double scale = constantSpeed / linearSpeed;
            linearSpeed *= scale;
            angularSpeed *= scale;

            if (rho < 3){
                linearSpeed = 0;
                angularSpeed *= 0.4;
            }

            if (fabs(alpha) > 0.5 * M_PI) {
                linearSpeed = -linearSpeed;
            }

            result.leftVel = (linearSpeed - angularSpeed * 3.35) / 2.;
            result.rightVel = (linearSpeed + angularSpeed * 3.35) / 2.;

            double maxSpeed = std::max(fabs(result.leftVel), fabs(result.rightVel));

            if (maxSpeed > 100) {
                result.leftVel *= 100 / fabs(maxSpeed);
                result.rightVel *= 100 / fabs(maxSpeed);
            }

            return result;
        }

};

#endif
