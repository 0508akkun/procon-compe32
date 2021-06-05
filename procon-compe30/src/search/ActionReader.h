# pragma once
# include "Field.h"
# include "picojson/picojson.h"

class ActionReader
{
public:
    ActionReader() = default;
    ActionReader(picojson::value val);

    bool writeTo(std::vector<Action>& actions);

private:
    picojson::value init_value;
    Field m_field;
};