[CCode (cheader_filename = "zinnia.h")]
namespace Zinnia {
  [CCode (free_function = "zinnia_recognizer_destroy", cname = "zinnia_recognizer_t", cprefix = "zinnia_recognizer_")]
  [Compact]
  class Recognizer {
    [CCode (cname = "zinnia_recognizer_new")]
    public Recognizer();

    public int open(string name);

  }
}



