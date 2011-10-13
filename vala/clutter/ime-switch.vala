class Switchor : GtkClutter.Embed {
    private Gtk.Window m_toplevel;
    private Clutter.Stage m_stage;
    private Clutter.CairoTexture m_texture;

    public Switchor() {
        // Create toplevel window
        m_toplevel = new Gtk.Window(Gtk.WindowType.POPUP);
        m_toplevel.add(this);
        m_toplevel.set_visual(Gdk.Screen.get_default().get_rgba_visual());
        m_toplevel.set_default_size(640, 480);
        m_toplevel.set_position(Gtk.WindowPosition.CENTER_ALWAYS);
        m_toplevel.draw.connect((w, cr) => true);
        m_toplevel.delete_event.connect((w,e) => {
            Gtk.main_quit();
            return true;
        });

        // Init stage
        m_stage = (Clutter.Stage)get_stage();
        m_stage.use_alpha = true;
        m_stage.color = Clutter.Color() { alpha = 255 };
        m_stage.opacity = 0;

        m_texture = new Clutter.CairoTexture(500, 500);

        m_texture.draw.connect(texture_draw);
        m_texture.invalidate();

        m_stage.add(m_texture);
        m_toplevel.show_all();
    }

    public void start() {
        if (true) {
            m_stage.animate(
                Clutter.AnimationMode.LINEAR, 5000,
                opacity: 255);
        }
    }

    public override void size_allocate(Gtk.Allocation allocation) {
        base.size_allocate(allocation);
        stdout.printf("size_allocate(%d %d %d %d)\n", allocation.x, allocation.y, allocation.width, allocation.height);
    }

    private void rectangle_path(Cairo.Context cr,
                                double x,
                                double y,
                                double width,
                                double height,
                                double radius,
                                bool fill) {
        assert(radius * 2 < double.min(width, height));
        const double PI = 3.1415926;
        cr.new_path();

        cr.move_to(x + radius, y);
        cr.line_to(x + width - radius, y);
        cr.arc(x + width - radius, y + radius, radius, - PI / 2 , 0.0);
        cr.line_to(x + width, y + height - radius);
        cr.arc(x + width - radius, y + height - radius, radius, 0.0, PI / 2);
        cr.line_to(x + radius, y + height);
        cr.arc(x + radius, y + height - radius, radius, PI / 2, PI);
        cr.line_to(x, y + radius);
        cr.arc(x + radius, y + radius, radius, PI, PI + PI / 2);
        cr.close_path();
    }

    private bool texture_draw(Clutter.CairoTexture texture, Cairo.Context cr) {
        rectangle_path(cr, 10.0, 10.0, 400.0, 400.0, 10.0, false);

        cr.set_source_rgba(0.0, 0.0, 0.0, 0.8);
        cr.fill_preserve();

        cr.set_source_rgba(1.0, 1.0, 1.0, 1.0);
        cr.set_line_width(2.0);
        cr.stroke();

        return true;
    }
}

extern void clutter_x11_set_use_argb_visual(bool use_argb);

public static void main(string[] args) {
    clutter_x11_set_use_argb_visual(true);
    GtkClutter.init(ref args);
    var switchor = new Switchor();
    switchor.start();
    Gtk.main();
}
