from PIL import Image

inp = "app/src/images/SAMI_WALK_LOGO.png"
out = "app/src/images/sami_walk_logo.c"

img = Image.open(inp).convert("RGBA")
img.thumbnail((260, 140))

w, h = img.size
data = []

for y in range(h):
    for x in range(w):
        r, g, b, a = img.getpixel((x, y))
        data.append(b)
        data.append(g)
        data.append(r)
        data.append(a)

with open(out, "w") as f:
    f.write('#include "lvgl.h"\n\n')
    f.write("const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST uint8_t sami_walk_logo_map[] = {\n")

    for i, byte in enumerate(data):
        if i % 16 == 0:
            f.write("    ")
        f.write(f"0x{byte:02X}, ")
        if i % 16 == 15:
            f.write("\n")

    f.write("\n};\n\n")

    f.write("const lv_img_dsc_t sami_walk_logo = {\n")
    f.write("    .header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA,\n")
    f.write("    .header.always_zero = 0,\n")
    f.write("    .header.reserved = 0,\n")
    f.write(f"    .header.w = {w},\n")
    f.write(f"    .header.h = {h},\n")
    f.write(f"    .data_size = {w * h} * LV_IMG_PX_SIZE_ALPHA_BYTE,\n")
    f.write("    .data = sami_walk_logo_map,\n")
    f.write("};\n")
