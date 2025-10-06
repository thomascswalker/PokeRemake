import json
from pathlib import Path

def root_dir() -> Path:
    return Path(__file__).parent.parent.parent

def parse_map(filename: str) -> list[int]:
    with open(filename, 'r') as file:
        map_data = json.load(file)
    tiles = map_data.get("Tiles", [])
    out_data = []
    for tile in tiles:
        out_data.append(tile["Index"])
    return out_data


if __name__ == "__main__":
    root = root_dir()
    maps_dir = root / "Data" / "Maps"
    
    pallet_town_map = maps_dir / "PalletTown.json"
    data = parse_map(pallet_town_map.as_posix())
    print(data)