import os
import glob

files = glob.glob("src/**/*.obj", recursive=True)
for file in files:
    print(file)
    os.remove(file)

files = glob.glob("src/**/*.os", recursive=True)
for file in files:
    print(file)
    os.remove(file)

files = glob.glob("src/**/*.bc", recursive=True)
for file in files:
    print(file)
    os.remove(file)
