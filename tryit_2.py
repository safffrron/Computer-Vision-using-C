from uwimg import *

im = load_image("figs/ronbledore.jpg")
f = make_gaussian_filter(2)
lfreq_ronbledore = convolve_image(im, f, 1)
hfreq_ronbledore = im - lfreq_ronbledore
reconstruct_ronbledore = lfreq_ronbledore + hfreq_ronbledore
save_image(lfreq_ronbledore, "low-frequency_ronbledore")
save_image(hfreq_ronbledore, "high-frequency_ronbledore")
save_image(reconstruct_ronbledore, "reconstruct_ronbledore")