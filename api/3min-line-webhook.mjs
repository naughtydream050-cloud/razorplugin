const WEBHOOK_AUTH_HEADER = process.env.WEBHOOK_AUTH_HEADER || "Authorization";
const WEBHOOK_AUTH_VALUE = process.env.WEBHOOK_AUTH_VALUE || "";
const LINE_CHANNEL_ACCESS_TOKEN = process.env.LINE_CHANNEL_ACCESS_TOKEN || "";
const LINE_GROUP_ID = process.env.LINE_GROUP_ID || "";
const LINE_DRY_RUN = (process.env.LINE_DRY_RUN || "true").toLowerCase() !== "false";

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

function buildLineMessages(payload) {
  if (Array.isArray(payload.messages) && payload.messages.length > 0) {
    return payload.messages;
  }

  const imageUrl = payload.image_url || payload.imageUrl || payload.originalContentUrl;
  const previewUrl = payload.preview_image_url || payload.previewImageUrl || imageUrl;
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

    const error = new Error("Missing image_url in webhook payload.");
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
  if (LINE_DRY_RUN) {
    return {
      dryRun: true,
      to: LINE_GROUP_ID || "(LINE_GROUP_ID not set)",
      messages
    };
  }

  if (!LINE_CHANNEL_ACCESS_TOKEN) {
    const error = new Error("LINE_CHANNEL_ACCESS_TOKEN is required when LINE_DRY_RUN=false.");
    error.statusCode = 500;
    throw error;
  }

  if (!LINE_GROUP_ID) {
    const error = new Error("LINE_GROUP_ID is required when LINE_DRY_RUN=false.");
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
  if (!response.ok) {
    const error = new Error(`LINE push failed: ${response.status} ${text}`);
    error.statusCode = 502;
    throw error;
  }

  return {
    dryRun: false,
    status: response.status,
    body: text ? JSON.parse(text) : {}
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
