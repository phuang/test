/* vi: set et ts=2 sts=2 ai : */
using GLib;
using Gdk;
using Gtk;
using Cairo;
using Zinnia;

class Path {
  public Path() {}

  public void Add(owned Gdk.Point point) {
    m_points += point;
  }

  public Iterator iterator() {
    return new Iterator(this);
  }

  public class Iterator {
    private int m_index;
    private Path m_path;

    public Iterator(Path path) {
      m_index = 0;
      m_path = path;
    }

    public bool next() {
      return m_index < m_path.m_points.length;
    }

    public Gdk.Point get() {
      return m_path.m_points[m_index++];
    }
  }

  private Gdk.Point[] m_points = {};
}

class Application {
  public Application() {
    m_recognizer = new Zinnia.Recognizer();
    m_recognizer.open("/usr/share/zinnia/model/tomoe/handwriting-zh_CN.model");

    var vbox = new Gtk.VBox(false, 1);

    m_drawing_area = new Gtk.DrawingArea();
    vbox.pack_start(m_drawing_area, true, true);
    m_drawing_area.set_size_request(400, 400);
    // m_drawing_area.add_events(Gdk.EventMask.POINTER_MOTION_MASK);
    m_drawing_area.add_events(Gdk.EventMask.BUTTON_PRESS_MASK);
    m_drawing_area.add_events(Gdk.EventMask.BUTTON_RELEASE_MASK);
    m_drawing_area.add_events(Gdk.EventMask.BUTTON_MOTION_MASK);
   

    m_drawing_area.button_press_event.connect((e) => {
      m_paths += new Path();
      return true;
    });

    m_drawing_area.button_release_event.connect((e) => {
      m_drawing_area.queue_draw();
      return true;
    });
    
    m_drawing_area.motion_notify_event.connect((e) => {
      m_paths[m_paths.length - 1].Add (Gdk.Point() {
        x = (int)e.x,
        y = (int)e.y
      });
      m_drawing_area.queue_draw();
      return true;
    });

    m_drawing_area.draw.connect((cr) => {
      var character = new Zinnia.Character();
      character.set_width(400);
      character.set_height(400);
      int path_id = 0;
      foreach (var path in m_paths) {
        var i = 0;
        foreach (var p in path) {
          if (i == 0)
            cr.move_to(p.x, p.y);
          else 
            cr.line_to(p.x, p.y);
          character.add(path_id, p.x, p.y);
          i ++;
        }
        path_id++;
      }
      cr.set_source_rgb(0.0, 0.0, 1.0);
      cr.stroke();

      Zinnia.Result result = m_recognizer.classify(character, 20);
      debug("===========");
      for (int i = 0; result != null && i < result.size(); i ++) {
        debug("%d = %s", i, result.value(i));
      }
      return true;
    });

    var hbox = new Gtk.HBox(true, 1);
    
    var button = new Gtk.Button.with_label("Back");
    hbox.pack_start(button, true, true);
    button.clicked.connect((e) => {
      if (m_paths.length > 0) {
        m_paths.resize(m_paths.length - 1);
        m_drawing_area.queue_draw();
      }
    });
    
    button = new Gtk.Button.with_label("Clear");
    hbox.pack_start(button, true, true);
    button.clicked.connect((e) => {
      m_paths = {};
      m_drawing_area.queue_draw();
    });
    
    vbox.pack_start(hbox, false, false);
    m_window = new Gtk.Window();
    m_window.add(vbox);
    m_window.delete_event.connect((e) => {
      Gtk.main_quit();
      return true;
    });
    m_window.show_all();
  }

  private Gtk.Window m_window;
  private Zinnia.Recognizer m_recognizer;
  private Gtk.DrawingArea m_drawing_area;
  private Path[] m_paths = {};
}

public void main(string[] argv) { 
  Gtk.init(ref argv);
  Application app = new Application();
  Gtk.main();
}

