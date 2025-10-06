import codecs
from glob import glob
import json
import os


def convert_data(data: dict) -> dict:
    out_data = {
        "Id": data["id"],
        "Name": data["name"]["english"],
        "Type": data["type"],
        "Stats": data["base"],
    }
    return out_data

def convert_all_data() -> None:
    with codecs.open("pokemon.json", "r", encoding="utf-8") as f:
        data = json.load(f)

    out_data = []
    for mon in data:
        out_data.append(convert_data(mon))
    print(json.dumps(out_data, indent=4))

    with open("converted_data.json", "w") as f:
        json.dump(out_data, f, indent=4)

def convert_sprite(sprite_path: str, front: bool):
    name = sprite_path.split("/")[-1].split(".")[0]
    new_name = name.title() + ("Front" if front else "Back") + ".png"
    print(f"Renaming {sprite_path} => {new_name}")
    os.rename(sprite_path, new_name)

def convert_sprites() -> None:
    root = r"C:\Users\thoma\OneDrive\Documents\GitHub\PokeRemake\Resources\Textures"
    for filename in glob(f"{root}\\Front\\*.png"):
        convert_sprite(filename, True)
    for filename in glob(f"{root}\\Back\\*.png"):
        convert_sprite(filename, False)

if __name__ == "__main__":
    # convert_all_data()
    convert_sprites()