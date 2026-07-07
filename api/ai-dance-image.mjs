import React from "react";
import { ImageResponse } from "@vercel/og";

export const config = {
  runtime: "edge",
};

const WIDTH = 720;
const HEIGHT = 1280;

const tiers = [
  {
    rank: "SSS",
    title: "音の粒を聞く",
    items: "ビートだけでなく、ハイハット・ベース・声の切れ目で動きを変える。上手い人は音の細部に反応している。",
    accent: "#0f0b08",
  },
  {
    rank: "SS",
    title: "毎日10分のアイソレ",
    items: "首・胸・腰を各1分。鏡の前で小さく正確に。派手な技より、体の分離が一番伸びる。",
    accent: "#17100b",
  },
  {
    rank: "S",
    title: "動画は0.5倍で見る",
    items: "振付を覚える時は速度を落とす。足、胸、腕、目線の順に分解すると覚えるスピードが上がる。",
    accent: "#271307",
  },
  {
    rank: "A",
    title: "重心を低く保つ",
    items: "膝を軽く曲げて、体重移動を見せる。リズムが安定して、動きに余裕が出る。",
    accent: "#5b260f",
  },
  {
    rank: "B",
    title: "スマホを床に置いて撮る",
    items: "低い角度で撮ると、姿勢の崩れ、手足の遅れ、ノリの弱さが見えやすい。",
    accent: "#87531d",
  },
  {
    rank: "C",
    title: "10分練習メニュー",
    items: "1分ストレッチ、3分アイソレ、3分ステップ、2分振付、1分録画チェック。短く毎日。",
    accent: "#9a7530",
  },
];

function Row({ tier }) {
  return React.createElement(
    "div",
    {
      style: {
        display: "flex",
        width: "100%",
        height: 142,
        border: "3px solid #8b6a30",
        background: "#f8edcf",
        boxShadow: "0 3px 0 #c7aa65",
      },
    },
    React.createElement(
      "div",
      {
        style: {
          width: 150,
          height: "100%",
          background: tier.accent,
          color: "#ffe28a",
          display: "flex",
          alignItems: "center",
          justifyContent: "center",
          fontSize: tier.rank.length === 3 ? 50 : 62,
          fontWeight: 800,
          letterSpacing: 2,
        },
      },
      tier.rank
    ),
    React.createElement(
      "div",
      {
        style: {
          flex: 1,
          padding: "20px 24px",
          display: "flex",
          flexDirection: "column",
          justifyContent: "center",
        },
      },
      React.createElement(
        "div",
        {
          style: {
            fontSize: 38,
            fontWeight: 800,
            color: "#21140d",
            marginBottom: 10,
            lineHeight: 1.05,
          },
        },
        tier.title
      ),
      React.createElement(
        "div",
        {
          style: {
            fontSize: 22,
            lineHeight: 1.35,
            color: "#2d2118",
            fontWeight: 500,
          },
        },
        tier.items
      )
    )
  );
}

export default function handler() {
  return new ImageResponse(
    React.createElement(
      "div",
      {
        style: {
          width: WIDTH,
          height: HEIGHT,
          display: "flex",
          flexDirection: "column",
          background: "linear-gradient(180deg, #f5e8c4 0%, #ead19a 100%)",
          padding: 32,
          fontFamily: "sans-serif",
          color: "#1c120d",
          border: "12px solid #62410f",
        },
      },
      React.createElement(
        "div",
        {
          style: {
            display: "flex",
            flexDirection: "column",
            alignItems: "center",
            border: "4px solid #7e5d25",
            background: "rgba(255, 248, 226, 0.78)",
            padding: "24px 26px 22px",
            height: "100%",
            boxShadow: "inset 0 0 0 6px rgba(255,255,255,0.34)",
          },
        },
        React.createElement(
          "div",
          { style: { fontSize: 24, letterSpacing: 8, color: "#78521e", marginBottom: 10 } },
          "DAILY DANCE HACK"
        ),
        React.createElement(
          "div",
          {
            style: {
              width: "100%",
              height: 3,
              background: "#8b6a30",
              marginBottom: 18,
            },
          }
        ),
        React.createElement(
          "div",
          {
            style: {
              fontSize: 56,
              fontWeight: 900,
              lineHeight: 1.05,
              textAlign: "center",
              color: "#21140d",
              marginBottom: 8,
            },
          },
          "ダンス上達Tier表"
        ),
        React.createElement(
          "div",
          {
            style: {
              fontSize: 22,
              color: "#6e4b1d",
              marginBottom: 22,
              letterSpacing: 1,
            },
          },
          "10分練習・ライフハック・見える成長のコツ"
        ),
        React.createElement(
          "div",
          {
            style: {
              display: "flex",
              flexDirection: "column",
              gap: 12,
              width: "100%",
            },
          },
          tiers.map((tier) => React.createElement(Row, { key: tier.rank, tier }))
        ),
        React.createElement(
          "div",
          {
            style: {
              marginTop: "auto",
              width: "100%",
              background: "#160f0a",
              color: "#ffe28a",
              fontSize: 22,
              textAlign: "center",
              padding: "13px 18px",
              letterSpacing: 1,
            },
          },
          "保存して今日10分だけやる。上達は短くても毎日の勝ち。"
        )
      )
    ),
    {
      width: WIDTH,
      height: HEIGHT,
      headers: {
        "Cache-Control": "no-store",
      },
    }
  );
}
