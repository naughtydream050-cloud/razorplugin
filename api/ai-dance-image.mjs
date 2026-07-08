import React from "react";
import { ImageResponse } from "@vercel/og";

export const config = { runtime: "edge" };

const rows = [
  ["S", "Rhythm control / Musicality"],
  ["A", "Foundation / Isolation / Grooves"],
  ["B", "Freestyle ideas / Battle stamina"],
  ["C", "Practice routine / Basic drills"]
];

function Row({ rank, text }) {
  return React.createElement(
    "div",
    {
      style: {
        display: "flex",
        width: "100%",
        marginBottom: 18,
        border: "4px solid #1f1308",
        borderRadius: 22,
        overflow: "hidden",
        background: "#fff7df"
      }
    },
    React.createElement(
      "div",
      {
        style: {
          width: 150,
          display: "flex",
          alignItems: "center",
          justifyContent: "center",
          fontSize: 78,
          fontWeight: 900,
          color: "#fff7df",
          background: "#1f1308"
        }
      },
      rank
    ),
    React.createElement(
      "div",
      {
        style: {
          flex: 1,
          display: "flex",
          alignItems: "center",
          padding: "26px 34px",
          fontSize: 38,
          fontWeight: 800,
          color: "#1f1308"
        }
      },
      text
    )
  );
}

export default function handler() {
  return new ImageResponse(
    React.createElement(
      "div",
      {
        style: {
          width: "100%",
          height: "100%",
          display: "flex",
          flexDirection: "column",
          background: "linear-gradient(180deg, #fff1bc 0%, #d6a94d 100%)",
          padding: "58px 64px",
          color: "#1f1308",
          fontFamily: "sans-serif"
        }
      },
      React.createElement(
        "div",
        {
          style: {
            display: "flex",
            justifyContent: "center",
            fontSize: 78,
            fontWeight: 900,
            letterSpacing: -2,
            marginBottom: 14
          }
        },
        "DANCE SKILL TIER"
      ),
      React.createElement(
        "div",
        {
          style: {
            display: "flex",
            justifyContent: "center",
            fontSize: 34,
            fontWeight: 800,
            marginBottom: 44
          }
        },
        "LINE image delivery test - clean PNG"
      ),
      React.createElement(
        "div",
        {
          style: {
            display: "flex",
            flexDirection: "column",
            width: "100%"
          }
        },
        rows.map(([rank, text]) => React.createElement(Row, { key: rank, rank, text }))
      ),
      React.createElement(
        "div",
        {
          style: {
            marginTop: "auto",
            display: "flex",
            justifyContent: "center",
            fontSize: 30,
            fontWeight: 700,
            opacity: 0.82
          }
        },
        "Generated on Vercel OG ImageResponse"
      )
    ),
    {
      width: 1080,
      height: 1350,
      headers: {
        "Cache-Control": "no-store, max-age=0",
        "Content-Type": "image/png"
      }
    }
  );
}
