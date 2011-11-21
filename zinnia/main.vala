using GLib;
using Gdk;
using Gtk;
using Cairo;
using Zinnia;

class Application {
  public Application() {
    mRecognizer = new Zinnia.Recognizer();
    mRecognizer.open("/usr/share/zinnia/model/tomoe/handwriting-zh_CN.model");

    var box = new Gtk.VBox(false, 1);

    mDrawingArea = new Gtk.DrawingArea();
    box.pack_end(mDrawingArea, true, true);
    mDrawingArea.set_size_request(400, 400);
    // mDrawingArea.add_events(Gdk.EventMask.POINTER_MOTION_MASK);
    mDrawingArea.add_events(Gdk.EventMask.BUTTON_PRESS_MASK);
    mDrawingArea.add_events(Gdk.EventMask.BUTTON_RELEASE_MASK);
    mDrawingArea.add_events(Gdk.EventMask.BUTTON_MOTION_MASK);
   

    mDrawingArea.button_press_event.connect((e) => {
      if (this.mLines == null)
        this.mLines = new List<List<Gdk.Point?>>(); 
      this.mLine = new List<Gdk.Point?>();
      return true;
    });

    mDrawingArea.button_release_event.connect((e) => {
      this.mLines.append((owned)this.mLine);
      mDrawingArea.queue_draw();
      return true;
    });
    
    mDrawingArea.motion_notify_event.connect((e) => {
      if (this.mLine != null) {
        var point = Gdk.Point() {
          x = (int)e.x,
          y = (int)e.y
        };
        this.mLine.append(point);
      }
      return true;
    });

    mDrawingArea.draw.connect((cr) => {
      debug("Draw");
      foreach (unowned List<Gdk.Point?> l in this.mLines) {
        int i = 0;
        foreach (unowned Gdk.Point p in l) {
          debug("x = %d, y = %d", p.x, p.y);
          if (i == 0)
            cr.move_to(p.x, p.y);
          else 
            cr.line_to(p.x, p.y);
          i ++;
        }
      }
      cr.set_source_rgb(1.0, 0.0, 0.0);
      cr.stroke();
      return true;
    });

    var button = new Gtk.Button.with_label("Clear");
    box.pack_end(button, false, false);
    
    mWindow = new Gtk.Window();
    mWindow.add(box);
    mWindow.delete_event.connect((e) => {
      Gtk.main_quit();
      return true;
    });
    mWindow.show_all();
  }

  private Gtk.Window mWindow;
  private Zinnia.Recognizer mRecognizer;
  private Gtk.DrawingArea mDrawingArea;
  private List<Gdk.Point?> mLine;
  private List<List<Gdk.Point?>> mLines;
}

public void main(string[] argv) { 
  Gtk.init(ref argv);
  Application app = new Application();
  Gtk.main();
}


