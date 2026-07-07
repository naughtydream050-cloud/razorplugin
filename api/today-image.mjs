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
    body: "リズム・グルーヴ・アクセントを聞き分ける。音の粒に反応できると、同じ振りでも一段上に見える。",
    visual: "HEADPHONES",
    accent: "#090807"
  },
  {
    rank: "SS",
    title: "10分アイソレ",
    body: "首・肩・胸・腰を小さく正確に動かす。派手な技より、体の分離がダンスの説得力を作る。",
    visual: "DANCER",
    accent: "#11100d"
  },
  {
    rank: "S",
    title: "動画0.5倍研究",
    body: "好きなダンサーを0.5倍で観察。足、胸、腕、目線の順に分解すると、吸収が速くなる。",
    visual: "PHONE",
    accent: "#1d130a"
  },
  {
    rank: "A",
    title: "重心を低く",
    body: "膝を軽く曲げて体重移動を見せる。安定感、キレ、余裕が一気に上がる。",
    visual: "STANCE",
    accent: "#552311"
  },
  {
    rank: "B",
    title: "床置きスマホ撮影",
    body: "低い角度で自分を撮る。姿勢の崩れ、手足の遅れ、ノリの弱さが一目でわかる。",
    visual: "TRIPOD",
    accent: "#7a4518"
  },
  {
    rank: "C",
    title: "10分練習メニュー",
    body: "2分ウォームアップ、3分アイソレ、3分ステップ、2分フリームーブ。短く毎日。",
    visual: "NOTE",
    accent: "#9a7732"
  }
];

function OrnateCorner({ pos }) {
  const style = {
    position: "absolute",
    width: 142,
    height: 142,
    borderColor: "#8b651f",
    opacity: 0.9
  };
  const map = {
    tl: { top: 24, left: 24, borderTop: "8px solid", borderLeft: "8px solid", borderTopLeftRadius: 42 },
    tr: { top: 24, right: 24, borderTop: "8px solid", borderRight: "8px solid", borderTopRightRadius: 42 },
    bl: { bottom: 24, left: 24, borderBottom: "8px solid", borderLeft: "8px solid", borderBottomLeftRadius: 42 },
    br: { bottom: 24, right: 24, borderBottom: "8px solid", borderRight: "8px solid", borderBottomRightRadius: 42 }
  };
  return e(
    "div",
    { style: { ...style, ...map[pos], display: "flex", alignItems: "center", justifyContent: "center" } },
    e("div", { style: { width: 74, height: 74, border: "5px solid #b78b2e", borderRadius: 999, opacity: 0.65 } })
  );
}

function RankBadge({ rank, accent }) {
  return e(
    "div",
    {
      style: {
        width: 210,
        height: "100%",
        display: "flex",
        alignItems: "center",
        justifyContent: "center",
        background: `linear-gradient(145deg, ${accent} 0%, #090704 70%)`,
        borderRight: "5px solid #9c7426",
        position: "relative"
      }
    },
    e("div", { style: { position: "absolute", inset: 12, border: "3px solid #b98b2d", borderRadius: 6 } }),
    e("div", { style: { position: "absolute", top: 26, fontSize: 35, color: "#f7d56e", fontWeight: 900 } }, "♛"),
    e("div", { style: { position: "absolute", left: 36, top: 68, bottom: 38, width: 4, background: "linear-gradient(#4b3410, #e3bd56, #4b3410)", borderRadius: 99 } }),
    e("div", { style: { position: "absolute", right: 36, top: 68, bottom: 38, width: 4, background: "linear-gradient(#4b3410, #e3bd56, #4b3410)", borderRadius: 99 } }),
    e("div", { style: { fontSize: rank.length === 3 ? 60 : 82, color: "#ffe37b", fontWeight: 900, letterSpacing: rank.length === 3 ? 1 : 4, textShadow: "0 3px 0 #3b2507" } }, rank),
    e("div", { style: { position: "absolute", bottom: 22, display: "flex", gap: 8 } },
      [0, 1, 2, 3, 4].map((i) => e("div", { key: i, style: { width: 9, height: 9, borderRadius: 999, background: "#d8a832" } }))
    )
  );
}

