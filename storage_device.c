#include <stdlib.h>
#include <stdio.h>
#include <libudev.h>
#include <blkid/blkid.h>

#include "storage_device.h"
#define SD_DEVICE_BEGIN "/dev/sd"
#define SG_DEVICE_BEGIN "/dev/sg"
#define NVME_DEVICE_BEGIN "/dev/nvme"
#define MMC_DEVICE_BEGIN "/dev/mmcblk"
#define USB_BUS "usb"
#define USB_STICK_DRIVER "usb-storage"
#define USB_STORAGE_DRIVER "uas"
#define MMC_ATTRIBUTE_TYPE "MMC"
#define UNKNOWN_FILESYSTEM "unknown"
#define SUDO "sudo -A "
#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

struct udev_enumerate *udev_enumerate_new(struct udev *udev);
struct udev_list_entry *u_entry = NULL;
size_t devices_count = 0U;

void replace_all_chars(char *const string, char delete, char add)
{
    char *temp = string;
    while ((temp = strchr(temp, delete)))
        *temp = add;
}

int detect_storage_device_capacity(storage_device_t * const device) {
	return get_device_capacity_bytes(device->name, &device->capacity_bytes, &device->total_sectors, &device->sector_size);
}

int scan_device(storage_device_list_t * const device_list){
    
    
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
                                strcpy(device_list->device[i_dev].sys_path, udev_list_entry_get_name(u_entry));

								struct udev_device *u_device = udev_device_new_from_syspath(u_dev, device_list->device[i_dev].sys_path);

                                if(u_device != NULL){
                                    struct udev_list_entry *u_entry = udev_device_get_properties_list_entry(u_device);
                                    while(u_entry != NULL) {
                                        if(strcmp(udev_list_entry_get_name(u_entry), "ID_SERIAL") == 0) {
											if(device_list->device[i_dev].serial[0] == '\0') {
												strcpy(device_list->device[i_dev].serial, udev_list_entry_get_value(u_entry));
											}
										} else if(strcmp(udev_list_entry_get_name(u_entry), "ID_SERIAL_SHORT") == 0) {
											strcpy(device_list->device[i_dev].serial, udev_list_entry_get_value(u_entry));
                                            printf("Dato: %s\n",udev_list_entry_get_value(u_entry));
										} else if(strcmp(udev_list_entry_get_name(u_entry), "ID_MODEL") == 0) {
											strcpy(device_list->device[i_dev].model, udev_list_entry_get_value(u_entry));
											replace_all_chars(device_list->device[i_dev].model, '_', ' ');
										} else if(strcmp(udev_list_entry_get_name(u_entry), "ID_VENDOR") == 0) {
											strcpy(device_list->device[i_dev].vendor, udev_list_entry_get_value(u_entry));
											replace_all_chars(device_list->device[i_dev].vendor, '_', ' ');
										} else if(strcmp(udev_list_entry_get_name(u_entry), "ID_PART_TABLE_TYPE") == 0) {
											strcpy(device_list->device[i_dev].partition_table_type, udev_list_entry_get_value(u_entry));
										} else if(strcmp(udev_list_entry_get_name(u_entry), "ID_PART_TABLE_UUID") == 0) {
											strcpy(device_list->device[i_dev].partition_table_uuid, udev_list_entry_get_value(u_entry));
										} else if(strcmp(udev_list_entry_get_name(u_entry), "ID_BUS") == 0) {
											strcpy(device_list->device[i_dev].bus, udev_list_entry_get_value(u_entry));
										} else if(strcmp(udev_list_entry_get_name(u_entry), "ID_USB_DRIVER") == 0) {
											strcpy(device_list->device[i_dev].usb_driver, udev_list_entry_get_value(u_entry));
										} else if(strcmp(udev_list_entry_get_name(u_entry), "DEVNAME") == 0) {
											strcpy(device_list->device[i_dev].name, udev_list_entry_get_value(u_entry));
										}
										u_entry = udev_list_entry_get_next(u_entry);
                                    }

                                    {
										struct udev_device *u_device_parent = u_device;
										while(u_device_parent != NULL) {
											const char *type = udev_device_get_sysattr_value(u_device_parent, "type");
											if((type != NULL) && (strlen(type) != 0U)) {
												strcpy(device_list->device[i_dev].type_attribute, type);
												break;
											}
											u_device_parent = udev_device_get_parent(u_device_parent);
										}
									}

                                    udev_device_unref(u_device);
									++device_list->count;
                                
                                } else {
                                    device_list->device[i_dev].sys_path[0] = '\0';
                                }
                            }
                        }
                    }
                }
                udev_enumerate_unref(u_enumerate);
            }
            udev_unref(u_dev);
        }
    }

    for(size_t i = 0U; i < device_list->count; ++i) {
		detect_storage_device_type(&device_list->device[i]);
		detect_storage_device_capacity(&device_list->device[i]);
		set_device_state(&device_list->device[i]);
		set_dev_gb(&device_list->device[i]);
	}
	return 0;

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


