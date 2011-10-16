TARGET = \
	clutter-demo \
	cairo-demo \
	mash-demo \
	mx-demo \
	mx-widget-factory \
	$(NULL)

all:$(TARGET)

VALAC_FLAGS = \
	--debug \
	--save-temps \
	-X -lmash-0.1 \
	-X -I/usr/include/mash-0.1 \
	--pkg clutter-1.0 \
	--pkg clutter-gtk-1.0 \
	--pkg pango \
	--pkg mx-1.0 \
	--pkg Mash-0.1 \
	$(NULL)

clutter-demo: clutter-demo.vala
	valac $(VALAC_FLAGS) -o $@ $^

cairo-demo: cairo-demo.vala
	valac $(VALAC_FLAGS) -o $@ $^

mash-demo: mash-demo.vala
	valac $(VALAC_FLAGS) -o $@ $^

mx-demo: mx-demo.vala
	valac $(VALAC_FLAGS) -o $@ $^

mx-widget-factory: mx-widget-factory.vala
	valac $(VALAC_FLAGS) -o $@ $^

clean:
	$(RM) $(TARGET) *.c
