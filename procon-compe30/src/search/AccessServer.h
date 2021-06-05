# pragma <once>
# include "Action.h"
# include <bits/stdc++.h>

struct Response {
    Response() = default;
    Response(int _status_code, std::string body);
    int status_code;
    std::string body;
};

bool CheckStatus(const Response& res);
bool CheckStatus(const Response& res, int matchID);

Response RunPing(std::string& urlInfo);
Response GetMatches(std::string& urlInfo);
Response GetStatus(int id,std::string& urlInfo);
Response PostActions(const std::string& actions_json, int matchID,std::string& urlInfo);
