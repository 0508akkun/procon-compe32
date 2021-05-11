# pragma once
# include "Color.h"
# include <string>

namespace box
{
    static const std::string check = "[" + Color::CharMagenta + "v" + Color::Reset + "] ";
    static const std::string success = "[" + Color::CharBlue + "o" + Color::Reset + "] ";
    static const std::string failure = "[" + Color::CharRed + "x" + Color::Reset + "] ";
    static const std::string disp = "[" + Color::CharGreen + "*" + Color::Reset + "] ";
    static const std::string input = "[" + Color::CharGreen + ">" + Color::Reset + "] ";
    static const std::string hint = "[" + Color::CharYellow + "?" + Color::Reset + "] ";
}
