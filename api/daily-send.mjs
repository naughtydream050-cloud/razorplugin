const LINE_CHANNEL_ACCESS_TOKEN = process.env.LINE_CHANNEL_ACCESS_TOKEN || "";
const LINE_GROUP_ID = process.env.LINE_GROUP_ID || "";
const LINE_DRY_RUN = (process.env.LINE_DRY_RUN || "true").toLowerCase() !== "false";
const CRON_SECRET = process.env.CRON_SECRET || "";

function getHeader(req, name) {
  const value = req.headers[name.toLowerCase()];
  return Array.isArray(value) ? value[0] : value;
}

function verifyCron(req) {
  if (!CRON_SECRET) return true;
  return getHeader(req, "authorization") === `Bearer ${CRON_SECRET}`;
}

function publicBaseUrl(req) {
  const host = getHeader(req, "x-forwarded-host") || getHeader(req, "host");
  const proto = getHeader(req, "x-forwarded-proto") || "https";
  return `${proto}://${host}`;
}

async function sendToLine(messages) {
  if (LINE_DRY_RUN) {
    return {
      dryRun: true,
      to: LINE_GROUP_ID || "(LINE_GROUP_ID not set)",
      messages,
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
      "content-type": "application/json",
    },
    body: JSON.stringify({
      to: LINE_GROUP_ID,
      messages,
    }),
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
    body: text ? JSON.parse(text) : {},
  };
}

export default async function handler(req, res) {
  try {
    if (req.method !== "GET" && req.method !== "POST") {
      res.status(405).json({ ok: false, error: "method_not_allowed" });
      return;
    }

    if (!verifyCron(req)) {
      res.status(401).json({ ok: false, error: "invalid_cron_auth" });
      return;
    }

    const baseUrl = publicBaseUrl(req);
    const imageUrl = `${baseUrl}/api/today-image`;
    const result = await sendToLine([
      {
        type: "image",
        originalContentUrl: imageUrl,
        previewImageUrl: imageUrl,
      },
      {
        type: "text",
        text: "今日の画像これで",
      },
    ]);

    res.status(200).json({
      ok: true,
      imageUrl,
      line: result,
    });
  } catch (error) {
    res.status(error.statusCode || 500).json({
      ok: false,
      error: error.message || "internal_error",
    });
  }
}
