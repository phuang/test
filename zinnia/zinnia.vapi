[CCode (cheader_filename = "zinnia.h")]
namespace Zinnia {
  [CCode (free_function = "zinnia_character_destroy", cname = "zinnia_character_t", cprefix = "zinnia_character_")]
  [Compact]
  class Character {
    [CCode (cname = "zinnia_character_new")]
    public Character();

    public void set_width(size_t width);
    public void set_height(size_t width);

    public void add(size_t id, int x, int y);
  }

  [CCode (free_function = "zinnia_result_destroy", cname = "zinnia_result_t", cprefix = "zinnia_result_")]
  [Compact]
  class Result {
    public unowned string value(size_t i);
    public float score(size_t i);
    public size_t size();
  }

  [CCode (free_function = "zinnia_recognizer_destroy", cname = "zinnia_recognizer_t", cprefix = "zinnia_recognizer_")]
  [Compact]
  class Recognizer {
    [CCode (cname = "zinnia_recognizer_new")]
    public Recognizer();

    public int open(string name);
    public Result classify(Character character, int nbest);
  }
}

