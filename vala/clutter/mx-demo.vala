using Mx;

int main(string[] args) {
  Clutter.init(ref args);

  var stage = Clutter.Stage.get_default();
  stage.set_size(640, 480);
  stage.user_resizable = true;

  var expander = new Mx.Expander();
  expander.label = "Expander";
  stage.add_actor(expander);
  expander.set_position(10, 10);

  expander.expand_complete.connect(() = > {
    stdout.printf("Expand complete (%s)\n",
                  expander.expanded ? "open" : "closed");
  });

  var scroll = new Mx.ScrollView();
  expander.add_actor(scroll);
  scroll.set_size(320, 240);

  var grid = new Mx.Grid();
  scroll.add_actor(grid);

  for (var i = 1; i <= 50; i++) {
    grid.add_actor(new Mx.Button.with_label(@"Button $i"));
  }

  var label = new Mx.Label();
  label.set_text("Hello World");
  label.x = 300;
  stage.add(label);

  stage.show();
  Clutter.main();

  return 0;
}
