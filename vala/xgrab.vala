using Gdk;
using GLib;
using Gtk;
using X;

public
class XGrabber : GLib.Object {
 public
  signal void trigger(int id);

 public
  XGrabber() {}
}

void main(string[] argv){
  Gtk.init(ref argv);

  var grabber = new XGrabber();
  grabber.trigger.connect((g, id) =
                              > { stdout.printf("trigger id=%d\n", id); });

  var window = new Gtk.Window();
  window.show_all();
  window.destroy.connect(Gtk.main_quit);
  grabber.trigger(99);
  Gtk.main();
}
