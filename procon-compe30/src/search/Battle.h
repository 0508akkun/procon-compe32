# pragma once
# include "Match.h"
# include "Field.h"

void LocalBattle(const Field& field);
void Battle(const Match& match, bool debug, int error_count = 0);
