/* vi: set et ts=2 sts=2 ai : */
using Cairo;
using Gdk;
using GLib;
using Gtk;
using Zinnia;

class Path {
 public
  Path() {}

 public
  void Add(owned Gdk.Point point) { m_points += point; }

 public
  Iterator iterator() { return new Iterator(this); }

 public
  class Iterator {
   private
    int m_index;
   private
    Path m_path;

   public
    Iterator(Path path) {
      m_index = 0;
      m_path = path;
    }

   public
    bool next() { return m_index < m_path.m_points.length; }

   public
    Gdk.Point get() { return m_path.m_points[m_index++]; }
  }

  private Gdk.Point[] m_points = {};
}

class Application {
 public
  Application() {
    m_recognizer = new Zinnia.Recognizer();
    m_recognizer.open("/usr/share/zinnia/model/tomoe/handwriting-zh_CN.model");

    var vbox = new Gtk.VBox(false, 1);

    m_drawing_area = new Gtk.DrawingArea();
    vbox.pack_start(m_drawing_area, true, true);
    m_drawing_area.set_size_request(400, 400);
    m_drawing_area.add_events(Gdk.EventMask.POINTER_MOTION_MASK);
    m_drawing_area.add_events(Gdk.EventMask.KEY_PRESS_MASK);
    m_drawing_area.add_events(Gdk.EventMask.BUTTON_PRESS_MASK);
    m_drawing_area.add_events(Gdk.EventMask.BUTTON_RELEASE_MASK);
    // m_drawing_area.add_events(Gdk.EventMask.BUTTON_MOTION_MASK);

    m_drawing_area.button_press_event.connect((e) = > {
      debug("button press");
      return true;
    });

    double x = 0;
    double y = 0;
    m_drawing_area.motion_notify_event.connect((e) = > {
      if (e.x_root >= 1440 - 10 && e.y_root > 900 - 10) {
        ungrab();
        return true;
      }

      double dx = e.x_root - x;
      double dy = e.y_root - y;

      if (dx * dx + dy * dy > 10000.0) {
        m_paths += new Path();
      }

      if (m_paths.length > 0) {
        m_paths[m_paths.length - 1].Add(Gdk.Point(){
            x = (int)(e.x_root * 400 / 1440), y = (int)(e.y_root * 400 / 900)});
        m_drawing_area.queue_draw();
        if (!m_idle) {
          m_idle = true;
          GLib.Idle.add(() = > {
            classify();
            m_idle = false;
            return false;
          });
        }
        x = e.x_root;
        y = e.y_root;
      }
      return true;
    });

    m_drawing_area.key_press_event.connect((e) = > {
      debug("keypress");
      switch (e.keyval) {
        case 0xff8d:  // KP_Enter
        case 0xff0d:  // Return
          m_paths = {};
          m_drawing_area.queue_draw();
          break;
        case 0xff1b:  // Escape
          ungrab();
          break;
        default:
          break;
      }
      return true;
    });

    m_drawing_area.draw.connect(this.drawing_area_draw);
    var hbox = new Gtk.HBox(true, 1);

    var button = new Gtk.Button.with_label("Grab");
    hbox.pack_start(button, true, true);
    button.clicked.connect((e) = > { grab(); });

    button = new Gtk.Button.with_label("Back");
    hbox.pack_start(button, true, true);
    button.clicked.connect((e) = > {
      if (m_paths.length > 0) {
        m_paths.resize(m_paths.length - 1);
        m_drawing_area.queue_draw();
      }
      classify();
    });

    button = new Gtk.Button.with_label("Clear");
    hbox.pack_start(button, true, true);
    button.clicked.connect((e) = > {
      m_paths = {};
      m_drawing_area.queue_draw();
      classify();
    });

    button = new Gtk.Button.with_label("Quit");
    hbox.pack_start(button, true, true);
    button.clicked.connect((e) = > {
      Gtk.main_quit();

    });

    vbox.pack_start(hbox, false, false);
    m_window = new Gtk.Window();
    m_window.set_position(Gtk.WindowPosition.CENTER);
    m_window.add(vbox);
    m_window.delete_event.connect((e) = > {
      Gtk.main_quit();
      return true;
    });
    m_window.show_all();
    test();
  }

 private
  void test() {
    var display = Gdk.Display.get_default();
    var device_manager = display.get_device_manager();
    var devices = device_manager.list_devices(Gdk.DeviceType.MASTER);
    foreach (var dev in devices) {}
  }

 private
  void grab() {
    var display = Gdk.Display.get_default();
    var device_manager = display.get_device_manager();
    var devices = device_manager.list_devices(Gdk.DeviceType.MASTER);
    foreach (var dev in devices) {
      dev.grab(m_drawing_area.get_window(), Gdk.GrabOwnership.WINDOW, false,
               Gdk.EventMask.POINTER_MOTION_MASK |
                   Gdk.EventMask.BUTTON_PRESS_MASK |
                   Gdk.EventMask.KEY_PRESS_MASK,
               new Gdk.Cursor.for_display(Gdk.Display.get_default(),
                                          Gdk.CursorType.BLANK_CURSOR),
               Gdk.CURRENT_TIME);
    }

    Gtk.grab_add(m_drawing_area);

    try {
      GLib.Process.spawn_command_line_sync(
          "/usr/bin/xinput set-mode 11 ABSOLUTE");
    } catch (GLib.Error e) {
      debug("xinput error");
    }
  }

 private
  void ungrab() {
    var display = Gdk.Display.get_default();
    var device_manager = display.get_device_manager();
    var devices = device_manager.list_devices(Gdk.DeviceType.MASTER);
    foreach (var dev in devices) { dev.ungrab(Gdk.CURRENT_TIME); }
    try {
      GLib.Process.spawn_command_line_sync(
          "/usr/bin/xinput set-mode 11 RELATIVE");
    } catch (GLib.Error e) {
      debug("xinput error");
    }
    Gtk.grab_remove(m_drawing_area);
  }

 private
  bool drawing_area_draw(Cairo.Context cr) {
    foreach (var path in m_paths) {
      var i = 0;
      foreach (var p in path) {
        if (i == 0)
          cr.move_to(p.x, p.y);
        else
          cr.line_to(p.x, p.y);
        i++;
      }
    }
    cr.set_source_rgb(0.0, 0.0, 1.0);
    cr.stroke();
    return true;
  }

 private
  void classify() {
    var character = new Zinnia.Character();
    character.set_width(400);
    character.set_height(400);
    int path_id = 0;
    foreach (var path in m_paths) {
      foreach (var p in path) { character.add(path_id, p.x, p.y); }
      path_id++;
    }

    Zinnia.Result result = m_recognizer.classify(character, 10);
    debug("===========");
    for (int i = 0; result != null && i < result.size(); i++) {
      debug("%d = %s", i, result.value(i));
    }
  }

 private
  Gtk.Window m_window;
 private
  Zinnia.Recognizer m_recognizer;
 private
  Gtk.DrawingArea m_drawing_area;
 private
  Path[] m_paths = {};
 private
  bool m_idle = false;
}

public void
main(string[] argv) {
  Gtk.init(ref argv);
  Application app = new Application();
  Gtk.main();
  app = null;
}
