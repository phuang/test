TARGETS = \
    xgrab \
	keybinding-manager \
    $(NULL)

all: $(TARGETS)

ibus_pkgname = ibus-1.0
vala_pkgname = libvala-0.14

xgrab: xgrab.vala
	valac \
		-g --vapidir .. \
		--pkg $(ibus_pkgname) \
		--pkg $(vala_pkgname) \
		--pkg gio-2.0 \
		--pkg gtk+-3.0 \
		--pkg gtk+-3.0 \
		--pkg gdk-x11-3.0 \
		--pkg x11 \
		$^ -o $@

keybinding-manager: keybinding-manager.vala
	valac \
		-g --vapidir .. \
		--pkg $(ibus_pkgname) \
		--pkg $(vala_pkgname) \
		--pkg gio-2.0 \
		--pkg gtk+-3.0 \
		--pkg gtk+-3.0 \
		--pkg gdk-x11-3.0 \
		--pkg glib-2.0 \
		--pkg x11 \
		$^ -o $@


clean:
	$(RM) $(TARGETS)
