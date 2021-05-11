# pragma once
# include <string>
# include <fstream>
# include "picojson/picojson.h"
# include "Field.h"

using FilePath = std::string;

class FieldReader
{
public:
    FieldReader() = default;
    FieldReader(picojson::value val);
    ~FieldReader();

    bool setPicojsonValue(picojson::value value);
    bool writeTo(Field& field);

private:
    picojson::value init_value;
    Field m_field;
};
