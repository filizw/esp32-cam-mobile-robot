#include "WiFiStation.h"
#include "HTTPServer.h"
#include <string>

class RobotController
{
public:
    RobotController(const std::string &ssid, const std::string &password);
private:
    WiFiStation station;
    HTTPServer server;
    
    static std::string indexPage;
};
