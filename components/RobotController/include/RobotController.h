#include "WiFiStation.h"
#include "HTTPServer.h"
#include <string>
#include <functional>
#include <unordered_map>

class RobotController
{
public:
    enum class Key
    {
        ARROW_UP,
        ARROW_DOWN,
        ARROW_LEFT,
        ARROW_RIGHT,
        L
    };

    enum class KeyEvent
    {
        DOWN,
        UP
    };

    using KeyEventHandler = std::function<void()>;

    RobotController(const std::string &ssid, const std::string &password);

    static void registerKeyEventHandler(const Key &key, const KeyEvent &event, KeyEventHandler handler);
private:
    struct KeyEventHandlerContext
    {
        KeyEventHandler handler;
    };

    static WiFiStation station;
    static HTTPServer server;
    static std::string indexPage;
    static std::unordered_map<std::string, KeyEventHandlerContext> contexts;
};
