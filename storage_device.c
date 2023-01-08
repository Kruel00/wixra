#include <stdlib.h>
#include <stdio.h>
#include <libudev.h>

#include "storage_device.h"

struct udev_enumerate *udev_enumerate_new(struct udev *udev);
struct udev_list_entry *u_entry = NULL;
size_t devices_count = 0U;

void scan_device(storage_device_list_t * const device_list){
    
    
    printf("Scan device\n");
    
    {
        struct udev *u_dev = udev_new();

        if(u_dev != NULL){

            struct udev_enumerate *u_enumerate = udev_enumerate_new(u_dev);

            if(u_enumerate != NULL){

                udev_enumerate_add_match_subsystem(u_enumerate, "block");
				udev_enumerate_add_match_sysname(u_enumerate, "sd?");
				udev_enumerate_add_match_sysname(u_enumerate, "nvme?n?");
				udev_enumerate_add_match_sysname(u_enumerate, "mmcblk?");

                if(udev_enumerate_scan_devices(u_enumerate) >= 0) {
                    
                    struct udev_list_entry *u_last_entry = udev_enumerate_get_list_entry(u_enumerate);
                    
                    if(u_last_entry != NULL) {

                        struct udev_list_entry *u_entry = NULL;
                        size_t devices_count = 0U;

                        udev_list_entry_foreach(u_entry, u_last_entry) {
							++devices_count;
						}

                        if(alloc_storage_device_list(device_list, devices_count) == EXIT_SUCCESS) {
                            
                            device_list->count = 0U;
                            u_entry = NULL;

                            udev_list_entry_foreach(u_entry, u_last_entry) {

                                const size_t i_dev = device_list->count;

                                printf("device %s\n", udev_list_entry_get_name(u_entry));  
                            }

                        }

                        printf("Cantidad de devices %li\n",devices_count);

                    }

                }
            }
        }
            


    }

}

int alloc_storage_device_list(storage_device_list_t *const device_list, size_t partition_list_size)
{

    if (partition_list_size < 1U)
        return EXIT_FAILURE;

    device_list->device = (storage_device_t *)malloc(sizeof(storage_device_t) * partition_list_size);

    if (device_list->device == NULL)
        return EXIT_FAILURE;

    for (size_t i = 0U; i < partition_list_size; ++i)
    {
        init_storage_device(&device_list->device[i]);
    }
    device_list->count = partition_list_size;

    return EXIT_SUCCESS;
}

void init_storage_device(storage_device_t * const device) {

	device->name[0] = '\0';
	device->sys_path[0] = '\0';
	device->partition_table_type[0] = '\0';
	device->partition_table_uuid[0] = '\0';
	device->serial[0] = '\0';
	device->model[0] = '\0';
	device->vendor[0] = '\0';
	device->bus[0] = '\0';
	device->usb_driver[0] = '\0';
	device->type_attribute[0] = '\0';
	device->nvme_short_name[0] = '\0';
	device->is_boot = false;
	device->type = UNKNOWN_DEVICE;
	device->sg_name[0] = '\0';
	device->capacity_bytes = 0ULL;
	device->used_bytes = 0ULL;
	device->total_sectors = 0ULL;
	device->sector_size = 0ULL;

	device->partitions = NULL;
	device->partition_count = 0U;
}
