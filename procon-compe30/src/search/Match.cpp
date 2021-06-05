# include "Match.h"

Match::Match(int _id, int _intervalMillis, std::string _matchTo, int _teamID, int _turnMillis, int _turns)
    : id(_id)
    , intervalMillis(_intervalMillis)
    , matchTo(_matchTo)
    , teamID(_teamID)
    , turnMillis(_turnMillis)
    , turns(_turns)
{}

