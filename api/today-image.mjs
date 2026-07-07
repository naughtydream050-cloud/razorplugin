import React from "react";
import { ImageResponse } from "@vercel/og";

export const config = {
  runtime: "edge"
};

const e = React.createElement;

const tiers = [
  {
    rank: "SSS",
    title: "音を細かく聞く",
    body: "ビートだけでなく、ハイハット・ベース・声の切れ目で動きを変える。上手く見える人は音の粒に反応している。"
  },
  {
    rank: "SS",
    title: "毎日10分のアイソレ",
    body: "首・胸・腰を各1分。鏡の前で小さく正確に。派手な技より、体の分離が一番伸びる。"
  },
  {
    rank: "S",
    title: "動画は0.5倍で見る",
    body: "振付を覚える時は速度を落とす。足、胸、腕の順で分解すると覚えるスピードが上がる。"
  },
  {
    rank: "A",
    title: "重心を低く保つ",
    body: "膝を軽く曲げて、体重移動を見せる。リズムが安定して、動きに余裕が出る。"
  },
  {
    rank: "B",
    title: "スマホを床に置いて撮る",
    body: "真正面だけでなく低い角度で撮る。姿勢の崩れ、手足の遅れ、ノリの弱さが見えやすい。"
  },
  {
    rank: "C",
    title: "10分メニュー",
    body: "1分ストレッチ、3分アイソレ、3分ステップ、2分振付、1分録画チェック。短く毎日。"
  }
];

function TierRow({ rank, title, body, index }) {
  const rankColors = ["#0f0b08", "#17110c", "#2b1a0d", "#5b2a13", "#7d4a1b", "#8d6a31"];
  return e(
    "div",
    {
      style: {
        display: "flex",
        width: "100%",
        minHeight: 214,
        marginTop: index === 0 ? 0 : 16,
        border: "4px solid #8c6a31",
        background: "#f6ead0"
      }
    },
    e(
      "div",
      {
        style: {
          width: 212,
          display: "flex",
          alignItems: "center",
          justifyContent: "center",
          background: rankColors[index] || "#1b130c",
          color: "#f8d77a",
          fontSize: rank.length === 3 ? 58 : 72,
          fontWeight: 900,
          letterSpacing: 2,
          borderRight: "4px solid #8c6a31"
        }
      },
      rank
    ),
    e(
      "div",
      {
        style: {
          flex: 1,
          display: "flex",
          flexDirection: "column",
          justifyContent: "center",
          padding: "24px 28px",
          color: "#23180f"
        }
      },
      e("div", { style: { fontSize: 48, fontWeight: 900, marginBottom: 10 } }, title),
      e("div", { style: { fontSize: 29, lineHeight: 1.35, fontWeight: 700 } }, body)
    )
  );
}

export default function handler() {
  const today = new Intl.DateTimeFormat("ja-JP", {
    timeZone: "Asia/Tokyo",
    year: "numeric",
    month: "2-digit",
    day: "2-digit"
  }).format(new Date());

  return new ImageResponse(
    e(
      "div",
      {
        style: {
          width: "100%",
          height: "100%",
          display: "flex",
          flexDirection: "column",
          padding: 56,
          background: "linear-gradient(180deg, #fbf1da 0%, #ead7ae 100%)",
          color: "#21170f",
          fontFamily: "Noto Sans JP, sans-serif"
        }
      },
      e("div", { style: { display: "flex", justifyContent: "center", fontSize: 34, color: "#73501f", marginBottom: 10 } }, "DAILY DANCE HACK"),
      e(
        "div",
        {
          style: {
            display: "flex",
            justifyContent: "center",
            alignItems: "center",
            textAlign: "center",
            fontSize: 74,
            fontWeight: 900,
            lineHeight: 1.12,
            padding: "18px 20px",
            borderTop: "5px solid #8c6a31",
            borderBottom: "5px solid #8c6a31"
          }
        },
        "ダンスが上手くなるコツ Tier表"
      ),
      e("div", { style: { display: "flex", justifyContent: "center", marginTop: 14, marginBottom: 26, fontSize: 28, fontWeight: 700, color: "#6c4c22" } }, `${today} / 10分でできる練習とライフハック`),
      e(
        "div",
        { style: { display: "flex", flexDirection: "column", flex: 1 } },
        tiers.map((tier, index) => e(TierRow, { ...tier, index, key: tier.rank }))
      ),
      e("div", { style: { display: "flex", justifyContent: "center", fontSize: 23, fontWeight: 700, color: "#6d5428", marginTop: 24 } }, "保存して今日10分だけやる。上達は短くても毎日の勝ち。")
    ),
    {
      width: 1080,
      height: 1920,
      headers: {
        "Cache-Control": "public, max-age=60, s-maxage=60",
        "Content-Type": "image/png"
      }
    }
  );
}
