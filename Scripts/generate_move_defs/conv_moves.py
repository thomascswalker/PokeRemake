import codecs
import json
from pathlib import Path

moves_filename = Path(__file__).parent / 'moves.json'

GEN_1_MOVE_COUNT = 165
PHYSICAL = "物理"
SPECIAL = "特殊"
STATUS = "变化"

with codecs.open(moves_filename.as_posix(), "r", encoding="utf-8") as f:
    moves = json.load(f)

out_moves = []
for move in moves:
    if move["id"] > GEN_1_MOVE_COUNT:
        continue

    out = {"Id": move["id"], "Accuracy": move["accuracy"], "Name": move["ename"], "Power": move["power"],
           "Type": move["type"], "PP": move["pp"]}
    if move["category"] == PHYSICAL:
        out["Category"] = "Physical"
    elif move["category"] == SPECIAL:
        out["Category"] = "Special"
    elif move["category"] == STATUS:
        out["Category"] = "Status"

    for k, v in out.items():
        if v is None:
            out[k] = 0

    out_moves.append(out)

conv_moves_filename = Path(__file__).parent / 'conv_moves.json'
with open(conv_moves_filename.as_posix(), "w") as f:
    json.dump(out_moves, f, indent=4)
