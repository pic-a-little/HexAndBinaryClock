#include "myWiFiSetting.h"
#include "myDisp.h"

const char *settings = "/wifi_settings.txt";
// AP mode WiFi Setting
const char *defaultWifiMode = "APMode";
const char *defaultSsid = "HexClock";
const char *defaultEssKey = "Hex969969";
String wifiMode;
String ssid;
String essKey;
String channel;
WebServer server(80);

bool connectionSuccess = false;

void funcHandleRoot()
{
    char temp[500];
    snprintf(temp, 500, "<html><head><title>HexClock WiFiSetting</title><style>body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style></head><body><h1> HexClock WiFiSetting</h1><br><hr> <a href=\"/wifiInput\">wifi settings</a> </body></html>");
    server.send(200, "text/html", temp);
}

void funcWifiInput()
{
    String html = "";
    html += "<h1>WiFi Settings</h1>";
    html += "<form method='post' action='/wifiSet'>";
    html += "  <br>";
    html += "  <input type='text' name='essid' >ESSID<br>";
    html += "  <input type='text' name='key' >KEY(pass)<br>";
    html += "  <br>";
    html += "  <input type='submit'><br>";
    html += "</form>";
    server.send(200, "text/html", html);
}

void funcWifiSet()
{
    String wifiMode = "clientMode";
    String setSsid = server.arg("essid");
    String setKey = server.arg("key");
    String setChannel = server.arg("0");

    File f = SPIFFS.open(settings, "w");
    f.println(wifiMode);
    f.println(setSsid);
    f.println(setKey);
    f.println(channel);
    f.close();

    String html = "";
    html += "<h1>WiFi Settings</h1>";
    html += "ESSID:" + setSsid + "<br>";
    html += "key(pass):" + setKey + "<br>";
    html += "<hr>";
    html += "<h1>Please Reset!</h1>";

    server.send(200, "text/html", html);
}

void funcHandleNotFound()
{
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    server.send(404, "text/plain", message);
}

void getSPIFFSSetting()
{
    //   init filesystem
    bool res = SPIFFS.begin(true); // FORMAT_SPIFFS_IF_FAILED
    if (!res)
    {
        Serial.println("SPIFFS.begin fail");
    }
    // set file read
    File fp = SPIFFS.open(settings, "r");
    if (!fp)
    {
        Serial.println("open error");
    }
    wifiMode = fp.readStringUntil('\n'); // always "clientMode"
    ssid = fp.readStringUntil('\n');
    essKey = fp.readStringUntil('\n');
    channel = fp.readStringUntil('\n'); // no use
    Serial.print("wifiMode:");
    Serial.println(wifiMode);
    Serial.print("ssid:");
    Serial.println(ssid);
    Serial.print("essKey:");
    Serial.println(essKey);
    fp.close();
    wifiMode.trim();
    ssid.trim();
    essKey.trim();
    channel.trim();

    //ファイル破損時のリカバリ
    if (!(wifiMode.compareTo("clientMode") == 0 || wifiMode.compareTo("APMode") == 0))
    {
        Serial.print("SPIFFS data seems clash. Default load...");
        wifiMode = defaultWifiMode;
        ssid = defaultSsid;
        essKey = defaultEssKey;
        File f = SPIFFS.open(settings, "w");
        f.println(wifiMode);
        f.println(ssid);
        f.println(essKey);
        f.close();
    }
}

void setClientMode(int *WiFiMode)
{
    Serial.println("WiFiMode is ON");
    //----------WiFi access point---------
    Serial.println();
    Serial.print("Configuring access point...");
    /* You can remove the password parameter if you want the AP to be open. */
    if (wifiMode.compareTo("APMode") == 0)
    {
        setAccessPointMode();
        *WiFiMode = APMODE;
    }
    //----- WiFi client --------------
    if (wifiMode.compareTo("clientMode") == 0)
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), essKey.c_str());
        Serial.println("");
        // Wait for connection
        int i;
        for (i = 0; i < 8; i++) //4000msまでは待つ
        {
            if (WiFi.status() != WL_CONNECTED)
            {
                delay(500);
                Serial.print(".");
                // dispModule.clearDisplay();
                dispModule.setupDisplay(true, 7); // sometimes noise on the line may change the intensity level
                dispModule.setDisplayToString("wait   # ");
                // dispModule.setSegments(swapbit(0), 6);
                // dispModule.setSegments(swapbit(255), 7);
                // dispModule.setSegments(swapbit(0), 8);
            }
        }

        if (WiFi.status() != WL_CONNECTED)
        {
            connectionSuccess = false;
            // dispModule.clearDisplay();
            dispModule.setDisplayToString("nw-Err");
            delay(2000);
        }
        else
        {
            connectionSuccess = true;
            Serial.println("");
            Serial.print("Connected to ");
            Serial.println(ssid);
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());

            setArduinoOTA();
        }
    }
}

void setArduinoOTA()
{
    ArduinoOTA
        .onStart([]() {
            String type;
            if (ArduinoOTA.getCommand() == U_FLASH)
                type = "sketch";
            else // U_SPIFFS
                type = "filesystem";

            // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
            Serial.println("Start updating " + type);
        })
        .onEnd([]() {
            Serial.println("\nEnd");
        })
        .onProgress([](unsigned int progress, unsigned int total) {
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        })
        .onError([](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR)
                Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR)
                Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR)
                Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR)
                Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR)
                Serial.println("End Failed");
        });

    ArduinoOTA.begin();
    Serial.println("Done setupOTA");
}

void setAccessPointMode()
{
    Serial.println("WiFiMode default boot");
    //----------WiFi access point---------
    Serial.println();
    Serial.print("Configuring access point...");
    /* You can remove the password parameter if you want the AP to be open. */
    WiFi.softAP(defaultSsid, defaultEssKey);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    server.on("/", funcHandleRoot);
    server.on("/wifiInput", funcWifiInput);
    server.on("/wifiSet", funcWifiSet);
    server.onNotFound(funcHandleNotFound);
    server.begin();
    Serial.println("HTTP server started");
}
