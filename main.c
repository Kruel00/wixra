//
// Created By:  Javier Castorena
// Year:        2023
//

#include <gtk/gtk.h>
#include <stdlib.h>
#include "main.h"

#define DEBUG 1

//global define
GtkWidget   *window;
GtkWidget   *fixed1;
GtkWidget   *fixed2;
GtkWidget   *fixed3;
GtkWidget   *button1;
GtkWidget   *button2;
GtkWidget   *button3;
GtkWidget   *button4;
GtkWidget   *stack1;
GtkBuilder  *builder;

int main(int argc, char **argv){

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("../wixra_window.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);

    fixed1  = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
    fixed2 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed2"));
    fixed3 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed3"));
    button1 = GTK_WIDGET(gtk_builder_get_object(builder, "btn1_next"));
    button2 = GTK_WIDGET(gtk_builder_get_object(builder, "btn2_back"));
    button3 = GTK_WIDGET(gtk_builder_get_object(builder, "btn2_next"));
    button4 = GTK_WIDGET(gtk_builder_get_object(builder, "btn3_back"));
    stack1  = GTK_WIDGET(gtk_builder_get_object(builder,"stack1"));

    gtk_builder_connect_signals(builder,NULL);

    gtk_widget_show(window);

    gtk_main();

    return 0;
}

void on_button_next_click(GtkButton *b){
    printf("Le diste al boton next\n");
}


