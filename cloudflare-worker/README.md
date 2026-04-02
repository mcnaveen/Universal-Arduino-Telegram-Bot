# Telegram Bot API — Cloudflare Worker Proxy

This worker proxies all Telegram Bot API requests from your Arduino/ESP device
through Cloudflare, letting you reach `api.telegram.org` even in restricted regions.

## Deploy

Click the button below to deploy the worker instantly to your Cloudflare account:

[![Deploy to Cloudflare Workers](https://deploy.workers.cloudflare.com/button)](https://deploy.workers.cloudflare.com/?url=https://github.com/mcnaveen/universal-arduino-telegram-bot/tree/claude/add-telegram-proxy-DWUco/cloudflare-worker)

> You will need a free [Cloudflare account](https://dash.cloudflare.com/sign-up).

## Manual deploy

1. Go to [Cloudflare Dashboard → Workers & Pages](https://dash.cloudflare.com/?to=/:account/workers-and-pages/create)
2. Click **Create Application → Create Worker**
3. Paste the contents of `worker.js` and click **Deploy**
4. Copy your worker URL (e.g. `my-bot.yourname.workers.dev`)

## Arduino usage

```cpp
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

bot.proxyHost = "my-bot.yourname.workers.dev";
bot.proxyPort = 443;
secured_client.setInsecure(); // or pin your proxy's CA cert
```

See `examples/ESP32/CloudflareProxy/CloudflareProxy.ino` for a full sketch.

## How it works

```
ESP32/ESP8266  →  HTTPS  →  Cloudflare Worker  →  api.telegram.org
```

The worker receives your HTTPS request and forwards it transparently to
`api.telegram.org`, then returns the response to your device.
The library connects to the worker host instead of `api.telegram.org` directly —
no other code changes required.
