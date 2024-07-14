# Separator Comment Generator Script

MAX_LEN = 69
BEG_SYM = "/* =----"
END_SYM = "-= */"
TOT_LEN = MAX_LEN - (len(BEG_SYM) + len(END_SYM) + 1)

def GenComment(lbl):
    tot = len(lbl)
    res = BEG_SYM

    if tot == 0:
        tot = TOT_LEN

        while tot > 0:
            res = res + "-"
            tot = tot - 1
    else:
        tot = TOT_LEN - (tot + 2)
        res = res + " " + lbl + " "

        while tot > 0:
            res = res + "-"
            tot = tot - 1

    res = res + END_SYM

    return res

while True:
    lbl = input("Label name: ")
    sep = GenComment(lbl)

    print("\n" + sep + "\n")
