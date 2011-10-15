class Text : Clutter.Box {
    private Clutter.Text m_text;
    public Text(string text) {
        layout_manager = new Clutter.BoxLayout();
        m_text = new Clutter.Text();
        m_text.text = text;
        m_text.color = { 255, 255, 255, 255 };
        pack(m_text,
            "x-align", Clutter.BoxAlignment.CENTER,
            "y-align", Clutter.BoxAlignment.CENTER,
            "expand", true);
    }
}

class SwitchorWidget : Clutter.CairoTexture {
}


