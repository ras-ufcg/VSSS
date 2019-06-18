#ifndef JOGADOR1_AMARELO_H
#define JOGADOR1_AMARELO_H

#include <Communications/StateReceiver.h>
#include <Communications/CommandSender.h>

#include "utils.hpp"
#include "Calculos.hpp"


Utils::Posture yellowObjective(vss::State state, int index, vss::Robot goleiro, vss::Robot outro)
{
	
	printf("< Jogador 1 Amarelo >\n");
	return Utils::Posture(20, 10, M_PI / 4);
}


#endif
