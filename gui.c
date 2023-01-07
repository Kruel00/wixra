// File: gui.c
// Created By:  Javier Castorena
// Year:        2023
//
#include <gtk/gtk.h>
//***
#define MAIN_WINDOW "window_main"
#define MAIN_GRID   "main_grid"
//***
//global define
GtkWidget   *window;
GtkWidget   *grid;
GtkBuilder  *builder;
//***
void load_gui(int argc, char **argv){
    //***
    builder = gtk_builder_new_from_file("../wixra_window.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, MAIN_WINDOW));
    g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
    gtk_builder_connect_signals(builder,NULL);
    //***
    grid = GTK_WIDGET(gtk_builder_get_object(builder,MAIN_GRID));
    gtk_grid_set_column_homogeneous (GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous (GTK_GRID(grid), TRUE);
    gtk_widget_show(window);
    gtk_window_fullscreen(GTK_WINDOW(window));
    gtk_main();
}

void load_css(){

    GtkCssProvider *provider;
    GdkDisplay     *display;
    GdkScreen      *screen;
    /// ***
    const gchar *css_style_file = "../style.css";
    GFile *css_fp               = g_file_new_for_path ( css_style_file );
    GError *error               = 0;
    /// ***
    provider = gtk_css_provider_new ();
    display  = gdk_display_get_default ();
    screen   = gdk_display_get_default_screen ( display );
    /// ***
    gtk_style_context_add_provider_for_screen   ( screen, GTK_STYLE_PROVIDER ( provider ), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION );
    gtk_css_provider_load_from_file             ( provider, css_fp, &error );
    /// ***
    g_object_unref ( provider );
}