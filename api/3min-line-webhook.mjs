const LINE_CHANNEL_ACCESS_TOKEN = process.env.LINE_CHANNEL_ACCESS_TOKEN || "";
const LINE_GROUP_ID = process.env.LINE_GROUP_ID || "";
const LINE_DRY_RUN = (process.env.LINE_DRY_RUN || "false").toLowerCase() === "true";

function unwrapPayload(body) {
  return body?.payload || body?.record?.payload || body?.data?.payload || body || {};
}

function assertHttpsUrl(value, fieldName) {
  if (!value) return null;
  const text = String(value);
  if (!/^https:\/\//i.test(text)) {
    const error = new Error(`${fieldName} must be an https URL.`);
    error.statusCode = 400;
    throw error;
  }
  return text;
}

function buildMessages(payload, req) {
  const host = req.headers["x-forwarded-host"] || req.headers.host || "razorplugin.vercel.app";
  const proto = req.headers["x-forwarded-proto"] || "https";
  const defaultImageUrl = `${proto}://${host}/api/poster?v=${Date.now()}`;

  const imageUrl = assertHttpsUrl(
    payload.image_url || payload.imageUrl || payload.originalContentUrl || defaultImageUrl,
    "image_url"
  );
  const previewUrl = assertHttpsUrl(
    payload.preview_image_url || payload.previewImageUrl || imageUrl,
    "preview_image_url"
  );
  const caption = payload.caption || payload.text || "今日のダンス上達Tier表です。保存して10分だけやってみてください。";

  const messages = [];
  if (imageUrl) {
    messages.push({
      type: "image",
      originalContentUrl: imageUrl,
      previewImageUrl: previewUrl
    });
  }
  if (caption) {
    messages.push({
      type: "text",
      text: String(caption).slice(0, 5000)
    });
  }
  return messages;
}

async function sendToLine(messages, to) {
  const config = {
    dryRun: LINE_DRY_RUN,
    hasToken: Boolean(LINE_CHANNEL_ACCESS_TOKEN),
    hasGroupId: Boolean(to),
    toPrefix: to ? String(to).slice(0, 8) : null
  };

  if (LINE_DRY_RUN) {
    const error = new Error(`LINE_DRY_RUN is true. Not sending to LINE. Config: ${JSON.stringify(config)}`);
    error.statusCode = 500;
    throw error;
  }
  if (!LINE_CHANNEL_ACCESS_TOKEN) {
    const error = new Error(`LINE_CHANNEL_ACCESS_TOKEN is missing. Config: ${JSON.stringify(config)}`);
    error.statusCode = 500;
    throw error;
  }
  if (!to) {
    const error = new Error(`LINE_GROUP_ID is missing. Config: ${JSON.stringify(config)}`);
    error.statusCode = 500;
    throw error;
  }

  const response = await fetch("https://api.line.me/v2/bot/message/push", {
    method: "POST",
    headers: {
      authorization: `Bearer ${LINE_CHANNEL_ACCESS_TOKEN}`,
      "content-type": "application/json"
    },
    body: JSON.stringify({ to, messages })
  });

  const text = await response.text();
  let parsed = {};
  try {
    parsed = text ? JSON.parse(text) : {};
  } catch {
    parsed = { raw: text };
  }

  if (!response.ok) {
    const error = new Error(`LINE push failed: ${response.status} ${text}`);
    error.statusCode = 502;
    throw error;
  }

  return { status: response.status, body: parsed, config };
}

export default async function handler(req, res) {
  try {
    if (req.method !== "POST") {
      res.status(405).json({ ok: false, error: "method_not_allowed" });
      return;
    }

    const payload = unwrapPayload(req.body || {});
    const to = payload.group_id || payload.groupId || payload.to || LINE_GROUP_ID;
    const messages = Array.isArray(payload.messages) && payload.messages.length > 0
      ? payload.messages
      : buildMessages(payload, req);
    const result = await sendToLine(messages, to);

    res.status(200).json({ ok: true, line: result });
  } catch (error) {
    res.status(error.statusCode || 500).json({ ok: false, error: error.message || "internal_error" });
  }
}
