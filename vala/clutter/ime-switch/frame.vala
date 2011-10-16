using Math;

class Frame : Clutter.CairoTexture {
    private static const float RADIUS = 20.0f;
    public Frame() {
    }

    public override void allocate(Clutter.ActorBox box,
                                  Clutter.AllocationFlags flags) {
        base.allocate(box, flags);
        set_surface_size((uint)floor(width + 0.5),
                         (uint)floor(height + 0.5));
        invalidate();
    }

    public override void get_preferred_width(float for_height,
                                             out float min_width,
                                             out float natural_width) {
        min_width =
        natural_width = (float)RADIUS * 2;
    }
    
    public override void get_preferred_height(float for_width,
                                              out float min_height,
                                              out float natural_height) {
        min_height =
        natural_height = (float)RADIUS * 2;
    }

    public override bool draw(Cairo.Context cr) {
        const double R = (double) RADIUS;
       
        uint w, h;
        get_surface_size(out w,out h);

        cr.new_path();
        cr.move_to(R, 0);
        cr.line_to(w - R, 0);
        cr.arc(w - R, R, R, 3 * PI / 2 , 2 * PI);
        cr.line_to(w, h - R);
        cr.arc(w - R, h - R, R, 0.0, PI / 2);
        cr.line_to(R, h);
        cr.arc(R, h - R, R, PI / 2, PI);
        cr.line_to(0, R);
        cr.arc(R, R, R, PI, 3 * PI / 2);
        cr.close_path();
        
        // Fill background
        cr.set_line_width(1.0);
        cr.set_source_rgba(0.0, 0.0, 0.0, 0.8);
        cr.fill_preserve();

        // Stroke border
        cr.set_source_rgba(1.0, 1.0, 1.0, 0.5);
        cr.stroke();
        
        return true;
    }
}


