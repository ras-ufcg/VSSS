#ifndef GENERIC_PLAYER1_H
#define GENERIC_PLAYER1_H

#include <Communications/StateReceiver.h>
#include <Communications/CommandSender.h>
#include <math.h>
#include <iostream>
#include "utils.hpp"

/**
 * @brief Guarda a lógica de funcionamento do jogador de camisa verde (atacante)
 *
 */
class GenericPlayer1 {
    private:
        vss::TeamType _teamType;

    public:

        GenericPlayer1(vss::TeamType type) {
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


        Utils::Posture yellowObjective(vss::State state, int index)
        {
            float xpos = state.ball.x;
            float ypos = state.ball.y;
            float limitSup = 8;
            float limitInf = 118;

            vss::Robot robot = state.teamYellow[index];

            bool posAtkX = false;
            //bool posAtkY = false;

            // Se a bola estiver no campo de ataque, posiciona-se 10 passos atras da bola
            if (state.ball.x <= 100) xpos = state.ball.x + 5;

            // Se a bola estiver no campo de defesa, espera voltar para o ataque
            if (state.ball. x > 100) xpos = 110;

            // Se a bola estiver na metade de baixo do campo, posiciona-se 10 passos acima da bola
            if ((state.ball.y >= 62.5) && (state.ball.y > 10)) ypos = state.ball.y + 5;

            // Se a bola estiver na metade de cima do campo, posiciona-se 10 passos abaixo da bola
            if ((state.ball.y < 62.5) && (state.ball.y < 115)) ypos = state.ball.y - 5;

             // Aplica limites em Y
            if (ypos <= limitSup) ypos = limitSup;
            if (ypos >= limitInf) ypos = limitInf;

            // Se o robô estiver em posição de ataque, levar a bola para o gol
            if ((robot.x < (state.ball.x + 10)) && (robot.x > (state.ball.x + 5))) posAtkX = true;
            else posAtkX = false;
            //if ((state.ball.y >= 62.5) && (robot.y > 10)) posAtkY = true;
            //if ((state.ball.y < 62.5) && (state.ball.y < 115)) posAtkY = true;

            if (posAtkX) printf("Ok em X\n");
            else printf("Esperando OK em X\n");
            //if (posAtkY) printf("Ok em Y\n");


            return Utils::Posture(xpos, ypos, M_PI/4);
        }

        Utils::Posture blueObjective(vss::State state, int index)
        {
            float xpos = state.ball.x;
            float ypos = state.ball.y;
            float limitSup = 8;
            float limitInf = 118;

            vss::Robot robot = state.teamYellow[index];

            bool posAtkX = false;
            //bool posAtkY = false;

            // Se a bola estiver no campo de ataque, posiciona-se 10 passos atras da bola
            if (state.ball.x >= 55) xpos = state.ball.x - 5;

            // Se a bola estiver no campo de defesa, espera voltar para o ataque
            if (state.ball. x < 50) xpos = 55;

            // Se a bola estiver na metade de baixo do campo, posiciona-se 10 passos acima da bola
            if ((state.ball.y >= 62.5) && (state.ball.y > 10)) ypos = state.ball.y + 5;

            // Se a bola estiver na metade de cima do campo, posiciona-se 10 passos abaixo da bola
            if ((state.ball.y < 62.5) && (state.ball.y < 115)) ypos = state.ball.y - 5;

            // Aplica limites em Y
            if (ypos <= limitSup) ypos = limitSup;
            if (ypos >= limitInf) ypos = limitInf;


            // Se o robô estiver em posição de ataque, levar a bola para o gol
            if ((robot.x < (state.ball.x + 10)) && (robot.x > (state.ball.x + 5))) posAtkX = true;
            else posAtkX = false;
            //if ((state.ball.y >= 62.5) && (robot.y > 10)) posAtkY = true;
            //if ((state.ball.y < 62.5) && (state.ball.y < 115)) posAtkY = true;

            if (posAtkX) printf("Ok em X\n");
            else printf("Esperando OK em X\n");
            //if (posAtkY) printf("Ok em Y\n");

            return Utils::Posture(xpos, ypos, M_PI/4.);
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
