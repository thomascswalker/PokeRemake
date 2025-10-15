import codecs
import json
from pathlib import Path

moves_filename = Path(__file__).parent / "moves.json"

GEN_1_MOVE_COUNT = 165
PHYSICAL = "物理"
SPECIAL = "特殊"
STATUS = "变化"


class EPokeType:
    Normal = 0x00
    Fighting = 0x01
    Flying = 0x02
    Poison = 0x03
    Ground = 0x04
    Rock = 0x05
    Bird = 0x06
    Bug = 0x07
    Ghost = 0x08
    Fire = 0x14
    Water = 0x15
    Grass = 0x16
    Electric = 0x17
    Psychic = 0x18
    Ice = 0x19
    Dragon = 0x1A


with codecs.open(moves_filename.as_posix(), "r", encoding="utf-8") as f:
    moves = json.load(f)

out_moves = []
for move in moves:
    if move["id"] > GEN_1_MOVE_COUNT:
        continue

    out = {
        "Id": move["id"],
        "Accuracy": move["accuracy"],
        "Name": move["ename"],
        "Power": move["power"],
        "PP": move["pp"],
    }
    if move["category"] == PHYSICAL:
        out["Category"] = 0x00
    elif move["category"] == SPECIAL:
        out["Category"] = 0x01
    elif move["category"] == STATUS:
        out["Category"] = 0x02

    out["Type"] = (
        getattr(EPokeType, move["type"]) if hasattr(EPokeType, move["type"]) else 0x00
    )

    for k, v in out.items():
        if v is None:
            out[k] = 0

    out_moves.append(out)

conv_moves_filename = Path(__file__).parent / "conv_moves.json"
with open(conv_moves_filename.as_posix(), "w") as f:
    json.dump(out_moves, f, indent=4)
