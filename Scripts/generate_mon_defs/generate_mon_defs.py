import codecs
import csv
from dataclasses import asdict, dataclass
import json
from pathlib import Path

RAW_MOVES_FILE = "raw_moves.csv"
RAW_DEX_FILE = "raw_dex.json"
MON_COUNT = 151


@dataclass
class MoveCsvLine:
    pokemon_id: int
    version_group_id: int
    move_id: int
    pokemon_move_method_id: int
    level: int
    order: int | None = None
    mastery: int | None = None


def parse_moves():
    pokemon_moves_file = Path(__file__).parent / RAW_MOVES_FILE
    out_data = []
    mons = []

    with open(pokemon_moves_file, "r") as f:
        csv_reader = csv.reader(f, delimiter=",")
        for index, line in enumerate(csv_reader):
            if index == 0 or int(line[0]) > MON_COUNT or int(line[1]) > 1:
                continue
            data = MoveCsvLine(*[int(x) if x else None for x in line])
            if data.pokemon_id not in [p.pokemon_id for p in out_data]:
                mons.append(asdict(data))

    consolidated_data = []
    for mon in mons:
        pokemon_id = mon["pokemon_id"]
        if pokemon_id not in [p["Id"] for p in consolidated_data]:
            consolidated_data.append(
                {
                    "Id": pokemon_id,
                    "Moves": {
                        "Defaults": [],
                        "Levels": [],
                        "TMs": [],
                        "HMs": [],
                    },
                }
            )
        move_id = mon["move_id"]
        new_mon = consolidated_data[-1]
        moves = new_mon["Moves"]
        level = mon["level"]
        if level > 1:
            moves["Levels"].append({"Level": level, "Id": move_id})
        elif level == 1:
            moves["Defaults"].append(move_id)
        else:
            moves["TMs"].append(move_id)
    return consolidated_data


def parse_evo(evo_data: dict) -> dict:
    if evo_data is None or evo_data == {}:
        return {}
    if "next" not in evo_data:
        return {}
    next_data = evo_data["next"][0]
    mon_id: str = next_data[0]
    mon_level: str = next_data[1]
    return {
        "Id": int(mon_id),
        "Level": int(mon_level[len("Level ") :])
        if mon_level.startswith("Level ")
        else 0,
        "Item": mon_level[len("use ") :] if mon_level.startswith("use ") else "",
    }


def parse_dex():
    pokemon_file = Path(__file__).parent / RAW_DEX_FILE

    parsed_dex: list[dict[str, any]] = []
    with codecs.open(pokemon_file.as_posix(), "r", encoding="utf-8") as f:
        raw_dex: list[dict[str, any]] = json.load(f)

    for index, raw_mon in enumerate(raw_dex):
        if index >= MON_COUNT:
            break
        parsed_mon = {
            "Id": raw_mon["id"],
            "Name": raw_mon["name"]["english"],
            "Type": raw_mon["type"],
            "Stats": raw_mon["base"],
            "Evolution": parse_evo(raw_mon["evolution"]),
        }
        parsed_dex.append(parsed_mon)

    return parsed_dex


def main():
    dex = parse_dex()
    moves = parse_moves()

    for index, entry in enumerate(dex):
        dex[index].update(moves[index])

    with open(Path(__file__).parent / "Pokedex.json", "w") as f:
        json.dump(dex, f)


if __name__ == "__main__":
    main()
