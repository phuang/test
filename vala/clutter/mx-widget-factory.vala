using Mx;

Clutter.Actor create_main_content () {
    var table = new Table ();
    table.column_spacing = 24;
    table.row_spacing = 24;

    /* button */
    var button = new Button.with_label ("Button");
    table.add_actor_with_properties (button, 0, 0, "y-fill", false);

    /* entry */
    var entry = new Entry.with_text ("Entry");
    table.add_actor_with_properties (entry, 0, 1, "y-fill", false);

    /* combo */
    var combo = new ComboBox ();
    combo.active_text = "Combo Box";
    combo.append_text ("Hello");
    combo.append_text ("Dave");
    table.add_actor_with_properties (combo, 0, 2, "y-fill", false);

    /* scrollbar */
    var adjustment = new Adjustment.with_values (0, 0, 10, 1, 1, 1);
    var scrollbar = new ScrollBar.with_adjustment (adjustment);
    scrollbar.height = 22.0f;
    table.add_actor_with_properties (scrollbar, 1, 0, "y-fill", false);

    /* progress bar */
    var progressbar = new ProgressBar ();
    progressbar.progress = 0.7;
    table.add_actor_with_properties (progressbar, 1, 1, "y-fill", false);

    /* slider */
    var slider = new Slider ();
    table.add_actor_with_properties (slider, 1, 2, "y-fill", false);

    slider.notify["value"].connect (() => {
        progressbar.progress = slider.value;
    });

    /* path bar */
    var pathbar = new PathBar ();
    pathbar.push ("");
    pathbar.push ("Path");
    pathbar.push ("Bar");
    table.add_actor_with_properties (pathbar, 2, 0, "y-fill", false);

    /* expander */
    var expander = new Expander ();
    table.add_actor_with_properties (expander, 2, 1, "y-fill", false);
    expander.label = "Expander";
    expander.add_actor (new Label.with_text ("Hello"));

    /* toggle */
    var toggle = new Toggle ();
    table.add_actor_with_properties (toggle, 2, 2, "y-fill", false);

    /* toggle button */
    var togglebutton = new Button.with_label ("Toggle");
    togglebutton.is_toggle = true;
    table.add_actor_with_properties (togglebutton, 3, 0, "y-fill", false);

    /* check button */
    var checkbutton = new Button ();
    checkbutton.set_style_class ("check-box");
    checkbutton.is_toggle = true;
    table.add_actor_with_properties (checkbutton, 3, 1, "y-fill", false, "x-fill", false);

    /* vertical scroll bar */
    adjustment = new Adjustment.with_values (0, 0, 10, 1, 1, 1);
    scrollbar = new ScrollBar.with_adjustment (adjustment);
    scrollbar.orientation = Orientation.VERTICAL;
    scrollbar.width = 22.0f;
    table.add_actor_with_properties (scrollbar, 0, 3, "row-span", 3);

    var frame = new Frame ();
    frame.child = table;

    return frame;
}

int main (string [] args) {
    var app = new Mx.Application (ref args, "Widget Factory", 0);
    var window = app.create_window ();
    window.clutter_stage.set_size (500, 300);

    var toolbar = window.get_toolbar ();
    var hbox = new BoxLayout ();
    toolbar.add_actor (hbox);

    var button = new Button.with_label ("Click me");
    button.tooltip_text = "Please click this button!";
    button.clicked.connect (() => {
        button.label = "Thank you!";
    });

    var combo = new ComboBox ();
    combo.append_text ("Africa");
    combo.append_text ("Antarctica");
    combo.append_text ("Asia");
    combo.append_text ("Australia");
    combo.append_text ("Europe");
    combo.append_text ("North America");
    combo.append_text ("South America");
    combo.index = 0;
    combo.notify["index"].connect (() => {
        stdout.printf ("Selected continent: %s\n", combo.active_text);
    });

    hbox.add (button, combo);
    window.child = create_main_content ();

    window.clutter_stage.show ();
    app.run ();

    return 0;
}
