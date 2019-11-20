#include <gtk/gtk.h>

static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *grid;
  GtkWidget *notebook;
  GtkWidget *frame;
  GtkWidget *label;

  // create new window
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW (window), "System Monitor");
  gtk_window_set_default_size(GTK_WINDOW (window), 600, 600);

  // create a new notebook
  notebook = gtk_notebook_new();
  gtk_container_add(GTK_CONTAINER (window), notebook);

  // set tab position
  gtk_notebook_set_tab_pos(GTK_NOTEBOOK (notebook), GTK_POS_TOP);

  // create System frame and label
  frame = gtk_frame_new("Info");
  label = gtk_label_new("System");

  // add page
  gtk_notebook_append_page(GTK_NOTEBOOK (notebook), frame, label);

  // create Processes frame and label
  frame = gtk_frame_new("Info");
  label = gtk_label_new("Processes");

  // add page
  gtk_notebook_append_page(GTK_NOTEBOOK (notebook), frame, label);
  
  // create Resources frame and label
  frame = gtk_frame_new("Info");
  label = gtk_label_new("Resources");

  // add page
  gtk_notebook_append_page(GTK_NOTEBOOK (notebook), frame, label);

  // create File Systems frame and label
  frame = gtk_frame_new("Info");
  label = gtk_label_new("File Systems");

  // add pages
  gtk_notebook_append_page(GTK_NOTEBOOK (notebook), frame, label);

  gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run(G_APPLICATION (app), argc, argv);
  g_object_unref(app);

  return status;
}
