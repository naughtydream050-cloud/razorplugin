const WEBHOOK_AUTH_HEADER = process.env.WEBHOOK_AUTH_HEADER || "Authorization";
const WEBHOOK_AUTH_VALUE = process.env.WEBHOOK_AUTH_VALUE || "";
const LINE_CHANNEL_ACCESS_TOKEN = process.env.LINE_CHANNEL_ACCESS_TOKEN || "";
const LINE_GROUP_ID = process.env.LINE_GROUP_ID || "";
// Default must be false for production. If this is true, Vercel accepts requests but does not send to LINE.
const LINE_DRY_RUN = (process.env.LINE_DRY_RUN || "false").toLowerCase() === "true";

function getHeader(req, name) {
  const value = req.headers[name.toLowerCase()];
  return Array.isArray(value) ? value[0] : value;
}

function verifyWebhookAuth(req) {
  if (!WEBHOOK_AUTH_VALUE) return true;
  return getHeader(req, WEBHOOK_AUTH_HEADER) === WEBHOOK_AUTH_VALUE;
}

function unwrapPayload(body) {
  return body?.payload || body?.record?.payload || body?.data?.payload || body;
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

function buildLineMessages(payload) {
  if (Array.isArray(payload.messages) && payload.messages.length > 0) {
    return payload.messages;
  }

  const imageUrl = assertHttpsUrl(
    payload.image_url || payload.imageUrl || payload.originalContentUrl,
    "image_url"
  );
  const previewUrl = assertHttpsUrl(
    payload.preview_image_url || payload.previewImageUrl || imageUrl,
    "preview_image_url"
  );
  const caption = payload.caption || payload.text || "";

  if (!imageUrl) {
    if (caption) {
      return [
        {
          type: "text",
          text: String(caption).slice(0, 5000)
        }
      ];
    }

    const error = new Error("Missing image_url or caption in webhook payload.");
    error.statusCode = 400;
    throw error;
  }

  const messages = [
    {
      type: "image",
      originalContentUrl: imageUrl,
      previewImageUrl: previewUrl
    }
  ];

  if (caption) {
    messages.push({
      type: "text",
      text: String(caption).slice(0, 5000)
    });
  }

  return messages;
}

async function sendToLine(messages) {
  const config = {
    dryRun: LINE_DRY_RUN,
    hasToken: Boolean(LINE_CHANNEL_ACCESS_TOKEN),
    hasGroupId: Boolean(LINE_GROUP_ID),
    groupIdPrefix: LINE_GROUP_ID ? LINE_GROUP_ID.slice(0, 6) : null
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

  if (!LINE_GROUP_ID) {
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
    body: JSON.stringify({
      to: LINE_GROUP_ID,
      messages
    })
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

  return {
    dryRun: false,
    status: response.status,
    body: parsed,
    config
  };
}

export default async function handler(req, res) {
  try {
    if (req.method !== "POST") {
      res.status(405).json({ ok: false, error: "method_not_allowed" });
      return;
    }

    if (!verifyWebhookAuth(req)) {
      res.status(401).json({ ok: false, error: "invalid_webhook_auth" });
      return;
    }

    const payload = unwrapPayload(req.body || {});
    const messages = buildLineMessages(payload);
    const result = await sendToLine(messages);

    res.status(200).json({
      ok: true,
      accepted: {
        title: payload.title || null,
        date: payload.date || null,
        status: payload.status || null
      },
      line: result
    });
  } catch (error) {
    res.status(error.statusCode || 500).json({
      ok: false,
      error: error.message || "internal_error"
    });
  }
}
