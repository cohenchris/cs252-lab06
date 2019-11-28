#include "sysinfo.h"
#include "procinfo.h"

#include <gtk/gtk.h>
#include <pthread.h>

system_info g_sys_info_struct = { 0 };

/* 0 = all processes (default)
 * 1 = active processes
 * 2 = my processes
 */

int g_toggle = 0;

void toggle_active(GtkWidget *widget, gpointer status) {
  if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM (widget))) {
    g_toggle = 1;   
  }
}

void toggle_all(GtkWidget *widget, gpointer status) {
  if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM (widget))) {
    g_toggle = 0;
  }
}

void toggle_my(GtkWidget *widget, gpointer status) {
  if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM (widget))) {
    g_toggle = 2;
  }
}

void toggle_refresh(GtkWidget *widget, gpointer status) {
}

gint create_tree_view(GtkWidget *treeview) {
  // create Processes treeview and label
  GtkTreeModel *model;
  GtkTreeIter toplevel, child;
  GtkTreeStore *treestore = gtk_tree_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
      G_TYPE_STRING, G_TYPE_STRING);
  // gtk_tree_store_append(treestore, &toplevel, NULL);

  process_info *proc_info = get_proc_info();
  /*for proc in procs:
    if proc is child of lastProc:
      gtk_tree_store_append(treestore, &child, &toplevel);
      gtk_tree_store_set(treestore, &child,
                         0, proc.proc_name,
                         1, proc.state,
                         2, proc.cpu_time,
                         3, proc.proc_id,
                         4, proc.virtual_mem,
                         -1);
   else:*/
   for (int i = 0; i < g_num_procs; i++) {
     process_info proc = proc_info[i];
     if (strcmp(proc.proc_user, "chan207") != 0) {
       continue;
     }
     char buf[1000] = {0};
     sprintf(buf, "%ld", proc.proc_id);
     char * proc_id = strdup(buf);
     gtk_tree_store_append(treestore, &toplevel, NULL);
     gtk_tree_store_set(treestore, &toplevel,
                        0, proc.proc_name,
                        1, proc.state,
                        2, proc.cpu_time,
                        3, proc_id,
                        4, proc.virtual_mem,
                        -1);
   }

  model = GTK_TREE_MODEL(treestore);
  gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), model);
  g_object_unref(model);
  printf("HELLO\n");
  return TRUE;
}

