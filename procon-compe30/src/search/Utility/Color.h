# pragma once
# include <string>

namespace Color
{
    const std::string Reset       = "\e[0m";

    const std::string CharBlack   = "\e[30m"; 
    const std::string CharRed     = "\e[31m"; 
    const std::string CharGreen   = "\e[32m"; 
    const std::string CharYellow  = "\e[33m"; 
    const std::string CharBlue    = "\e[34m"; 
    const std::string CharMagenta = "\e[35m"; 
    const std::string CharCyan    = "\e[36m"; 
    const std::string CharWhite   = "\e[37m"; 

    const std::string BackBlack   = "\e[40m"; 
    const std::string BackRed     = "\e[41m"; 
    const std::string BackGreen   = "\e[42m"; 
    const std::string BackYellow  = "\e[44m"; 
    const std::string BackBlue    = "\e[44m"; 
    const std::string BackMagenta = "\e[45m"; 
    const std::string BackCyan    = "\e[46m"; 
    const std::string BackWhite   = "\e[47m"; 
}

