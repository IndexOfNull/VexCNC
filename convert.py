import sys

with open(sys.argv[1], "r") as f:
    lines = f.readlines()
    lines = [x.replace("\n", "") for x in lines]

commands = []

def getParam(letter, code):
    fstr = ""
    collecting = False
    for char in code:
        if collecting:
            if char == " ":
                collecting = False
                continue
            fstr += char
        if char == letter:
            collecting = True
    return fstr

for command in lines:
    if not command.startswith("G1"): continue
    fstr = ""
    x = getParam("X", command);
    y = getParam("Y", command);
    z = getParam("Z", command);
    f = getParam("F", command);
    if f:
        fstr += "driver.setFeedrate({});".format(f)
    if x:
        fstr += "driver.setTargetX({0});".format(x)
    if y:
        fstr += "driver.setTargetY({0});".format(y)
    if z:
        fstr += "driver.setTargetZ({0});".format(z)
    fstr += "driver.directMoveToTarget(false);"
    print(fstr);