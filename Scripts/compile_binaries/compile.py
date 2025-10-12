import json
import PIL


def compile_sprite(filename: str):
    image = PIL.Image.open(filename)
