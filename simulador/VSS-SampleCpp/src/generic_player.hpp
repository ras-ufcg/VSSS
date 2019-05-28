#ifndef GENERIC_PLAYER_H
#define GENERIC_PLAYER_H

#include <Communications/StateReceiver.h>
#include <Communications/CommandSender.h>
#include <math.h>
#include <iostream>
#include "utils.hpp"

#define X_GLRO_AZUL 17
#define X_GLRO_AMRL 151

struct ball_s{
    float xb = 0;
    float yb = 0;
    float old_xb = 0;
    float old_yb = 0;
}bolinha;

/**
 * @brief Guarda a lógica de funcionamento do jogador de camisa vermelha (goleiro)
 *
 */
class GenericPlayer {
    private:
        vss::TeamType _teamType;

    public:

        GenericPlayer(vss::TeamType type) {
            this->_teamType = type;
        }

        /**
         * @brief Função chamada a cada atualização com informações novas
         *
         * @param state Estado atual do jogo
         * @param index Indice do robo que se deseja atualizar
         * @return vss::WheelsCommand Comandos para as rodas do robo que se deseja atualizar
         */
        vss::WheelsCommand update(vss::State state, int index) {
            Utils::Posture objective = defineObjective(state, index);
            return motionControl(state, objective, index);
        }

        /**
         * @brief Define a postura final que se deseja chegar na iteração atual
         *
         * @param index Indice do robo que se deseja atualizar
         * @return Posture Posiçãoque se deseja adquirir
         */
        Utils::Posture defineObjective(vss::State state, int index)
        {

            return (this->_teamType == vss::TeamType::Blue)? blueObjective(state, index) : yellowObjective(state, index);
        }

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
            float y_inters = bolinha.yb - ((vyb * (bolinha.xb - xg))/(vxb));

            // Máquina de estados

            volatile char estado = 'a';

            // Regras de transição

            if(estado == 'a')
            {
                // Regra 1
                estado = ((bolinha.xb > d1) && (vxb < 0))? 'b' : 'a';
                // Regra 2
                estado = ((bolinha.xb < d2) && (vxb >= 0))? 'c' : 'a';
            }

            if(estado == 'b')
            {
                // Regra 2
                estado = ((bolinha.xb < d2) && (vxb >= 0))? 'c' : 'b';
            }

            if(estado == 'c')
            {
                // Regra 1
                estado = ((bolinha.xb > d1) && (vxb < 0))? 'b' : 'c';
                // Regra 3
                estado = (bolinha.xb >= d2)? 'a' : 'c';
            }


            switch (estado)
            {
                case 'a':
                    // Goleiro no centro do gol
                    return Utils::Posture(X_GLRO_AZUL, 65.30, M_PI/4);
                    break;

                case 'b':
                    // Goleiro na posição estimada
                    return Utils::Posture(X_GLRO_AZUL, y_inters, M_PI/4);
                    break;

                case 'c':
                    // Goleiro seguindo o y da bola
                    // Dentro dos limites min e max da área
                    bolinha.yb = (bolinha.yb > y_min)? (bolinha.yb < y_max)? bolinha.yb : y_max : y_min;

                    return Utils::Posture(X_GLRO_AZUL, bolinha.yb, M_PI/4);
                    break;

                default:
                    // Goleiro no centro do gol
                    return Utils::Posture(X_GLRO_AZUL, 65.30, M_PI/4);
                    break;
            }
        }

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

            vss::Robot robot = (this->_teamType == vss::TeamType::Blue) ? state.teamBlue[index] : state.teamYellow[index];

            // Obtenção de valores para as variáveis

            // Obtem as coordenadas da bola

            bolinha.xb = state.ball.x;
            bolinha.yb = state.ball.y;

            // Obtem o vetor variação de acordo com os últimos dois estados recebidos
            float vxb = calcula_variacao_x(bolinha.xb, bolinha.old_xb)*(-1);
            float vyb = calcula_variacao_y(bolinha.yb, bolinha.old_yb)*(-1);
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
            float y_inters = bolinha.yb - ((vyb * (bolinha.xb - xg))/(vxb));

            // Máquina de estados

            volatile char estado = 'a';

            // Regras de transição

            if(estado == 'a')
            {
                // Regra 1
                estado = ((bolinha.xb < d1) && (vxb < 0))? 'b' : 'a';
                // Regra 2
                estado = ((bolinha.xb > d2) && (vxb >= 0))? 'c' : 'a';
            }

            if(estado == 'b')
            {
                // Regra 2
                estado = ((bolinha.xb > d2) && (vxb >= 0))? 'c' : 'b';
            }

            if(estado == 'c')
            {
                // Regra 1
                estado = ((bolinha.xb < d1) && (vxb < 0))? 'b' : 'c';
                // Regra 3
                estado = (bolinha.xb <= d2)? 'a' : 'c';
            }


            switch (estado)
            {
                case 'a':
                    // Goleiro no centro do gol
                    return Utils::Posture(X_GLRO_AMRL, 65.30, M_PI/4);
                    break;

                case 'b':
                    // Goleiro na posição estimada
                    return Utils::Posture(X_GLRO_AMRL, y_inters, M_PI/4);
                    break;

                case 'c':
                    // Goleiro seguindo o y da bola
                    // Dentro dos limites min e max da área
                    bolinha.yb = (bolinha.yb > y_min)? (bolinha.yb < y_max)? bolinha.yb : y_max : y_min;

                    return Utils::Posture(X_GLRO_AMRL, bolinha.yb, M_PI/4);
                    break;

                default:
                    // Goleiro no centro do gol
                    return Utils::Posture(X_GLRO_AMRL, 65.30, M_PI/4);
                    break;
            }
        }

        /**
         * @brief Calcula valores unitários representando a direção de deslocamento da bola em x.
         *
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

            return (( ball_pos - _ball_pos) > 0)? -1 : (( ball_pos - _ball_pos) == 0)? 0 : 1;
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

            return (( ball_pos - _ball_pos) > 0)? -1 : (( ball_pos - _ball_pos) == 0)? 0 : 1;
        }

        // TODO criar (ou usar um existente) typedef para calcular as duas variações em uma única função
        // e retornar os dois valores em uma única variável

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
