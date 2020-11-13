#! /usr/bin/env python
# coding: utf-8

# generator-1.py, a simple python dungeon generator by
# James Spencer <jamessp [at] gmail.com>.

# To the extent possible under law, the person who associated CC0 with
# pathfinder.py has waived all copyright and related or neighboring rights
# to pathfinder.py.

# You should have received a copy of the CC0 legalcode along with this
# work. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

from __future__ import print_function
import random, json, sys, getopt, logging, uuid, numpy as np

CHARACTER_TILES = {'stone': ' ',
                   'floor': '.',
                   'wall': '#'}

log = None
output_filename = None

class Generator():
    def __init__(self, width=64, height=64, max_rooms=15, min_room_xy=5,
                 max_room_xy=10, rooms_overlap=False, random_connections=1,
                 random_spurs=3, tiles=CHARACTER_TILES):
        self.width = width
        self.height = height
        self.max_rooms = max_rooms
        self.min_room_xy = min_room_xy
        self.max_room_xy = max_room_xy
        self.rooms_overlap = rooms_overlap
        self.random_connections = random_connections
        self.random_spurs = random_spurs
        self.tiles = CHARACTER_TILES
        self.level = []
        self.room_list = []
        self.corridor_list = []
        self.tiles_level = []

    def gen_room(self):
        x, y, w, h = 0, 0, 0, 0

        w = random.randint(self.min_room_xy, self.max_room_xy)
        h = random.randint(self.min_room_xy, self.max_room_xy)
        x = random.randint(1, (self.width - w - 1))
        y = random.randint(1, (self.height - h - 1))

        return [x, y, w, h]

    def room_overlapping(self, room, room_list):
        x = room[0]
        y = room[1]
        w = room[2]
        h = room[3]

        for current_room in room_list:

            # The rectangles don't overlap if
            # one rectangle's minimum in some dimension
            # is greater than the other's maximum in
            # that dimension.

            if (x < (current_room[0] + current_room[2]) and
                current_room[0] < (x + w) and
                y < (current_room[1] + current_room[3]) and
                    current_room[1] < (y + h)):

                return True

        return False

    def corridor_between_points(self, x1, y1, x2, y2, join_type='either'):
        if x1 == x2 and y1 == y2 or x1 == x2 or y1 == y2:
            return [(x1, y1), (x2, y2)]
        else:
            # 2 Corridors
            # NOTE: Never randomly choose a join that will go out of bounds
            # when the walls are added.
            join = None
            if join_type == 'either' and set([0, 1]).intersection(
                    set([x1, x2, y1, y2])):

                join = 'bottom'
            elif join_type == 'either' and set([self.width - 1,
                                                self.width - 2]).intersection(set([x1, x2])) or set(
                    [self.height - 1, self.height - 2]).intersection(
                    set([y1, y2])):

                join = 'top'
            elif join_type == 'either':
                join = random.choice(['top', 'bottom'])
            else:
                join = join_type

            if join == 'top':
                return [(x1, y1), (x1, y2), (x2, y2)]
            elif join == 'bottom':
                return [(x1, y1), (x2, y1), (x2, y2)]

    def join_rooms(self, room_1, room_2, join_type='either'):
        # sort by the value of x
        sorted_room = [room_1, room_2]
        sorted_room.sort(key=lambda x_y: x_y[0])

        x1 = sorted_room[0][0]
        y1 = sorted_room[0][1]
        w1 = sorted_room[0][2]
        h1 = sorted_room[0][3]
        x1_2 = x1 + w1 - 1
        y1_2 = y1 + h1 - 1

        x2 = sorted_room[1][0]
        y2 = sorted_room[1][1]
        w2 = sorted_room[1][2]
        h2 = sorted_room[1][3]
        x2_2 = x2 + w2 - 1
        y2_2 = y2 + h2 - 1

        # overlapping on x
        if x1 < (x2 + w2) and x2 < (x1 + w1):
            jx1 = random.randint(x2, x1_2)
            jx2 = jx1
            tmp_y = [y1, y2, y1_2, y2_2]
            tmp_y.sort()
            jy1 = tmp_y[1] + 1
            jy2 = tmp_y[2] - 1

            corridors = self.corridor_between_points(jx1, jy1, jx2, jy2)
            self.corridor_list.append(corridors)

        # overlapping on y
        elif y1 < (y2 + h2) and y2 < (y1 + h1):
            if y2 > y1:
                jy1 = random.randint(y2, y1_2)
                jy2 = jy1
            else:
                jy1 = random.randint(y1, y2_2)
                jy2 = jy1
            tmp_x = [x1, x2, x1_2, x2_2]
            tmp_x.sort()
            jx1 = tmp_x[1] + 1
            jx2 = tmp_x[2] - 1

            corridors = self.corridor_between_points(jx1, jy1, jx2, jy2)
            self.corridor_list.append(corridors)

        # no overlap
        else:
            join = None
            if join_type == 'either':
                join = random.choice(['top', 'bottom'])
            else:
                join = join_type

            if join == 'top':
                if y2 > y1:
                    jx1 = x1_2 + 1
                    jy1 = random.randint(y1, y1_2)
                    jx2 = random.randint(x2, x2_2)
                    jy2 = y2 - 1
                    corridors = self.corridor_between_points(
                        jx1, jy1, jx2, jy2, 'bottom')
                    self.corridor_list.append(corridors)
                else:
                    jx1 = random.randint(x1, x1_2)
                    jy1 = y1 - 1
                    jx2 = x2 - 1
                    jy2 = random.randint(y2, y2_2)
                    corridors = self.corridor_between_points(
                        jx1, jy1, jx2, jy2, 'top')
                    self.corridor_list.append(corridors)

            elif join == 'bottom':
                if y2 > y1:
                    jx1 = random.randint(x1, x1_2)
                    jy1 = y1_2 + 1
                    jx2 = x2 - 1
                    jy2 = random.randint(y2, y2_2)
                    corridors = self.corridor_between_points(
                        jx1, jy1, jx2, jy2, 'top')
                    self.corridor_list.append(corridors)
                else:
                    jx1 = x1_2 + 1
                    jy1 = random.randint(y1, y1_2)
                    jx2 = random.randint(x2, x2_2)
                    jy2 = y2_2 + 1
                    corridors = self.corridor_between_points(
                        jx1, jy1, jx2, jy2, 'bottom')
                    self.corridor_list.append(corridors)

    def gen_level(self):

        # build an empty dungeon, blank the room and corridor lists
        for i in range(self.height):
            self.level.append(['stone'] * self.width)
        self.room_list = []
        self.corridor_list = []

        max_iters = self.max_rooms * 5

        for a in range(max_iters):
            tmp_room = self.gen_room()

            if self.rooms_overlap or not self.room_list:
                self.room_list.append(tmp_room)
            else:
                tmp_room = self.gen_room()
                tmp_room_list = self.room_list[:]

                if self.room_overlapping(tmp_room, tmp_room_list) is False:
                    self.room_list.append(tmp_room)

            if len(self.room_list) >= self.max_rooms:
                break

        # connect the rooms
        for a in range(len(self.room_list) - 1):
            self.join_rooms(self.room_list[a], self.room_list[a + 1])

        # do the random joins
        for a in range(self.random_connections):
            room_1 = self.room_list[random.randint(0, len(self.room_list) - 1)]
            room_2 = self.room_list[random.randint(0, len(self.room_list) - 1)]
            self.join_rooms(room_1, room_2)

        # do the spurs
        for a in range(self.random_spurs):
            room_1 = [random.randint(2, self.width - 2), random.randint(
                2, self.height - 2), 1, 1]
            room_2 = self.room_list[random.randint(0, len(self.room_list) - 1)]
            self.join_rooms(room_1, room_2)

        # fill the map
        # paint rooms
        for room_num, room in enumerate(self.room_list):
            for b in range(room[2]):
                for c in range(room[3]):
                    self.level[room[1] + c][room[0] + b] = 'floor'

        # paint corridors
        for corridor in self.corridor_list:
            x1, y1 = corridor[0]
            x2, y2 = corridor[1]
            for width in range(abs(x1 - x2) + 1):
                for height in range(abs(y1 - y2) + 1):
                    self.level[min(y1, y2) + height][
                        min(x1, x2) + width] = 'floor'

            if len(corridor) == 3:
                x3, y3 = corridor[2]

                for width in range(abs(x2 - x3) + 1):
                    for height in range(abs(y2 - y3) + 1):
                        self.level[min(y2, y3) + height][
                            min(x2, x3) + width] = 'floor'

        # paint the walls
        for row in range(1, self.height - 1):
            for col in range(1, self.width - 1):
                if self.level[row][col] == 'floor':
                    if self.level[row - 1][col - 1] == 'stone':
                        self.level[row - 1][col - 1] = 'wall'

                    if self.level[row - 1][col] == 'stone':
                        self.level[row - 1][col] = 'wall'

                    if self.level[row - 1][col + 1] == 'stone':
                        self.level[row - 1][col + 1] = 'wall'

                    if self.level[row][col - 1] == 'stone':
                        self.level[row][col - 1] = 'wall'

                    if self.level[row][col + 1] == 'stone':
                        self.level[row][col + 1] = 'wall'

                    if self.level[row + 1][col - 1] == 'stone':
                        self.level[row + 1][col - 1] = 'wall'

                    if self.level[row + 1][col] == 'stone':
                        self.level[row + 1][col] = 'wall'

                    if self.level[row + 1][col + 1] == 'stone':
                        self.level[row + 1][col + 1] = 'wall'

    def gen_tiles_level(self):

        for row_num, row in enumerate(self.level):
            tmp_tiles = []

            for col_num, col in enumerate(row):
                if col == 'stone':
                    tmp_tiles.append(self.tiles['stone'])
                if col == 'floor':
                    tmp_tiles.append(self.tiles['floor'])
                if col == 'wall':
                    tmp_tiles.append(self.tiles['wall'])

            self.tiles_level.append(''.join(tmp_tiles))

        print('Room List: ', self.room_list)
        print('\nCorridor List: ', self.corridor_list)

        # [print(row) for row in self.tiles_level]

    def export_to_json(self):
        global output_filename
        if (self.tiles_level == []):
            log.error("Level tiles not generated yet!")
            sys.exit(1)

        map_name = str(uuid.uuid4())
        if (output_filename == None):
            output_filename = map_name + ".json"

        class CoordsObj:
            def __init__(self, x, y):
                self.x = x;
                self.y = y;

        floor_tiles = []
        for y in range(len(self.tiles_level)):
            for x in range(len(self.tiles_level[y])):
                if (self.tiles_level[y][x] == CHARACTER_TILES["floor"]):
                    floor_tiles.append(CoordsObj(y, x))


        start_pos = np.random.choice(floor_tiles, 1)
        end_pos = np.random.choice(floor_tiles, 1)
        print(start_pos[0].x, start_pos[0].y, end_pos[0].x, end_pos[0].y)
        map_data = {
            "Params": {
                "Name": map_name,
                "Width": self.width,
                "Height": self.height,
                "Start": {
                    "x": start_pos[0].x,
                    "y": start_pos[0].y
                },
                "End": {
                    "x": end_pos[0].x,
                    "y": end_pos[0].y
                }
            },
            "Ceiling": "wood",
            "Floor": "greystone",
            "Sprites": [],
            "Walls": []
        }
        for y in range(len(self.tiles_level)):
            for x in range(len(self.tiles_level[y])):
                colour = "0xFFFFFF"
                texture = "colorstone"
                if (self.tiles_level[y][x] != CHARACTER_TILES["wall"]):
                    colour = ""
                    texture = ""
                map_data["Walls"].append({
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
        with open(output_filename, "w+") as outfile:
            json.dump(map_data, outfile)

grid_w = 25
grid_h = 25
max_room_count = 15

def parseArgs(argv):
    global grid_w, grid_h, max_room_count, output_filename
    try:
        opts, _ = getopt.getopt(argv, "w:h:r:o")
    except getopt.GetoptError:
        log.error("Invalid arguments")
        sys.exit(2)

    for opt, arg in opts:
        if (opt == "-w"):
            if (0 < int(arg)):
                grid_w = int(arg)
            else:
                log.error("Invalid grid width [" + arg + "]. Must be in range [1,inf)")
                sys.exit(1)
        elif (opt == "-h"):
            if (0 < int(arg)):
                grid_h = int(arg)
            else:
                log.error("Invalid grid height [" + arg + "]. Must be in range [1,inf)")
                sys.exit(1)
        elif (opt == "-r"):
            if (0 < int(arg)):
                max_room_count = int(arg)
            else:
                log.error("Room count must be within range [1,inf)")
                sys,exit(1)
        elif (opt == "-o"):
            if (".json" in str(arg)):
                output_filename = arg
            else:
                log.error("Output file must be in JSON format")
                sys.exit(1)

if __name__ == '__main__':
    log = logging.getLogger(__name__)
    sh = logging.StreamHandler(sys.stdout)
    formatter = logging.Formatter(
        '[%(asctime)s][%(name)s] %(levelname)s :: %(message)s')
    sh.setFormatter(formatter)
    parseArgs(sys.argv[1:])

    log.addHandler(sh)
    log.setLevel(logging.INFO)
    gen = Generator(grid_w, grid_h, max_room_count)
    gen.gen_level()
    gen.gen_tiles_level()
    gen.export_to_json()
