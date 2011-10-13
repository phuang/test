import cairo

dst = cairo.ImageSurface(cairo.FORMAT_ARGB32, 256, 256)
cr = cairo.Context(dst)

# draw background
cr.set_source_rgb(1, 1, 1)
cr.paint()

for i in range(24):
   cr.rectangle(0, i*11, 256, 5)

for i in range(24):
   cr.rectangle(i*11, 0, 5, 256)

cr.set_source_rgb(0.85, 0.85, 0.85)
cr.fill()

# draw hands to initermediate surface
hand = cairo.ImageSurface(cairo.FORMAT_ARGB32, 256, 256)
hcr = cairo.Context(hand)
hcr.rotate(0.1)
hcr.set_source_rgba(0, 1, 1, 1)
hcr.rectangle(50, 50, 100, 10)
hcr.fill()
hcr.rectangle(70, 30, 10, 100)
hcr.set_source_rgba(1, 1, 0, 0.7)
hcr.fill()

# draw shadow, using hand as mask
cr.set_source_rgba(0, 0, 0, 0.5)
cr.mask_surface(hand, 5, 5)

# then paint hands on top
cr.set_source_surface(hand)
cr.paint()

# done
dst.write_to_png('hands.png')
