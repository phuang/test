int main (string[] args)
{
  Clutter.init (ref args);

  if (args.length != 2 && args.length != 3)
    {
      stderr.printf ("usage: %s <ply-file> [texture]\n", args[0]);
      return 1;
    }

  var stage = Clutter.Stage.get_default ();

  try
    {
      var model = new Mash.Model.from_file (Mash.DataFlags.NONE, args[1]);

      if (args.length > 2)
        {
          try
            {
              var texture = new Cogl.Texture.from_file (args[2], Cogl.TextureFlags.NONE, Cogl.PixelFormat.ANY);
              var material = new Cogl.Material ();
              material.set_layer (0, texture);
              model.set_material (material);
            }
          catch (Error e)
          {
            warning (e.message);
          }
        }

      model.set_size (stage.width * 0.7f, stage.height * 0.7f);
      model.set_position (stage.width * 0.15f, stage.height * 0.15f);

      var center_vertex = Clutter.Vertex ();
      center_vertex.x = stage.width * 0.35f;
      center_vertex.y = 0.0f;
      center_vertex.z = 0.0f;

      var anim = model.animate (
            Clutter.AnimationMode.LINEAR, 3000,
                "rotation-angle-x", 360.0f,
                "rotation-angle-y", 360.0f,
                "fixed::rotation-center-y", ref center_vertex);
      anim.loop = true;

      stage.add_actor (model);

      /* Enable depth testing only for this actor */
      model.paint.connect (() => { Cogl.set_depth_test_enabled (true); });
      model.paint.connect_after (() => { Cogl.set_depth_test_enabled (false); });
    }
  catch (Error e)
  {
    warning (e.message);
  }

  stage.show ();

  Clutter.main ();

  return 0;
}
