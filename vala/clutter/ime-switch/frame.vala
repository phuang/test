using Math;

class Frame : Clutter.CairoTexture {
    public Frame() {
        auto_resize = true;
    }

    public override bool draw(Cairo.Context cr) {
        Clutter.Geometry geom = get_allocation_geometry();

        cr.new_path();
        double radius = 10;
        cr.move_to(radius, 0);
        cr.line_to(geom.width - radius, 0);
        cr.arc(geom.width - radius, radius, radius, 3 * PI / 2 , 2 * PI);
        cr.line_to(geom.width, geom.height - radius);
        cr.arc(geom.width - radius, geom.height - radius, radius, 0.0, PI / 2);
        cr.line_to(radius, geom.height);
        cr.arc(radius, geom.height - radius, radius, PI / 2, PI);
        cr.line_to(0, radius);
        cr.arc(radius, radius, radius, PI, 3 * PI / 2);

        cr.close_path();

        cr.set_line_width(1.0);
        cr.set_source_rgba(0.0, 0.0, 0.0, 0.8);
        cr.fill_preserve();
        cr.set_source_rgba(1.0, 1.0, 1.0, 0.5);
        cr.stroke();
        
        return true;
    }
}


