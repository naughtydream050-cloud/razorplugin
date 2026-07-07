# LINE Vercel Bridge

3Min APIから受け取ったpayloadをLINE Messaging APIへ送る中継サーバーです。

## Vercel Environment Variables

VercelのProject Settings > Environment Variables に以下を設定してください。

```text
LINE_CHANNEL_ACCESS_TOKEN=LINEのチャネルアクセストークン
LINE_GROUP_ID=C3e5f0758b7ab654960c51c9e54f83766
LINE_DRY_RUN=false
WEBHOOK_AUTH_VALUE=Bearer bridge-test-20260707
```

`WEBHOOK_AUTH_VALUE` は3MinからこのVercel APIを呼ぶための合言葉です。LINEのトークンではありません。

## Endpoints

```text
GET /api/health
POST /api/3min-line-webhook
```

## 3Min API Webhook Settings

3Min API側のWebhook設定を以下に変更します。

```text
Webhook URL:
https://YOUR-VERCEL-URL.vercel.app/api/3min-line-webhook

認証ヘッダー:
Authorization

認証値:
Bearer bridge-test-20260707
```

## Text Test Payload

```json
{
  "text": "Vercel bridgeからLINE送信テストです"
}
```

## Image Payload

```json
{
  "image_url": "https://example.com/image.png",
  "preview_image_url": "https://example.com/image.png",
  "caption": "今日の画像これで"
}
```

LINEの画像メッセージは、HTTPSで公開されたJPEG/PNG URLが必要です。
