import { deflateSync } from "zlib";

const WIDTH = 720;
const HEIGHT = 1280;

function crc32(buffer) {
  let crc = 0xffffffff;
  for (const byte of buffer) {
    crc ^= byte;
    for (let i = 0; i < 8; i++) {
      crc = (crc >>> 1) ^ (0xedb88320 & -(crc & 1));
    }
  }
  return (crc ^ 0xffffffff) >>> 0;
}

function chunk(type, data) {
  const typeBuffer = Buffer.from(type);
  const length = Buffer.alloc(4);
  length.writeUInt32BE(data.length, 0);
  const crc = Buffer.alloc(4);
  crc.writeUInt32BE(crc32(Buffer.concat([typeBuffer, data])), 0);
  return Buffer.concat([length, typeBuffer, data, crc]);
}

function fillRect(pixels, x, y, w, h, r, g, b) {
  const x2 = Math.min(WIDTH, x + w);
  const y2 = Math.min(HEIGHT, y + h);
  for (let yy = Math.max(0, y); yy < y2; yy++) {
    for (let xx = Math.max(0, x); xx < x2; xx++) {
      const idx = (yy * WIDTH + xx) * 3;
      pixels[idx] = r;
      pixels[idx + 1] = g;
      pixels[idx + 2] = b;
    }
  }
}

function makePng() {
  const pixels = Buffer.alloc(WIDTH * HEIGHT * 3);
  fillRect(pixels, 0, 0, WIDTH, HEIGHT, 244, 226, 184);
  fillRect(pixels, 28, 28, WIDTH - 56, HEIGHT - 56, 248, 238, 210);
  fillRect(pixels, 50, 50, WIDTH - 100, 90, 24, 16, 10);
  fillRect(pixels, 80, 180, 190, 210, 18, 13, 9);
  fillRect(pixels, 285, 180, 355, 210, 236, 215, 164);
  fillRect(pixels, 80, 430, 190, 210, 64, 30, 16);
  fillRect(pixels, 285, 430, 355, 210, 236, 215, 164);
  fillRect(pixels, 80, 680, 190, 210, 130, 82, 31);
  fillRect(pixels, 285, 680, 355, 210, 236, 215, 164);
  fillRect(pixels, 80, 930, 190, 210, 152, 116, 49);
  fillRect(pixels, 285, 930, 355, 210, 236, 215, 164);
  fillRect(pixels, 52, HEIGHT - 118, WIDTH - 104, 58, 24, 16, 10);

  const raw = Buffer.alloc((WIDTH * 3 + 1) * HEIGHT);
  for (let y = 0; y < HEIGHT; y++) {
    raw[y * (WIDTH * 3 + 1)] = 0;
    pixels.copy(raw, y * (WIDTH * 3 + 1) + 1, y * WIDTH * 3, (y + 1) * WIDTH * 3);
  }

  const ihdr = Buffer.alloc(13);
  ihdr.writeUInt32BE(WIDTH, 0);
  ihdr.writeUInt32BE(HEIGHT, 4);
  ihdr[8] = 8;
  ihdr[9] = 2;
  ihdr[10] = 0;
  ihdr[11] = 0;
  ihdr[12] = 0;

  return Buffer.concat([
    Buffer.from([137, 80, 78, 71, 13, 10, 26, 10]),
    chunk("IHDR", ihdr),
    chunk("IDAT", deflateSync(raw, { level: 9 })),
    chunk("IEND", Buffer.alloc(0)),
  ]);
}

export default function handler(req, res) {
  const image = makePng();
  res.setHeader("Content-Type", "image/png");
  res.setHeader("Content-Length", String(image.length));
  res.setHeader("Cache-Control", "no-store");
  res.status(200).send(image);
}
