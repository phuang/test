using Math;

class Text : Clutter.Box {
    private Clutter.Text m_text;
    public Text(string text) {
        layout_manager = new Clutter.BoxLayout();
        m_text = new Clutter.Text();
        m_text.text = text;
        m_text.color = { 255, 255, 255, 255 };
        pack(m_text,
            "x-align", Clutter.BoxAlignment.CENTER,
            "y-align", Clutter.BoxAlignment.CENTER,
            "expand", true);
    }
}

class SwitchorWidget : Clutter.CairoTexture {
}

class Switchor : GtkClutter.Embed {
    private Gtk.Window m_toplevel;
    private Clutter.Stage m_stage;
    private Clutter.CairoTexture m_texture;
    private Clutter.CairoTexture m_shadow_texture;
    private Text m_text;
    private Gtk.Allocation m_allocation;

    public Switchor() {
        add_events(Gdk.EventMask.BUTTON_PRESS_MASK);
        // Create toplevel window
        m_toplevel = new Gtk.Window(Gtk.WindowType.POPUP);
        m_toplevel.add(this);
        m_toplevel.set_visual(Gdk.Screen.get_default().get_rgba_visual());
        m_toplevel.set_default_size(200, 300);
        m_toplevel.set_position(Gtk.WindowPosition.CENTER_ALWAYS);
        m_toplevel.draw.connect((w, cr) => true);
        m_toplevel.delete_event.connect((w,e) => {
            Gtk.main_quit();
            return true;
        });

        // Init stage
        m_stage = (Clutter.Stage)get_stage();
        m_stage.use_alpha = true;
        m_stage.color = Clutter.Color() { alpha = 0 };
        m_stage.opacity = 255;

        m_toplevel.show_all();
    }

    public void start() {
        /*
        if (false) {
            m_stage.animate(
                Clutter.AnimationMode.LINEAR, 5000,
                opacity: 255);
        }
        */
    }

    public override void size_allocate(Gtk.Allocation allocation) {
        base.size_allocate(allocation);
        if (m_allocation != allocation) {
            stdout.printf("size_allocate(%d %d %d %d)\n",
                allocation.x, allocation.y, allocation.width, allocation.height);

            m_allocation = allocation;
            m_shadow_texture = new Clutter.CairoTexture(allocation.width, allocation.height);
            m_shadow_texture.add_effect(new Clutter.BlurEffect());
            m_shadow_texture.draw.connect(shadow_texture_draw);
            m_shadow_texture.invalidate();
            // m_stage.add(m_shadow_texture);

            m_texture = new Clutter.CairoTexture(allocation.width, allocation.height);
            m_texture.draw.connect(texture_draw);
            m_texture.invalidate();
            m_stage.add(m_texture);

            m_text = new Text("拼音");
            m_stage.add(m_text);
        }
    }

    public override bool button_press_event(Gdk.EventButton e) {
        stdout.printf("button press\n");
        return true;
    }

    private void rectangle_path(Cairo.Context cr,
                                double x,
                                double y,
                                double width,
                                double height,
                                double radius,
                                bool fill) {
        assert(radius * 2 < double.min(width, height));
        cr.new_path();

        cr.move_to(x + radius, y);
        cr.line_to(x + width - radius, y);
        cr.arc(x + width - radius, y + radius, radius, 3 * PI / 2 , 2 * PI);
        cr.line_to(x + width, y + height - radius);
        cr.arc(x + width - radius, y + height - radius, radius, 0.0, PI / 2);
        cr.line_to(x + radius, y + height);
        cr.arc(x + radius, y + height - radius, radius, PI / 2, PI);
        cr.line_to(x, y + radius);
        cr.arc(x + radius, y + radius, radius, PI, 3 * PI / 2);
        cr.close_path();
    }

    private bool shadow_texture_draw(Clutter.CairoTexture texture, Cairo.Context cr) {
        // Draw shadow
        cr.set_source_rgba(1.0, 1.0, 1.0, 0.0);
        cr.paint();
        rectangle_path(cr,
                       0.0, 0.0,
                       texture.width - 0.0, texture.height - 0.0,
                       10.0, false);

        cr.set_source_rgba(0.0, 0.0, 0.0, 0.3);
        cr.fill();
        return true;
    }

    private bool texture_draw(Clutter.CairoTexture texture, Cairo.Context cr) {
        // Draw
        rectangle_path(cr,
                       0.0, 0.0,
                       texture.width, texture.height,
                       20.0, false);
        cr.set_source_rgba(0.0, 0.0, 0.0, 0.8);
        cr.fill_preserve();
        cr.set_source_rgba(1.0, 1.0, 1.0, 0.5);
        cr.set_line_width(0.5);
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
