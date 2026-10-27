#!/usr/bin/env python3
"""Generate four GPL-2.0 DOS-palette signal sprites in GRF v1 format.
No external dependencies. R/Y/YG/G correspond to native aspects 0/1/2/3.
GRF decoder contracts: src/spritecache.cpp and src/spriteloader/grf.cpp.
"""
from pathlib import Path
import argparse
import struct

WIDTH, HEIGHT = 7, 24
RED, YELLOW, GREEN = 0xB8, 0xBF, 0xD0
TARGET = Path(__file__).resolve().parents[1] / "media/baseset/four_aspect_lamps.grf"

def pixels(aspect):
    data = bytearray(WIDTH * HEIGHT)
    def put(x, y, colour):
        data[y * WIDTH + x] = colour
    for y in range(1, 16):
        for x in range(1, 6):
            put(x, y, 1 if x in (1, 5) or y in (1, 15) else 6)
    for y in range(16, HEIGHT):
        put(3, y, 7)
        put(4, y, 1)
    for row, colour, lit in ((4, GREEN, aspect in (2, 3)),
                             (8, YELLOW, aspect in (1, 2)),
                             (12, RED, aspect == 0)):
        for dx, dy in ((0, 0), (-1, 0), (1, 0), (0, -1), (0, 1)):
            put(3 + dx, row + dy, colour if lit else 1)
    return bytes(data)

def encode_sprite(data):
    # Type 0: size is decompressed pixel count + eight-byte header.
    # Literal RLE packets; avoids the GRF back-reference variant entirely.
    compressed = b"".join(bytes([len(data[i:i+127])]) + data[i:i+127]
                          for i in range(0, len(data), 127))
    return struct.pack("<HBBHhh", len(data) + 8, 0, HEIGHT, WIDTH, -3, -23) + compressed

def build():
    return b"".join(encode_sprite(pixels(a)) for a in range(4)) + b"\0\0"

def validate(blob):
    # Independently walk all four records using the engine's v1 RLE contract.
    pos = 0
    for aspect in range(4):
        size, typ, height, width, x, y = struct.unpack_from("<HBBHhh", blob, pos)
        pos += 10
        assert (typ, height, width, x, y) == (0, HEIGHT, WIDTH, -3, -23)
        out = bytearray()
        while len(out) < size - 8:
            count = blob[pos]
            pos += 1
            assert count < 128
            count = count or 128
            out.extend(blob[pos:pos+count])
            pos += count
        assert len(out) == WIDTH * HEIGHT
        assert bytes(out) == pixels(aspect)
        assert (RED in out) == (aspect == 0)
        assert (YELLOW in out) == (aspect in (1, 2))
        assert (GREEN in out) == (aspect in (2, 3))
    assert blob[pos:] == b"\0\0"

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    result = build()
    validate(result)
    if args.check:
        assert TARGET.read_bytes() == result, "Regenerate four_aspect_lamps.grf"
    else:
        TARGET.parent.mkdir(parents=True, exist_ok=True)
        TARGET.write_bytes(result)
    print("Validated 4 distinct signal sprites and GRF v1 record boundaries.")