function MiniVisual({ type }) {
  const base = {
    position: "relative",
    width: 292,
    height: 146,
    display: "flex",
    alignItems: "center",
    justifyContent: "center"
  };
  if (type === "HEADPHONES") {
    return e("div", { style: base },
      e("div", { style: { width: 168, height: 104, border: "14px solid #15120d", borderBottom: "0", borderRadius: "90px 90px 0 0", position: "absolute", top: 15 } }),
      e("div", { style: { width: 58, height: 86, background: "linear-gradient(145deg,#1b1711,#050403)", border: "5px solid #c69b35", borderRadius: 22, position: "absolute", left: 52, top: 57 } }),
      e("div", { style: { width: 58, height: 86, background: "linear-gradient(145deg,#1b1711,#050403)", border: "5px solid #c69b35", borderRadius: 22, position: "absolute", right: 52, top: 57 } }),
      e("div", { style: { position: "absolute", right: 4, top: 30, width: 92, height: 64, borderTop: "5px solid #c99b30", borderBottom: "5px solid #c99b30" } })
    );
  }
  if (type === "PHONE") {
    return e("div", { style: base },
      e("div", { style: { width: 244, height: 126, background: "#101010", border: "8px solid #5f4a25", borderRadius: 26, transform: "rotate(-6deg)", display: "flex", alignItems: "center", justifyContent: "center" } },
        e("div", { style: { width: 190, height: 86, background: "linear-gradient(135deg,#2a2419,#6e5b36)", borderRadius: 12, display: "flex", alignItems: "center", justifyContent: "center", color: "#f2d06b", fontSize: 38, fontWeight: 900 } }, "0.5x")
      )
    );
  }
  if (type === "TRIPOD") {
    return e("div", { style: base },
      e("div", { style: { width: 210, height: 96, background: "#111", border: "6px solid #6f5526", borderRadius: 18, display: "flex", alignItems: "center", justifyContent: "center", color: "#e7c161", fontSize: 26, fontWeight: 900 } }, "REC"),
      e("div", { style: { position: "absolute", bottom: 10, width: 120, height: 8, background: "#17120b" } }),
      e("div", { style: { position: "absolute", bottom: 5, left: 122, width: 78, height: 8, background: "#17120b", transform: "rotate(24deg)" } }),
      e("div", { style: { position: "absolute", bottom: 5, right: 122, width: 78, height: 8, background: "#17120b", transform: "rotate(-24deg)" } })
    );
  }
  if (type === "NOTE") {
    return e("div", { style: base },
      e("div", { style: { width: 132, height: 132, background: "#f4e3ba", border: "5px solid #8a6727", borderRadius: 8, transform: "rotate(-7deg)", padding: 12, display: "flex", flexDirection: "column", gap: 8 } },
        e("div", { style: { fontSize: 22, fontWeight: 900, color: "#4d3512" } }, "10min"),
        ["Warm", "Iso", "Step", "Free"].map((t) => e("div", { key: t, style: { fontSize: 14, fontWeight: 800, color: "#3f2a10" } }, `✓ ${t}`))
      ),
      e("div", { style: { marginLeft: 12, width: 54, height: 112, border: "5px solid #4b3816", background: "linear-gradient(#d8a83a,#151008)", borderRadius: 10 } })
    );
  }
  return e("div", { style: base },
    e("div", { style: { width: 86, height: 86, borderRadius: 999, background: "#1a130a", border: "5px solid #c79a31" } }),
    e("div", { style: { position: "absolute", top: 28, width: 56, height: 10, background: "#15100a", transform: "rotate(28deg)" } }),
    e("div", { style: { position: "absolute", top: 72, width: 92, height: 12, background: "#15100a", transform: "rotate(-18deg)" } }),
    e("div", { style: { position: "absolute", bottom: 28, width: 132, height: 11, background: "#15100a", transform: "rotate(12deg)" } })
  );
}

