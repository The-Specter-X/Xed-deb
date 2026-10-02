#include <xed/xed-document.h>

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);
    XedDocument *document = xed_document_new();
    g_assert_true(XED_IS_DOCUMENT(document));
    GtkTextBuffer *buffer = GTK_TEXT_BUFFER(document);
    gtk_text_buffer_set_text(buffer, "package test", -1);
    g_assert_cmpint(gtk_text_buffer_get_char_count(buffer), ==, 12);
    g_object_unref(document);
    return 0;
}
