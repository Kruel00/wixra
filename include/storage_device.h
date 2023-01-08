#include <gtk/gtk.h>
#include <stdbool.h>

typedef enum storage_device_type {

	INTERNAL_SG_DEVICE,
	EXTERNAL_SG_DEVICE,
	NVME_DEVICE,
	INTERNAL_MMC_DEVICE,
	EXTERNAL_MMC_DEVICE,
	UNKNOWN_DEVICE

} StorageDeviceType;

typedef struct mount_point_s {

	char path[512];

} mount_point_t;

typedef struct partition_s {
	char name[64];
	int postfix;
	char filesystem_type[16];
	char filesystem_uuid[64];
	char label[64];

	unsigned long long int start_sector;
	unsigned long long int total_sectors;
	unsigned long long int sector_size;

	unsigned long long int capacity_bytes;
	unsigned long long int used_bytes;

	mount_point_t *mount_points;
	size_t mount_point_count;
} partition_t;


typedef struct storage_device_s {

	char name[32];
	char sys_path[512];
	char partition_table_type[32];
	char partition_table_uuid[64];
	gchar serial[256];
	char model[128];
	char vendor[128];
	char bus[32];
	char usb_driver[32];
	char type_attribute[32];
	gchar device_capacity_gb[12];

	unsigned long long int total_sectors;
	unsigned long long int sector_size;

	unsigned long long int capacity_bytes;
	unsigned long long int used_bytes;

	char device_state[64];
	bool is_boot;

	StorageDeviceType type;
	gchar sg_name[32];
	char nvme_short_name[32];

    bool is_erased;

    char erasing_status[8];

	partition_t *partitions;
	size_t partition_count;

} storage_device_t;


typedef struct storage_device_list_s
{
    storage_device_t *device;
    int count;
}storage_device_list_t;

void init_storage_device(storage_device_t * const device);
void scan_device(storage_device_list_t * const device_list);
int alloc_storage_device_list(storage_device_list_t *const device_list, size_t partition_list_size);
