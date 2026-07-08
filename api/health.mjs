export default function handler(req, res) {
  const lineDryRun = (process.env.LINE_DRY_RUN || "false").toLowerCase() === "true";
  const groupId = process.env.LINE_GROUP_ID || "";

  res.status(200).json({
    ok: true,
    service: "daily-line-image-vercel-bridge",
    line: {
      dryRun: lineDryRun,
      hasToken: Boolean(process.env.LINE_CHANNEL_ACCESS_TOKEN),
      hasGroupId: Boolean(groupId),
      groupIdPrefix: groupId ? groupId.slice(0, 6) : null
    },
    webhook: {
      hasAuthValue: Boolean(process.env.WEBHOOK_AUTH_VALUE),
      authHeader: process.env.WEBHOOK_AUTH_HEADER || "Authorization"
    }
  });
}
