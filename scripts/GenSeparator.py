# Separator Comment Generator Script

from GenHeaderSeparator import GenHeaderSeparatorComments
from GenRegionSeparator import GenRegionSeparatorComments

while True:
    label = input("Insert label name: ")

    if label.lower().startswith("region"):
        GenRegionSeparatorComments(label[6:].strip(), "// +----", "----+", 66)
    else:
        GenHeaderSeparatorComments(label, "/* =----", "-= */", 69)
