import tkinter as tk
import os
from PIL import Image, ImageTk

WIDTH = 20
HEIGHT = 20

OPTIONS = [
    "Jan",
    "Feb",
    "Mar"
]

PATH = "resources/textures/"

files = []
filenames = []
for r, d, f in os.walk(PATH):
    for file in f:
        if ".png" in file:
            files.append(os.path.join(r, file))
            filenames.append(file)

window = tk.Tk()

grid = []
for i in range(HEIGHT):
    row = []
    for j in range(ROW):
        frame = tk.Frame(
            master=window,
            relief=tk.RAISED,
            borderwidth=1
        )
        frame.grid(row=i, column=j)
        img = ImageTk.PhotoImage(Image.open(
            files[1]).resize((32,32)))
        label = tk.Label(frame, image=img)
        label.image = img
        label.pack()
        row.append(label)
    grid.append(row)

variable = tk.StringVar(window)
variable.set(filenames[0])

w = tk.OptionMenu(window, variable, *filenames)
w.grid(column=int(WIDTH / 2), row=WIDTH + 1)

window.mainloop()
