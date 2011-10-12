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

        draw_texture();
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

    private void draw_texture() {
        m_texture.clear();
        Cairo.Context context = m_texture.create();
        context.set_tolerance(0.1);
        context.set_line_width(7.0);
        // context.set_dash(new double[] {30 / 4.0, 30 / 4.0}, 0);
        context.set_source_rgba(1.0, 1.0, 1.0, 0.8);
        context.set_line_join(Cairo.LineJoin.ROUND);
        context.new_path();
        // context.rectangle(10.0, 10.0, 410.0, 410.0);
        context.move_to(10.0, 10.10);
        context.rel_line_to(0.0, 400.0);
        context.rel_line_to(400.0, 0.0);
        context.rel_line_to(0.0, -400.0);
        context.rel_line_to(-400.0, 0.0);
        context.close_path();
        context.stroke();
        // context.paint();
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
