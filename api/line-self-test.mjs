const LINE_CHANNEL_ACCESS_TOKEN = process.env.LINE_CHANNEL_ACCESS_TOKEN || "";
const LINE_GROUP_ID = process.env.LINE_GROUP_ID || "";
const SELF_TEST_KEY = process.env.LINE_SELF_TEST_KEY || "bridge-test-20260707";

function json(res, status, body) {
  res.status(status).json(body);
}

export default async function handler(req, res) {
  try {
    if (req.method !== "GET" && req.method !== "POST") {
      json(res, 405, { ok: false, error: "method_not_allowed" });
      return;
    }

    const key = req.query?.key || req.body?.key;
    if (SELF_TEST_KEY && key !== SELF_TEST_KEY) {
      json(res, 401, { ok: false, error: "invalid_key" });
      return;
    }

    const to = req.query?.to || req.body?.to || LINE_GROUP_ID;
    const text = `LINE self-test ${new Date().toISOString()}`;

    const config = {
      hasToken: Boolean(LINE_CHANNEL_ACCESS_TOKEN),
      hasGroupId: Boolean(to),
      toPrefix: to ? String(to).slice(0, 8) : null
    };

    if (!LINE_CHANNEL_ACCESS_TOKEN) {
      json(res, 500, { ok: false, error: "LINE_CHANNEL_ACCESS_TOKEN_missing", config });
      return;
    }
    if (!to) {
      json(res, 500, { ok: false, error: "LINE_GROUP_ID_missing", config });
      return;
    }

    const lineRes = await fetch("https://api.line.me/v2/bot/message/push", {
      method: "POST",
      headers: {
        authorization: `Bearer ${LINE_CHANNEL_ACCESS_TOKEN}`,
        "content-type": "application/json"
      },
      body: JSON.stringify({
        to,
        messages: [{ type: "text", text }]
      })
    });

    const bodyText = await lineRes.text();
    let lineBody = {};
    try {
      lineBody = bodyText ? JSON.parse(bodyText) : {};
    } catch {
      lineBody = { raw: bodyText };
    }

    json(res, lineRes.ok ? 200 : 502, {
      ok: lineRes.ok,
      lineStatus: lineRes.status,
      lineBody,
      sentText: text,
      config
    });
  } catch (error) {
    json(res, 500, { ok: false, error: error?.message || "internal_error" });
  }
}
