import math

with open("lut.h", "w") as f:
    f.write("static const uint32_t log_lut[4097] = {\n")
    for x in range(4097):
        if x == 0:
            val = 0
        else:
            # Formula: x * log2(x) * 1024
            val = int(x * math.log2(x) * 1024)
        
        f.write(f"{val},")
        if x % 8 == 7: # New line every 8 values for readability
            f.write("\n")
    f.write("\n};")
