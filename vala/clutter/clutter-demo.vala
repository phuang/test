using Clutter;
using GtkClutter;

class ClutterDemo {

    private Stage stage;
    private Rectangle[] rectangles;

    const string[] colors = {
        "blanched almond",
        "OldLace",
        "MistyRose",
        "White",
        "LavenderBlush",
        "CornflowerBlue",
        "chartreuse",
        "chocolate",
        "light coral",
        "medium violet red",
        "LemonChiffon2",
        "RosyBrown3"
    };

    public ClutterDemo () {
        var window = new Gtk.Window(Gtk.WindowType.POPUP);
        window.set_visual(Gdk.Screen.get_default().get_rgba_visual());
        window.resize(800, 600);
        window.move(100, 100);
        window.draw.connect((w,cr) => { return true; });
        var embed = new GtkClutter.Embed();
        window.add(embed);
        stage = (Clutter.Stage)embed.get_stage();
        // stage = Stage.get_default ();

        rectangles = new Rectangle[colors.length];
        stage.hide.connect (Gtk.main_quit);

        create_rectangles ();
        stage.use_alpha = true;
        stage.color = Color () { alpha = 0 };
        window.show_all ();
        // window.get_window().set_background_rgba({0.0, 0.0, 0.0, 0.0});
    }

    private void create_rectangles () {
        for (int i = 0; i < colors.length; i++) {
            var r = new Rectangle ();

            r.width = r.height = stage.height / colors.length;
            r.color = Color.from_string (colors[i]);
            r.anchor_gravity = Gravity.CENTER;
            r.y = i * r.height + r.height / 2;

            stage.add_actor (r);

            rectangles[i] = r;
        }
    }

    public void start () {
        var animations = new Animation[rectangles.length];
        for (int i = 0; i < rectangles.length; i++) {
            animations[i] = rectangles[i].animate (
                                      AnimationMode.LINEAR, 2000,
                                      x: stage.width / 2,
                                      rotation_angle_z: 500.0);
        }
        animations[animations.length - 1].completed.connect (() => {
            var text = new Text.full ("Bitstream Vera Sans 40",
                                      "Congratulations!",
                                      Color.from_string ("white"));

            text.anchor_gravity = Gravity.CENTER;
            text.x = stage.width / 2;
            text.y = -text.height;    // Off-stage
            stage.add_actor (text);
            var animation = text.animate (AnimationMode.EASE_OUT_BOUNCE, 1000,
                                          y: stage.height / 2);
            animation.completed.connect(Gtk.main_quit);


            for (int i = 0; i < rectangles.length; i++) {
                rectangles[i].animate (
                        AnimationMode.EASE_OUT_BOUNCE, 3000,
                        x: Random.next_double () * stage.width,
                        y: Random.next_double () * stage.height / 2
                                                 + stage.height / 2,
                        rotation_angle_z: rectangles[i].rotation_angle_z,
                        opacity: 0);
            }
        });
    }
}

extern void clutter_x11_set_use_argb_visual(bool use_argb);

void main (string[] args) {
    clutter_x11_set_use_argb_visual(true);
    Gtk.init(ref args);
    GtkClutter.init (ref args);
    var demo = new ClutterDemo ();
    demo.start ();
    Gtk.main();
}
