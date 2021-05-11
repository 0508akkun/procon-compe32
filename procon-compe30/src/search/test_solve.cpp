# include "Action.h"
# include "FieldReader.h"
# include "Field.h"
# include "beamsearch.h"
# include "picojson/picojson.h"
# include <bits/stdc++.h>

int main()
{
    Field field;

    std::ifstream ifs;
    ifs.open("testcase/field_turn0.json");

    picojson::value val;
    ifs >> val;
    ifs.close();

    FieldReader reader(val);
    reader.writeTo(field);

    DisplayFieldData(field);

    /// ally_index = 0としている
    Match match(0, 100, "瑠璃高専", field.teams[0].teamID, 5000, 30);

    std::vector<Action> actions = solve_beam(field, match);
    std::cout << ToJson(actions) << std::endl;

    return 0;
}