static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;

  GtkWidget *button;
  GtkWidget *notebook;
  GtkWidget *frame;
  GtkWidget *menuBar;
  GtkWidget *subMenu;
  GtkWidget *menuItem;
  GtkWidget *subItem;
  GtkWidget *label;
  GtkWidget *vbox;
  GtkWidget *grid;
  GtkWidget *text;

  GtkWidget *radio_group;
  GtkWidget *radio_button;

  GtkWidget *tog_stat_active;
  GtkWidget *tog_stat_all;
  GtkWidget *tog_stat_my;
  GtkWidget *refresh;
  GtkWidget *separator;

  GtkWidget *scroll_window;

  // create new window
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW (window), "System Monitor");
  gtk_window_set_default_size(GTK_WINDOW (window), 600, 600);

  // create box
  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER (window), vbox);

  // create new menu
  menuBar = gtk_menu_bar_new();

  menuItem = gtk_menu_item_new_with_label("Monitor");
  gtk_menu_shell_append(GTK_MENU_SHELL (menuBar), menuItem);

  menuItem = gtk_menu_item_new_with_label("Edit");
  gtk_menu_shell_append(GTK_MENU_SHELL (menuBar), menuItem);
  
  menuItem = gtk_menu_item_new_with_label("View");
  gtk_menu_shell_append(GTK_MENU_SHELL (menuBar), menuItem); 
  
  // create submenu for View
  subMenu = gtk_menu_new();
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), subMenu);

  // create a radio button group
  radio_group = gtk_radio_menu_item_new(NULL);

  // add submenu items
  tog_stat_active = gtk_radio_menu_item_new_with_label_from_widget(
      GTK_RADIO_MENU_ITEM (radio_group), "Active Processes");
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM (tog_stat_active), FALSE);
  gtk_menu_shell_append(GTK_MENU_SHELL (subMenu), tog_stat_active);

  // Active Processes signal handler
  g_signal_connect(G_OBJECT (tog_stat_active), "activate",
      G_CALLBACK(toggle_active), NULL);

  tog_stat_all = gtk_radio_menu_item_new_with_label_from_widget(
      GTK_RADIO_MENU_ITEM (radio_group), "All Processes"); 
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM (tog_stat_all), FALSE);
  gtk_menu_shell_append(GTK_MENU_SHELL (subMenu), tog_stat_all);
  
  // All Processes signal handler
  g_signal_connect(G_OBJECT (tog_stat_all), "activate",
      G_CALLBACK(toggle_all), NULL);

  tog_stat_my = gtk_radio_menu_item_new_with_label_from_widget(
      GTK_RADIO_MENU_ITEM (radio_group), "My Processes");
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM (tog_stat_my), FALSE);
  gtk_menu_shell_append(GTK_MENU_SHELL (subMenu), tog_stat_my);
 
  // My Processes signal handler
  g_signal_connect(G_OBJECT (tog_stat_my), "activate",
      G_CALLBACK(toggle_my), NULL);

  // Add separator
  separator = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL (subMenu), separator);

  // Add remaining submenu items
  subItem = gtk_check_menu_item_new_with_label("Dependencies");
  gtk_menu_shell_append(GTK_MENU_SHELL(subMenu), subItem);
  
  // Add separator
  separator = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL (subMenu), separator);
 
  subItem = gtk_menu_item_new_with_label("Memory Maps");
  gtk_widget_set_sensitive(subItem, FALSE);
  gtk_menu_shell_append(GTK_MENU_SHELL(subMenu), subItem);
  
  subItem = gtk_menu_item_new_with_label("Open Files");
  gtk_widget_set_sensitive(subItem, FALSE);
  gtk_menu_shell_append(GTK_MENU_SHELL(subMenu), subItem);
  
   // Add separator
  separator = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL (subMenu), separator);
  
  subItem = gtk_menu_item_new_with_label("Properties");
  gtk_widget_set_sensitive(subItem, FALSE);
  gtk_menu_shell_append(GTK_MENU_SHELL(subMenu), subItem);

  // Add separator
  separator = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL (subMenu), separator);
  
  refresh = gtk_menu_item_new_with_label("Refresh");
  gtk_menu_shell_append(GTK_MENU_SHELL(subMenu), refresh);

  // Refresh signal handler
  g_signal_connect(G_OBJECT (refresh), "activate", G_CALLBACK(toggle_refresh),
      NULL);

  // Add remaining menu items
  menuItem = gtk_menu_item_new_with_label("Help");
  gtk_menu_shell_append(GTK_MENU_SHELL (menuBar), menuItem);

  // add menubar to box
  gtk_box_pack_start(GTK_BOX (vbox), menuBar, FALSE, FALSE, 0);

  // create a new notebook
  notebook = gtk_notebook_new();
  
  gtk_box_pack_start(GTK_BOX (vbox), notebook, FALSE, FALSE, 0);

  // set notebook margins
  gtk_widget_set_margin_top(notebook, 20);
  gtk_widget_set_margin_start(notebook, 10);
  gtk_widget_set_margin_end(notebook, 10);
  gtk_widget_set_margin_bottom(notebook, 40);

  // set tab position
  gtk_notebook_set_tab_pos(GTK_NOTEBOOK (notebook), GTK_POS_TOP);

  // create System frame and label
  //frame = gtk_frame_new(NULL);
  grid = gtk_grid_new();

  // add system info
  char sys_info[200000];
  sprintf(sys_info, "%s\n\t%s\n\t%s\n\nHardware\n\tMemory: %sKB \
      \n\tProcessor: %s\nSystem Status\n\tAvailable disk space: %s",
      g_sys_info_struct.release_name, g_sys_info_struct.release_version,
      g_sys_info_struct.kernel_version, g_sys_info_struct.ram_size,
      g_sys_info_struct.cpu_info, g_sys_info_struct.disk_space); 

  text = gtk_label_new(sys_info);
  gtk_grid_attach(GTK_GRID (grid), text, 0, 0, 1, 1);

  label = gtk_label_new("System");

  // add page
  gtk_notebook_append_page(GTK_NOTEBOOK (notebook), grid, label);
  
  GtkTreeViewColumn *col;
  GtkCellRenderer *renderer;
  GtkWidget *treeview;

  treeview = gtk_tree_view_new();

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Process Name");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 0);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Status");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 1);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "% CPU");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 2);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Id");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 3);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Memory");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 4);

  // TODO: pass treeview to function, create treestore, add to
  // treeview
  create_tree_view(treeview);

  // create scrollable window
  scroll_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER (scroll_window), treeview);

  label = gtk_label_new("Processes");

  // add page
  gtk_notebook_append_page(GTK_NOTEBOOK (notebook), scroll_window, label);
  
  // create Resources frame and label
  frame = gtk_frame_new(NULL);
  label = gtk_label_new("Resources");

  // add page
  gtk_notebook_append_page(GTK_NOTEBOOK (notebook), frame, label);

  // create File Systems frame and label
  frame = gtk_frame_new(NULL);
  label = gtk_label_new("File Systems");

  // add pages
  gtk_notebook_append_page(GTK_NOTEBOOK (notebook), frame, label);
  
  g_timeout_add_full(G_PRIORITY_DEFAULT, 5000, create_tree_view, treeview, NULL);
  gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  /*for (int i = 0; i < 4; i++) {
    get_proc_info();
    printf("NUM: %d\n", g_num_procs);
  }
  exit(0);*/

  g_sys_info_struct = get_system_info();

  app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run(G_APPLICATION (app), argc, argv);
  g_object_unref(app);

  return status;
}
