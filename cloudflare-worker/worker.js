/**
 * Cloudflare Worker — Telegram Bot API Proxy
 *
 * Deploy this worker to route your Arduino/ESP device's Telegram Bot API
 * requests through Cloudflare instead of connecting to api.telegram.org directly.
 *
 * Useful when:
 *  - api.telegram.org is blocked in your region
 *  - You want to route traffic through a custom domain
 *
 * Deploy steps:
 *  1. Go to https://dash.cloudflare.com and open "Workers & Pages"
 *  2. Create a new Worker and paste this script
 *  3. Deploy — your worker URL will be something like:
 *       https://my-bot.yourname.workers.dev
 *  4. In your Arduino sketch set:
 *       bot.proxyHost = "my-bot.yourname.workers.dev";
 *       bot.proxyPort = 443;
 *
 * Optional: bind a custom domain to the worker in the CF dashboard.
 */

export default {
  async fetch(request) {
    const url = new URL(request.url);
    url.hostname = "api.telegram.org";
    url.port = "443";
    url.protocol = "https:";

    const proxyRequest = new Request(url.toString(), {
      method: request.method,
      headers: request.headers,
      body: request.body,
      redirect: "follow",
    });

    return fetch(proxyRequest);
  },
};
