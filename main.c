// File: main.c
// Created By:  Javier Castorena
// Year:        2023
//
//****************
#include <stdlib.h>
#include <gtk/gtk.h>

//***************
#include "main.h"
#include "gui.h"

#define DEBUG 1

int main(int argc, char **argv){
    
    if(NULL == argv[1]){
        gtk_init(&argc, &argv);
        load_css();
        load_gui(argc, argv);
    }
    else{
        printf("Argumento Revicido: %s\n",argv[1]);
    }
    
    return 0;
}


