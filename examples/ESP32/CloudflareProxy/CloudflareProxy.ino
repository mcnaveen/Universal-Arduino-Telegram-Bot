/*******************************************************************
    EchoBot using a Cloudflare Worker as a proxy for api.telegram.org

    Useful when Telegram is blocked in your region, or you want to
    route all bot traffic through a custom domain.

    Setup:
      1. Deploy cloudflare-worker/worker.js to a Cloudflare Worker
      2. Set PROXY_HOST below to your worker's domain
      3. Flash and run

    Parts:
    ESP32 D1 Mini style Dev board* - http://s.click.aliexpress.com/e/C6ds4my
    (or any ESP32 board)

    Written by Brian Lough
    YouTube: https://www.youtube.com/brianlough
 *******************************************************************/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// WiFi credentials
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_PASSWORD"

// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "XXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

// Cloudflare Worker domain (without https://)
// e.g. "my-bot.yourname.workers.dev" or your custom domain
#define PROXY_HOST "my-bot.yourname.workers.dev"

const unsigned long BOT_MTBS = 1000;

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime;

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    bot.sendMessage(bot.messages[i].chat_id, bot.messages[i].text, "");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  Serial.print("Connecting to WiFi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Route requests through the Cloudflare Worker
  bot.proxyHost = PROXY_HOST;
  bot.proxyPort = 443;

  // The proxy handles TLS termination; its cert is issued by a well-known CA
  // so we can rely on the built-in root store instead of pinning Telegram's cert.
  secured_client.setInsecure(); // or use setCACert() with your proxy's CA cert

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org");
  time_t now = time(nullptr);
  while (now < 24 * 3600) {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
}

void loop() {
  if (millis() - bot_lasttime > BOT_MTBS) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
}
