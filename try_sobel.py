from uwimg import *

im = load_image("data/dog.jpg")
res = sobel_image(im)
mag = res[0]
feature_normalize(mag)
bruh=colorize_sobel(mag)
save_image(bruh, "color_sobel")