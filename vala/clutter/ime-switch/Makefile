TARGET = switcher

all:$(TARGET)

VALAC_FLAGS = \
	--save-temps \
	--Xcc -lm \
	--pkg posix \
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
