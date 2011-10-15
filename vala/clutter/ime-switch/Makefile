TARGET = switcher

all:$(TARGET)

VALAC_FLAGS = \
	--debug \
	--save-temps \
	--pkg clutter-1.0 \
	--pkg clutter-gtk-1.0 \
	--pkg pango \
	$(NULL)

switcher_SOURCES = \
	frame.vala \
	switcher.vala \
	text.vala \
	$(NULL)

switcher: $(switcher_SOURCES)
	valac $(VALAC_FLAGS) -o $@ $^

clean:
	$(RM) $(TARGET) *.c
