# Header Separator Comment Generator Script

def GenHeaderSeparatorComments(label, begin, end, max):
    slen = max - (len(begin) + len(end) + 1)
    ssep = begin

    if len(label) == 0:
        while slen > 0:
            ssep = ssep + "-"
            slen = slen - 1
    else:
        llen = len(label)
        slen = slen - (llen + 2)
        ssep = ssep + " " + label + " "

        while slen > 0:
            ssep = ssep + "-"
            slen = slen - 1

    ssep = ssep + end

    print("\n" + ssep + "\n")
