#ifndef JOGADOR2_AMARELO_H
#define JOGADOR2_AMARELO_H

#include <Communications/StateReceiver.h>
#include <Communications/CommandSender.h>

#include "utils.hpp"
#include "Calculos.hpp"


Utils::Posture yellowObjective1(vss::State state, int index, vss::Robot goleiro, vss::Robot outro)
{
	printf("< Jogador 2 Amarelo >\n");
	return Utils::Posture(10, 10, M_PI / 4);
}

#endif
