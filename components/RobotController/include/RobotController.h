#include "WiFiStation.h"
#include "HTTPServer.h"
#include "CameraServer.h"
#include "nvs_flash.h"
#include <string>
#include <functional>
#include <unordered_map>

class RobotController
{
public:
    // Keys supported by Robot Controller
    enum class Key
    {
        ARROW_UP,
        ARROW_DOWN,
        ARROW_LEFT,
        ARROW_RIGHT,
        L
    };

    // Events occuring during key state change
    enum class KeyEvent
    {
        DOWN,   // Key pressed
        UP      // Key released
    };

    // Functions used to handle key events
    using KeyEventHandler = std::function<void()>;

    /*
    Constructor: initializes and configures Wi-Fi station and two HTTP servers(including one camera server)

    INPUT: SSID and password

    REMARKS: operates on IP: 192.168.0.100, port: 80(HTTP server), port: 81(HTTP camera server)
    */
    RobotController(const std::string &ssid, const std::string &password);

    /*
    Registers functions to handle key events

    INPUT: key, event, handler
    */
    static void registerKeyEventHandler(const Key &key, const KeyEvent &event, KeyEventHandler handler);
private:
    // Context used to store registered handlers in unordered map
    struct KeyEventHandlerContext
    {
        KeyEventHandler handler;
    };

    static WiFiStation station;
    static HTTPServer server;
    static CameraServer cameraServer;
    static std::string indexPage; // Main HTML index page
    static std::unordered_map<std::string, KeyEventHandlerContext> contexts; // Storage for registered handlers
};
