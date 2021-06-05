# include "AccessServer.h"
# include "Utility/Box.h"
# include <cpprest/http_client.h>
# include <cpprest/filestream.h>
# include "picojson/picojson.h"

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;
using namespace std;

bool CheckStatus(const Response& res)
{
    switch (res.status_code)
    {
        case 200:
        case 201:
            cout << box::success << res.status_code << " Success!" << endl;
            return true;
        case 401:
            cout << box::failure << res.status_code << " InvalidToken" << endl;
            cout << "    - トークンが間違っているもしくは存在しない" << endl;
            return false;

        default:
            return false;
    }
}

bool CheckStatus(const Response& res, int matchID)
{
    string matchIDstr = " (matchID: " + to_string(matchID) + ")";  //matchIDをmatchIDstrに代入
    switch (res.status_code)
    {
        case 200:
        case 201:
            cout << box::success << res.status_code << " Success!" << matchIDstr << endl;
            return true;
        case 400:
            if (res.body == "InvalidMatches")
            {
                cout << box::failure << res.status_code << " InvalidMatches" << matchIDstr << endl;
                cout << "    - 参加していない試合へのリクエスト" << endl;
                return false;
            }
            else if (res.body == "TooEarly")
            {
                cout << box::failure << res.status_code << " TooEarly" << matchIDstr << endl;
                cout << "    - 試合の開始前にアクセス" << endl;
                return false;
            }
            return false;
        case 401:
            cout << box::failure << res.status_code << " InvalidToken" << matchIDstr << endl;
            cout << "    - トークンが間違っているもしくは存在しない" << endl;
            return false;

        default:
            return false;
        }
}

Response::Response(int _status_code, std::string _body)
    : status_code(_status_code)
    , body(_body)
{}

Response Get(string url)
{
    int status_code;
    string body;
    pplx::task<void> requestTask = pplx::create_task([&]
    {
        // Create http_client to send the request.
        http_client client(url);

        http_request request(methods::GET);
        
        request.headers().add(U("Authorization"), U("0649e322073a67bd823a56861b88c89d84ab4a2544cf37f78ec718b1bbf4dc56"));

        return client.request(request);
    })
    .then([&](http_response response)
    {
        status_code = response.status_code();
        body = response.extract_string().get();
    });

    requestTask.wait();
    return Response(status_code, body);
}

Response Post(string url, string content_type, string post_data)
{
    int status_code;
    string body;
    pplx::task<void> requestTask = pplx::create_task([&]
    {
        // Create http_client to send the request.
        http_client client(url);

        http_request request(methods::POST);
        request.headers().add(U("Authorization"), U("0649e322073a67bd823a56861b88c89d84ab4a2544cf37f78ec718b1bbf4dc56"));
        request.headers().add(U("Content-Type"), content_type);

        request.set_body(post_data);

        return client.request(request);
    })
    .then([&](http_response response)
    {
        status_code = response.status_code();
        body = response.extract_string().get();
    });

    requestTask.wait();
    return Response(status_code, body);
}

Response RunPing(std::string& urlInfo)
{
    return Get(urlInfo + "/ping");
}

Response GetMatches(std::string& urlInfo)
{
    return Get(urlInfo + "/matches");
}

Response GetStatus(int id,std::string& urlInfo)
{
    Response res = Get(urlInfo + "/matches/" + to_string(id));
    
    picojson::value val;
    picojson::parse(val, res.body);
    picojson::object& obj = val.get<picojson::object>();

    if (obj["status"].is<string>())  //チェックする
    {
        res.body = obj["status"].get<string>();
    }

    return res;
}

Response PostActions(const string& actions_json, int matchID,std::string& urlInfo)
{
    Response res = Post(urlInfo + "/matches/" + to_string(matchID) + "/action", U("application/json"), actions_json);

    picojson::value val;
    picojson::parse(val, res.body);
    picojson::object& obj = val.get<picojson::object>();

    if (obj["status"].is<string>())
    {
        res.body = obj["status"].get<string>();
    }

    return res;
}
