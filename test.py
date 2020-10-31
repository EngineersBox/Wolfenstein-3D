import json

walls = [
    ["colorstone", "colorstone", "colorstone", "colorstone", "colorstone", "colorstone", "colorstone", "colorstone", "colorstone", "colorstone", "colorstone",
        "greystone", "greystone", "mossy", "greystone", "greystone", "mossy", "greystone", "mossy", "greystone", "greystone", "greystone", "mossy", "greystone"],
    ["colorstone", "none", "none", "none", "none", "none", "none", "none", "none", "none", "colorstone", "greystone",
        "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "greystone"],
    ["colorstone", "none", "purplestone", "purplestone", "none", "none", "none", "none", "none", "colorstone", "colorstone",
        "greystone", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "mossy"],
    ["colorstone", "none", "none", "purplestone", "none", "none", "none", "none", "none", "none", "none",
        "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "mossy"],
    ["colorstone", "none", "purplestone", "purplestone", "none", "none", "none", "none", "none", "colorstone", "colorstone",
        "greystone", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "greystone"],
    ["colorstone", "none", "none", "none", "none", "none", "none", "none", "none", "none", "colorstone", "greystone",
        "none", "none", "none", "none", "none", "mossy", "mossy", "mossy", "none", "mossy", "greystone", "mossy"],
    ["colorstone", "colorstone", "colorstone", "colorstone", "none", "colorstone", "colorstone", "colorstone", "colorstone", "colorstone", "colorstone",
        "greystone", "greystone", "greystone", "greystone", "greystone", "greystone", "mossy", "none", "none", "none", "none", "none", "mossy"],
    ["wood", "wood", "wood", "wood", "none", "wood", "wood", "wood", "wood", "none", "colorstone", "none", "colorstone",
        "none", "colorstone", "none", "colorstone", "greystone", "none", "greystone", "none", "mossy", "none", "mossy"],
    ["wood", "wood", "none", "none", "none", "none", "none", "none", "wood", "colorstone", "none", "colorstone", "none",
        "colorstone", "none", "colorstone", "colorstone", "mossy", "none", "none", "none", "none", "none", "mossy"],
    ["wood", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none",
        "none", "none", "none", "colorstone", "mossy", "none", "none", "none", "none", "none", "greystone"],
    ["wood", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none",
        "none", "none", "none", "colorstone", "mossy", "none", "mossy", "none", "mossy", "none", "mossy"],
    ["wood", "wood", "none", "none", "none", "none", "none", "none", "wood", "colorstone", "none", "colorstone", "none",
        "colorstone", "none", "colorstone", "colorstone", "mossy", "greystone", "mossy", "none", "mossy", "mossy", "mossy"],
    ["wood", "wood", "wood", "wood", "none", "wood", "wood", "wood", "wood", "colorstone", "colorstone", "greystone", "none", "mossy",
        "colorstone", "greystone", "colorstone", "purplestone", "purplestone", "purplestone", "none", "purplestone", "purplestone", "purplestone"],
    ["redbrick", "redbrick", "redbrick", "redbrick", "none", "redbrick", "redbrick", "redbrick", "redbrick", "greystone", "mossy",
        "greystone", "none", "none", "mossy", "none", "mossy", "purplestone", "none", "none", "none", "none", "none", "purplestone"],
    ["redbrick", "redbrick", "none", "none", "none", "none", "none", "redbrick", "redbrick", "greystone", "none", "none",
        "none", "none", "none", "none", "greystone", "purplestone", "none", "none", "none", "none", "none", "purplestone"],
    ["redbrick", "none", "none", "none", "none", "none", "none", "none", "redbrick", "greystone", "none", "none",
        "none", "none", "none", "none", "greystone", "purplestone", "none", "none", "none", "none", "none", "purplestone"],
    ["eagle", "none", "none", "none", "none", "none", "none", "none", "eagle", "greystone", "greystone", "greystone", "greystone",
        "greystone", "mossy", "none", "mossy", "purplestone", "purplestone", "none", "none", "none", "purplestone", "purplestone"],
    ["redbrick", "none", "none", "none", "none", "none", "none", "none", "redbrick", "redbrick", "redbrick", "eagle", "redbrick",
        "redbrick", "redbrick", "mossy", "mossy", "none", "none", "bluestone", "none", "bluestone", "none", "bluestone"],
    ["redbrick", "redbrick", "none", "none", "none", "none", "none", "redbrick", "redbrick", "redbrick", "none", "none", "none",
        "redbrick", "redbrick", "none", "bluestone", "none", "bluestone", "none", "none", "none", "bluestone", "bluestone"],
    ["redbrick", "none", "none", "none", "none", "none", "none", "none", "redbrick", "none", "none", "none", "none",
        "none", "redbrick", "bluestone", "none", "bluestone", "none", "bluestone", "none", "bluestone", "none", "bluestone"],
    ["eagle", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none",
        "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "bluestone"],
    ["redbrick", "none", "none", "none", "none", "none", "none", "none", "redbrick", "none", "none", "none", "none",
        "none", "redbrick", "bluestone", "none", "bluestone", "none", "bluestone", "none", "bluestone", "none", "bluestone"],
    ["redbrick", "redbrick", "none", "none", "none", "none", "none", "redbrick", "redbrick", "redbrick", "none", "none", "none",
        "redbrick", "redbrick", "none", "bluestone", "none", "bluestone", "none", "none", "none", "bluestone", "bluestone"],
    ["redbrick", "redbrick", "redbrick", "redbrick", "eagle", "redbrick", "redbrick", "redbrick", "redbrick", "redbrick", "redbrick", "eagle", "redbrick", "redbrick", "redbrick", "bluestone", "bluestone", "bluestone", "bluestone", "bluestone", "bluestone", "bluestone", "bluestone", "bluestone"]]

newmap = {"Walls":[]}
for y in range(len(walls)):
    for x in range(len(walls[y])):
        colour = "WHITE"
        texture = ""
        if (walls[y][x] == "none"):
            colour = "NONE"
        else:
            texture = walls[y][x]
        newmap["Walls"].append({
            "x": x,
            "y": y,
            "Left": {
                "Colour": colour,
                "Texture": texture
            },
            "Right": {
                "Colour": colour,
                "Texture": texture
            },
            "Up": {
                "Colour": colour,
                "Texture": texture
            },
            "Down": {
                "Colour": colour,
                "Texture": texture
            }
        })

with open('data.json', 'w') as outfile:
    json.dump(newmap, outfile)