function TierRow({ tier, index }) {
  return e(
    "div",
    {
      style: {
        display: "flex",
        width: "100%",
        height: 226,
        marginTop: index === 0 ? 0 : 14,
        border: "5px solid #8c6726",
        background: "linear-gradient(90deg,#fff2d3 0%, #f8e8be 58%, #ecd39b 100%)",
        boxShadow: "0 6px 0 rgba(83,50,8,0.28)",
        overflow: "hidden"
      }
    },
    e(RankBadge, { rank: tier.rank, accent: tier.accent }),
    e("div", { style: { flex: 1, display: "flex", padding: "22px 22px 18px 30px", position: "relative" } },
      e("div", { style: { flex: 1, display: "flex", flexDirection: "column", justifyContent: "center" } },
        e("div", { style: { fontSize: 50, fontWeight: 900, color: "#20170f", lineHeight: 1.05, marginBottom: 14 } }, tier.title),
        e("div", { style: { width: 330, height: 3, background: "linear-gradient(90deg,#8c6726,#d3a83f,transparent)", marginBottom: 14 } }),
        e("div", { style: { fontSize: 27, lineHeight: 1.34, fontWeight: 800, color: "#2a1d10" } }, tier.body)
      ),
      e(MiniVisual, { type: tier.visual }),
      e("div", { style: { position: "absolute", bottom: 12, left: 30, display: "flex", gap: 9 } },
        [0, 1, 2, 3, 4].map((i) => e("div", { key: i, style: { color: "#a97a24", fontSize: 18, fontWeight: 900 } }, "★"))
      )
    )
  );
}

export default function handler() {
  return new ImageResponse(
    e("div", {
      style: {
        width: "100%",
        height: "100%",
        display: "flex",
        flexDirection: "column",
        padding: "42px 44px 36px",
        background: "radial-gradient(circle at 50% 10%, #fff8e8 0%, #f2dfb2 45%, #d7b66d 100%)",
        border: "12px solid #5a3a0d",
        position: "relative",
        fontFamily: "Noto Sans JP, sans-serif",
        color: "#1c1209"
      }
    },
      e(OrnateCorner, { pos: "tl" }),
      e(OrnateCorner, { pos: "tr" }),
      e(OrnateCorner, { pos: "bl" }),
      e(OrnateCorner, { pos: "br" }),
      e("div", { style: { display: "flex", justifyContent: "center", color: "#7a551b", fontSize: 30, fontWeight: 900, letterSpacing: 6, marginTop: 12 } }, "DAILY DANCE HACK"),
      e("div", { style: { height: 5, background: "linear-gradient(90deg,transparent,#8b641d,#d5ac43,#8b641d,transparent)", marginTop: 18, marginBottom: 20 } }),
      e("div", { style: { display: "flex", justifyContent: "center", textAlign: "center", fontSize: 82, fontWeight: 900, lineHeight: 1.06, color: "#21150a", textShadow: "0 3px 0 #d8b562" } }, "ダンス上達Tier表"),
      e("div", { style: { display: "flex", justifyContent: "center", alignItems: "center", gap: 14, marginTop: 13, marginBottom: 25, fontSize: 31, fontWeight: 900, color: "#6b4815" } },
        e("div", { style: { width: 130, height: 3, background: "#9d762c" } }),
        "たった10分を、最高の習慣に変える。",
        e("div", { style: { width: 130, height: 3, background: "#9d762c" } })
      ),
      e("div", { style: { display: "flex", flexDirection: "column", flex: 1 } },
        tiers.map((tier, index) => e(TierRow, { tier, index, key: tier.rank }))
      ),
      e("div", { style: { marginTop: 20, height: 54, background: "linear-gradient(90deg,#0c0805,#3b2509,#0c0805)", border: "4px solid #b78b2f", color: "#f6d875", display: "flex", alignItems: "center", justifyContent: "center", fontSize: 27, fontWeight: 900, letterSpacing: 2 } }, "継続は才能を超える。今日の10分が、未来のあなたをつくる。")
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
