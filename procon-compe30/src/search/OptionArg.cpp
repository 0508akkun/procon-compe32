# include "OptionArg.h"
using namespace std;

cmdline::parser OptionArg::m_parser;

void OptionArg::Register(const string& name, char short_name, const string& description)
{
    m_parser.add(name, short_name, description);
}

void OptionArg::ParseCheck(int argc, char* argv[])
{
    m_parser.parse_check(argc, argv);
}

string OptionArg::GetURLInfo()
{
    string port_num = to_string(Get<int>("portnum"));
    string ip_address = Get<string>("address");
    return "http://" + ip_address + ":" + port_num;
}

bool OptionArg::Exist(const std::string& name)
{
    return m_parser.exist(name);
}

std::string OptionArg::Usage()
{
    return m_parser.usage();
}

