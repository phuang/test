TARGET = clutter-demo cairo-demo mx-demo

all:$(TARGET)

VALAC_FLAGS = \
	--debug \
	--save-temps \
	--pkg clutter-1.0 \
	--pkg clutter-gtk-1.0 \
	--pkg pango \
	--pkg mx-1.0 \
	$(NULL)

clutter-demo: clutter-demo.vala
	valac $(VALAC_FLAGS) -o $@ $^

cairo-demo: cairo-demo.vala
	valac $(VALAC_FLAGS) -o $@ $^

mx-demo: mx-demo.vala
	valac $(VALAC_FLAGS) -o $@ $^

clean:
	$(RM) $(TARGET) *.c
