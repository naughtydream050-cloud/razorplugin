import React from "react";
import { ImageResponse } from "@vercel/og";

export const config = { runtime: "edge" };

const rows = [
  ["SSS", "音の粒を聞く", "ビート・声・ベースの切れ目で動きを変える。音の細部に反応する。"],
  ["SS", "毎日10分のアイソレ", "首・胸・腰を各1分。小さく正確に動かすほど体の分離が伸びる。"],
  ["S", "動画は0.5倍で見る", "足、胸、腕、目線の順に分解。速く覚えるより、正確に拾う。"],
  ["A", "重心を低く保つ", "膝を軽く曲げて体重移動を見せる。リズムと余裕が出る。"],
  ["B", "床置きスマホで撮る", "低い角度で録画。姿勢の崩れ、手足の遅れ、ノリの弱さが見える。"],
  ["C", "10分メニュー", "1分ストレッチ、3分アイソレ、3分ステップ、2分振付、1分確認。"]
];

function Row({ rank, title, body, index }) {
  const colors = ["#0b0704", "#15100a", "#281004", "#64260f", "#94601f", "#aa842e"];
  return React.createElement(
    "div",
    { style: { display: "flex", width: "100%", height: 149, marginBottom: 12, border: "3px solid #9b742c", background: "#fff3d5" } },
    React.createElement(
      "div",
      { style: { width: 220, height: "100%", display: "flex", alignItems: "center", justifyContent: "center", background: colors[index], color: "#ffe68c", fontSize: rank.length === 3 ? 72 : 82, fontWeight: 700, letterSpacing: 4 } },
      rank
    ),
    React.createElement(
      "div",
      { style: { flex: 1, height: "100%", display: "flex", flexDirection: "column", justifyContent: "center", padding: "0 36px", color: "#211509" } },
      React.createElement("div", { style: { fontSize: 42, fontWeight: 800, marginBottom: 12 } }, title),
      React.createElement("div", { style: { fontSize: 25, lineHeight: 1.35, fontWeight: 500 } }, body)
    )
  );
}

export default function handler() {
  return new ImageResponse(
    React.createElement(
      "div",
      { style: { width: "100%", height: "100%", display: "flex", justifyContent: "center", alignItems: "center", background: "#5a340d", padding: 18, fontFamily: "sans-serif" } },
      React.createElement(
        "div",
        { style: { width: 984, height: 1500, display: "flex", flexDirection: "column", background: "#fff3d5", border: "5px solid #e0bd74", padding: "34px 38px 28px", boxShadow: "0 0 0 8px #f5e1ac inset" } },
        React.createElement("div", { style: { display: "flex", justifyContent: "center", fontSize: 29, letterSpacing: 16, color: "#866126", fontWeight: 600, marginTop: 10 } }, "DAILY DANCE HACK"),
        React.createElement("div", { style: { height: 4, background: "#8d6a2c", margin: "22px 20px 20px" } }),
        React.createElement("div", { style: { display: "flex", justifyContent: "center", fontSize: 72, fontWeight: 900, color: "#180f08", marginBottom: 6 } }, "ダンス上達Tier表"),
        React.createElement("div", { style: { display: "flex", justifyContent: "center", fontSize: 27, color: "#6c4519", fontWeight: 700, marginBottom: 28 } }, "10分練習・ライフハック・見える成長のコツ"),
        React.createElement("div", { style: { display: "flex", flexDirection: "column" } }, rows.map((r, i) => React.createElement(Row, { key: r[0], rank: r[0], title: r[1], body: r[2], index: i }))),
        React.createElement("div", { style: { marginTop: "auto", height: 74, background: "#120b06", color: "#ffe68c", display: "flex", alignItems: "center", justifyContent: "center", fontSize: 26, fontWeight: 700, border: "3px solid #9b742c" } }, "保存して今日10分だけやる。上達は短くても毎日の勝ち。")
      )
    ),
    { width: 1024, height: 1536, headers: { "content-type": "image/png", "cache-control": "no-store, max-age=0" } }
  );
}