int get_device_capacity_bytes(char const * const device, unsigned long long int * const size_in_bytes, unsigned long long int * const total_sectors, unsigned long long int * const sector_size) {

	if(device == NULL)
		return EXIT_FAILURE;

	int ret_val = EXIT_FAILURE;

	blkid_probe hdd_probe = blkid_new_probe_from_filename(device);
	if(hdd_probe != NULL) {

		const blkid_loff_t sectors = blkid_probe_get_sectors(hdd_probe);
		if(sectors != -1L) {

			const unsigned int device_sector_size = blkid_probe_get_sectorsize(hdd_probe);

			*total_sectors = (unsigned long long int)sectors;
			*sector_size = (unsigned long long int)device_sector_size;
			*size_in_bytes = (*total_sectors) * (*sector_size);

			ret_val = EXIT_SUCCESS;
		}
		blkid_free_probe(hdd_probe);
	}
	
	return ret_val;
}

int detect_storage_nvme_short_name(storage_device_t * const device) {

	if(device == NULL)
		return EXIT_FAILURE;

	strcpy(device->nvme_short_name, device->name);
	char *last_n_char = strrchr(device->nvme_short_name, 'n');
	if(last_n_char != NULL) {
		*last_n_char = '\0';
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

int detect_storage_serial_with_usb_adapter(storage_device_t * const device) {

	if(device == NULL)
		return EXIT_FAILURE;

	int ret_val = EXIT_FAILURE;

	char hdparam_command[128];
	sprintf(hdparam_command, SUDO "hdparm -I %s", device->name);

    FILE *hdparm_process = popen(hdparam_command, "r");
    if(hdparm_process != NULL) {
        char line[1024];
        while(fgets(line, ARRAY_SIZE(line), hdparm_process) != NULL) {
            if(strstr(line, "Serial Number:") != NULL) {
                char serial[256] = { '\0' }; // TT05-0625: increased from 64 to 256 as some devices can have rather long serial numbers
                if(sscanf(line, "	Serial Number:	%s", serial) == 1) {
                    const size_t serial_length = strlen(serial);
                    if ((serial_length > 0U) && (serial_length < 30U)) {
                        strcpy(device->serial, serial);
                        ret_val = EXIT_SUCCESS;
                    }
                }
                break;
            }
        }
		if(pclose(hdparm_process) != 0) {
			ret_val = EXIT_FAILURE;
		}
    }
	return ret_val;
}

int detect_storage_device_type(storage_device_t * const device) {

	if(device == NULL)
		return EXIT_FAILURE;

	int ret_val = EXIT_FAILURE;
	device->type = UNKNOWN_DEVICE;

	if(strncmp(NVME_DEVICE_BEGIN, device->name, strlen(NVME_DEVICE_BEGIN)) == 0) {
		device->type = NVME_DEVICE;
		detect_storage_nvme_short_name(device);
		ret_val = EXIT_SUCCESS;
	}
	else if(strncmp(MMC_DEVICE_BEGIN, device->name, strlen(MMC_DEVICE_BEGIN)) == 0) {
		if(strcmp(device->type_attribute, MMC_ATTRIBUTE_TYPE) == 0) {
			device->type = INTERNAL_MMC_DEVICE;
		}
		else {
			device->type = EXTERNAL_MMC_DEVICE;
		}
		ret_val = EXIT_SUCCESS;
	}
	else if(strncmp(SD_DEVICE_BEGIN, device->name, strlen(SD_DEVICE_BEGIN)) == 0) {
		device->type = INTERNAL_SG_DEVICE;
		if(strcmp(device->bus, USB_BUS) == 0) {
			if(strcmp(device->usb_driver, USB_STICK_DRIVER) == 0) {
				device->type = EXTERNAL_SG_DEVICE;
			}
			else if(strcmp(device->usb_driver, USB_STORAGE_DRIVER) == 0) {
				detect_storage_serial_with_usb_adapter(device);
			}
		}
	}
	return ret_val;
}

void set_device_state(storage_device_t *const device){
	strcpy(device->device_state,"Idle");
}

void set_dev_gb(storage_device_t *const device){

	strcpy(device->device_capacity_gb,"500 gb");

}
