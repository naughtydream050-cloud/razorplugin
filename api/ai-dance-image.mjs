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
    items: "ビート・声・ベースの切れ目で動きを変える。音の細部に反応する。",
    accent: "#0f0b08",
  },
  {
    rank: "SS",
    title: "毎日10分のアイソレ",
    items: "首・胸・腰を各1分。小さく正確に動かすほど体の分離が伸びる。",
    accent: "#17100b",
  },
  {
    rank: "S",
    title: "動画は0.5倍で見る",
    items: "足、胸、腕、目線の順に分解。速く覚えるより、正確に拾う。",
    accent: "#271307",
  },
  {
    rank: "A",
    title: "重心を低く保つ",
    items: "膝を軽く曲げて体重移動を見せる。リズムと余裕が出る。",
    accent: "#5b260f",
  },
  {
    rank: "B",
    title: "床置きスマホで撮る",
    items: "低い角度で録画。姿勢の崩れ、手足の遅れ、ノリの弱さが見える。",
    accent: "#87531d",
  },
  {
    rank: "C",
    title: "10分メニュー",
    items: "1分ストレッチ、3分アイソレ、3分ステップ、2分振付、1分確認。",
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
        height: 134,
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
          fontSize: tier.rank.length === 3 ? 48 : 60,
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
          padding: "17px 22px 14px",
          display: "flex",
          flexDirection: "column",
          justifyContent: "center",
          overflow: "hidden",
        },
      },
      React.createElement(
        "div",
        {
          style: {
            fontSize: 32,
            fontWeight: 800,
            color: "#21140d",
            marginBottom: 8,
            lineHeight: 1.05,
            whiteSpace: "nowrap",
          },
        },
        tier.title
      ),
      React.createElement(
        "div",
        {
          style: {
            fontSize: 19,
            lineHeight: 1.38,
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
          background: "linear-gradient(180deg, #f6e8bf 0%, #e8cd90 100%)",
          padding: 26,
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
            border: "4px solid #9a7c45",
            background: "rgba(255, 248, 226, 0.82)",
            padding: "22px 26px 20px",
            height: "100%",
            boxShadow: "inset 0 0 0 6px rgba(255,255,255,0.34)",
          },
        },
        React.createElement(
          "div",
          { style: { fontSize: 22, letterSpacing: 8, color: "#78521e", marginBottom: 8 } },
          "DAILY DANCE HACK"
        ),
        React.createElement("div", {
          style: { width: "100%", height: 3, background: "#8b6a30", marginBottom: 18 },
        }),
        React.createElement(
          "div",
          {
            style: {
              fontSize: 50,
              fontWeight: 900,
              lineHeight: 1,
              textAlign: "center",
              color: "#21140d",
              marginBottom: 6,
            },
          },
          "ダンス上達Tier表"
        ),
        React.createElement(
          "div",
          {
            style: {
              fontSize: 20,
              color: "#6e4b1d",
              marginBottom: 19,
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
              fontSize: 20,
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
