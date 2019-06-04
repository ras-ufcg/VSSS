#ifndef GOLEIRO_Amarelo_H
#define GOLEIRO_Amarelo_H

#include <Communications/StateReceiver.h>
#include <Communications/CommandSender.h>
#include <math.h>
#include <iostream>
#include "utils.hpp"
#include "Calculos.hpp"


Utils::Posture yellowObjective(vss::State state, int index, vss::Robot goleiro, vss::Robot outro)
        {
            //float xpos = 120;
            //float ypos = state.ball.y;


            // --- linha de treino de goleiro --- //
            //xpos = 40;
            //ypos = 150;

            return Utils::Posture(10, 10, M_PI/4);
        }

#endif
