# Region Separator Comment Generator Script

def GenRegionSeparatorComments(label, begin, end, max):
    tlen = max - (len(begin) + len(end) + 1)
    elen = max - (len(begin) + len(end) + 6)
    tsep = begin
    esep = begin

    if tlen == 0:
        while tlen > 0:
            tsep = tsep + "-"
            tlen = tlen - 1

        while elen > 0:
            esep = esep + "-"
            elen = elen - 1
    else:
        llen = len(label)
        tlen = tlen - (llen + 2)
        elen = elen - (llen + 2)
        tsep = tsep + " " + label + " "
        esep = esep + " " + label + " "

        while tlen > 0:
            tsep = tsep + "-"
            tlen = tlen - 1
        
        while elen > 0:
            esep = esep + "-"
            elen = elen - 1

    tsep = tsep + end
    esep = esep + " END " + end

    print("\n" + tsep + "\n")
    print("\n" + esep + "\n")
