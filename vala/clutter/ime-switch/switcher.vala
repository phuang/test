using Math;

class Switchor : GtkClutter.Embed {
 private
  Gtk.Window m_toplevel;
 private
  Clutter.Stage m_stage;
 private
  Gtk.Allocation m_allocation;

 public
  Switchor() {
    add_events(Gdk.EventMask.BUTTON_PRESS_MASK);
    // Create toplevel window
    m_toplevel = new Gtk.Window(Gtk.WindowType.POPUP);
    m_toplevel.add(this);
    m_toplevel.set_visual(Gdk.Screen.get_default().get_rgba_visual());
    m_toplevel.set_default_size(400, 400);
    m_toplevel.set_position(Gtk.WindowPosition.CENTER_ALWAYS);
    m_toplevel.draw.connect((w, cr) = > true);
    m_toplevel.delete_event.connect((w, e) = > {
      Gtk.main_quit();
      return true;
    });

    // Init stage
    m_stage = (Clutter.Stage)get_stage();
    m_stage.use_alpha = true;
    m_stage.color = Clutter.Color(){alpha = 128};
    m_stage.opacity = 255;

    m_toplevel.show_all();
  }

 public
  void start() {
    /*
    if (false) {
        m_stage.animate(
            Clutter.AnimationMode.LINEAR, 5000,
            opacity: 255);
    }
    */
  }

 public
  override void size_allocate(Gtk.Allocation allocation) {
    base.size_allocate(allocation);
    if (m_allocation != allocation) {
      var frame = new Frame();
      frame.x = 10;
      frame.y = 10;
      frame.width = 200;
      frame.height = 200;
      m_stage.add(frame);
      /*
      frame.animate(
          Clutter.AnimationMode.LINEAR, 1000,
          width : 200.0,
          height : 200.0);
          */
      return;
    }
  }

 public
  override bool button_press_event(Gdk.EventButton e) {
    stdout.printf("button press\n");
    return true;
  }
}

extern void
clutter_x11_set_use_argb_visual(bool use_argb);

public
static void main(string[] args) {
  clutter_x11_set_use_argb_visual(true);
  GtkClutter.init(ref args);
  var switchor = new Switchor();
  switchor.start();
  Gtk.main();
}
