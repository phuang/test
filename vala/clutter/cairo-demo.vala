/* Cairo Vala Sample Code */
using Cairo;
using Gtk;

public
class CairoSample : Window {
  const int SIZE = 30;
  DrawingArea drawingarea;

  construct {
    title = "Cairo Vala Demo";
    delete_event.connect((w, e) = > {
      Gtk.main_quit();
      return true;
    });
    set_default_size(450, 550);

    create_widgets();
  }

 public
  void create_widgets() {
    drawingarea = new DrawingArea();
    drawingarea.draw.connect(expose);

    add(drawingarea);
  }

 private
  bool expose(Gtk.Widget da, Context ctx) {
    ctx.set_source_rgb(0, 0, 0);

    ctx.set_line_width(SIZE / 4);
    ctx.set_tolerance(0.1);

    ctx.set_line_join(LineJoin.ROUND);
    ctx.set_dash(new double[]{SIZE / 4.0, SIZE / 4.0}, 2);
    stroke_shapes(ctx, 0, 0);

    ctx.set_dash(null, 0);
    stroke_shapes(ctx, 0, 3 * SIZE);

    ctx.set_line_join(LineJoin.BEVEL);
    stroke_shapes(ctx, 0, 6 * SIZE);

    ctx.set_line_join(LineJoin.MITER);
    stroke_shapes(ctx, 0, 9 * SIZE);

    fill_shapes(ctx, 0, 12 * SIZE);

    ctx.set_line_join(LineJoin.BEVEL);
    fill_shapes(ctx, 0, 15 * SIZE);
    ctx.set_source_rgb(1, 0, 0);
    stroke_shapes(ctx, 0, 15 * SIZE);
    return true;
  }

 public
  void stroke_shapes(Context ctx, int x, int y) {
    draw_shapes(ctx, x, y, false);
  }

 public
  void fill_shapes(Context ctx, int x, int y) { draw_shapes(ctx, x, y, true); }

 public
  void draw_shapes(Context ctx, int x, int y, bool fill) {
    ctx.save();

    ctx.new_path();
    ctx.translate(x + SIZE, y + SIZE);
    bowtie(ctx);
    if (fill)
      ctx.fill();
    else
      ctx.stroke();

    ctx.new_path();
    ctx.translate(3 * SIZE, 0);
    square(ctx);
    if (fill)
      ctx.fill();
    else
      ctx.stroke();

    ctx.new_path();
    ctx.translate(3 * SIZE, 0);
    triangle(ctx);
    if (fill)
      ctx.fill();
    else
      ctx.stroke();

    ctx.new_path();
    ctx.translate(3 * SIZE, 0);
    inf(ctx);
    if (fill)
      ctx.fill();
    else
      ctx.stroke();

    ctx.restore();
  }

 public
  void triangle(Context ctx) {
    ctx.move_to(SIZE, 0);
    ctx.rel_line_to(SIZE, 2 * SIZE);
    ctx.rel_line_to(-2 * SIZE, 0);
    ctx.close_path();
  }

 public
  void square(Context ctx) {
    ctx.move_to(0, 0);
    ctx.rel_line_to(2 * SIZE, 0);
    ctx.rel_line_to(0, 2 * SIZE);
    ctx.rel_line_to(-2 * SIZE, 0);
    ctx.close_path();
  }

 public
  void bowtie(Context ctx) {
    ctx.move_to(0, 0);
    ctx.rel_line_to(2 * SIZE, 2 * SIZE);
    ctx.rel_line_to(-2 * SIZE, 0);
    ctx.rel_line_to(2 * SIZE, -2 * SIZE);
    ctx.close_path();
  }

 public
  void inf(Context ctx) {
    ctx.move_to(0, SIZE);
    ctx.rel_curve_to(0, SIZE, SIZE, SIZE, 2 * SIZE, 0);
    ctx.rel_curve_to(SIZE, -SIZE, 2 * SIZE, -SIZE, 2 * SIZE, 0);
    ctx.rel_curve_to(0, SIZE, -SIZE, SIZE, -2 * SIZE, 0);
    ctx.rel_curve_to(-SIZE, -SIZE, -2 * SIZE, -SIZE, -2 * SIZE, 0);
    ctx.close_path();
  }

  static int main(string[] args) {
    Gtk.init(ref args);

    var cairosample = new CairoSample();
    cairosample.show_all();

    Gtk.main();
    return 0;
  }
}
