import { deflateSync } from "node:zlib";

const WIDTH = 480;
const HEIGHT = 480;

function clamp(v, min, max) {
  return Math.max(min, Math.min(max, v));
}

function fnv(seed) {
  let h = 2166136261;
  for (const c of seed) h = Math.imul(h ^ c.charCodeAt(0), 16777619);
  return () => {
    h += h << 13;
    h ^= h >>> 7;
    h += h << 3;
    h ^= h >>> 17;
    h += h << 5;
    return ((h >>> 0) % 10000) / 10000;
  };
}

function crc32(buf) {
  let crc = -1;
  for (const byte of buf) {
    crc ^= byte;
    for (let k = 0; k < 8; k += 1) crc = (crc >>> 1) ^ (0xedb88320 & -(crc & 1));
  }
  return (crc ^ -1) >>> 0;
}

function chunk(type, data) {
  const typeBuf = Buffer.from(type);
  const len = Buffer.alloc(4);
  len.writeUInt32BE(data.length);
  const crc = Buffer.alloc(4);
  crc.writeUInt32BE(crc32(Buffer.concat([typeBuf, data])));
  return Buffer.concat([len, typeBuf, data, crc]);
}

function makePng(seed) {
  const pixels = Buffer.alloc(WIDTH * HEIGHT * 4);
  const rand = fnv(seed);

  function setPixel(x, y, r, g, b, a = 255) {
    x = Math.trunc(x);
    y = Math.trunc(y);
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
    const i = (y * WIDTH + x) * 4;
    pixels[i] = r;
    pixels[i + 1] = g;
    pixels[i + 2] = b;
    pixels[i + 3] = a;
  }

  function rect(x0, y0, x1, y1, color) {
    const left = clamp(Math.floor(x0), 0, WIDTH - 1);
    const top = clamp(Math.floor(y0), 0, HEIGHT - 1);
    const right = clamp(Math.ceil(x1), 0, WIDTH - 1);
    const bottom = clamp(Math.ceil(y1), 0, HEIGHT - 1);
    for (let y = top; y <= bottom; y += 1) {
      for (let x = left; x <= right; x += 1) setPixel(x, y, ...color);
    }
  }

  function circle(cx, cy, radius, color) {
    const r2 = radius * radius;
    for (let y = Math.floor(cy - radius); y <= Math.ceil(cy + radius); y += 1) {
      for (let x = Math.floor(cx - radius); x <= Math.ceil(cx + radius); x += 1) {
        if ((x - cx) ** 2 + (y - cy) ** 2 <= r2) setPixel(x, y, ...color);
      }
    }
  }

  function line(x0, y0, x1, y1, color, thickness = 2) {
    const steps = Math.max(Math.abs(x1 - x0), Math.abs(y1 - y0));
    for (let i = 0; i <= steps; i += 1) {
      const t = i / steps;
      circle(x0 + (x1 - x0) * t, y0 + (y1 - y0) * t, thickness, color);
    }
  }

  rect(0, 0, WIDTH, HEIGHT, [76, 18, 24, 255]);
  for (let i = 0; i < 900; i += 1) {
    const x = Math.floor(rand() * WIDTH);
    const y = Math.floor(rand() * HEIGHT);
    const v = 22 + Math.floor(rand() * 22);
    setPixel(x, y, 76 + v, 18 + v / 3, 24 + v / 3, 255);
  }

  rect(55, 63, 425, 417, [241, 223, 190, 255]);
  rect(65, 73, 415, 407, [249, 235, 207, 255]);
  rect(82, 105, 398, 132, [48, 37, 33, 255]);
  rect(82, 352, 398, 359, [48, 37, 33, 255]);

  line(115, 174, 355, 170, [55, 43, 38, 255], 3);
  line(120, 218, 348, 222, [55, 43, 38, 255], 3);
  line(128, 263, 360, 257, [55, 43, 38, 255], 3);
  line(143, 306, 335, 310, [55, 43, 38, 255], 3);

  circle(107, 117, 6, [191, 37, 47, 255]);
  circle(373, 117, 6, [191, 37, 47, 255]);
  line(105, 116, 374, 116, [191, 37, 47, 255], 1);

  const scanlines = Buffer.alloc((WIDTH * 4 + 1) * HEIGHT);
  for (let y = 0; y < HEIGHT; y += 1) {
    const rowStart = y * (WIDTH * 4 + 1);
    scanlines[rowStart] = 0;
    pixels.copy(scanlines, rowStart + 1, y * WIDTH * 4, (y + 1) * WIDTH * 4);
  }

  const ihdr = Buffer.alloc(13);
  ihdr.writeUInt32BE(WIDTH, 0);
  ihdr.writeUInt32BE(HEIGHT, 4);
  ihdr[8] = 8;
  ihdr[9] = 6;

  return Buffer.concat([
    Buffer.from([137, 80, 78, 71, 13, 10, 26, 10]),
    chunk("IHDR", ihdr),
    chunk("IDAT", deflateSync(scanlines, { level: 9 })),
    chunk("IEND", Buffer.alloc(0)),
  ]);
}

export default function handler(req, res) {
  const date = new Intl.DateTimeFormat("sv-SE", {
    timeZone: "Asia/Tokyo",
    year: "numeric",
    month: "2-digit",
    day: "2-digit",
  }).format(new Date());

  res.setHeader("Content-Type", "image/png");
  res.setHeader("Cache-Control", "public, max-age=300, s-maxage=300");
  res.status(200).send(makePng(date));
}
