TARGET = clutter-demo cairo-demo ime-switch

all:$(TARGET)

clutter-demo: clutter-demo.vala
	valac --pkg clutter-gtk-1.0 -o $@ $^

cairo-demo: cairo-demo.vala
	valac --pkg clutter-gtk-1.0 -o $@ $^

ime-switch: ime-switch.vala
	valac --pkg clutter-gtk-1.0 -o $@ $^
