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
#include "storage_device.h"

#define DEBUG 1

int main(int argc, char **argv){
    
    if(NULL == argv[1]){
        gtk_init(&argc, &argv);
        load_css();
        load_gui(argc, argv);
    }
    else{
        printf("Arg: %s\n",argv[1]);
        storage_device_list_t dev_list;
        scan_device(&dev_list);
    }
    
    return 0;
}


