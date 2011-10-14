TARGET = clutter-demo cairo-demo ime-switch

all:$(TARGET)

VALAC_FLAGS = \
	--debug \
	--save-temps \
	--pkg clutter-1.0 \
	--pkg clutter-gtk-1.0 \
	--pkg pango \
	$(NULL)

clutter-demo: clutter-demo.vala
	valac $(VALAC_FLAGS) -o $@ $^

cairo-demo: cairo-demo.vala
	valac $(VALAC_FLAGS) -o $@ $^

ime-switch: ime-switch.vala
	valac $(VALAC_FLAGS) -o $@ $^
