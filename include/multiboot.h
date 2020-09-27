#ifndef __MULTIBOOT_H__
#define __MULTIBOOT_H__

#include <stdint.h>

#define MULTIBOOT_FLAG_MEM     0x001
#define MULTIBOOT_FLAG_DEVICE  0x002
#define MULTIBOOT_FLAG_CMDLINE 0x004
#define MULTIBOOT_FLAG_MODS    0x008
#define MULTIBOOT_FLAG_AOUT    0x010
#define MULTIBOOT_FLAG_ELF     0x020
#define MULTIBOOT_FLAG_MMAP    0x040
#define MULTIBOOT_FLAG_CONFIG  0x080
#define MULTIBOOT_FLAG_LOADER  0x100
#define MULTIBOOT_FLAG_APM     0x200
#define MULTIBOOT_FLAG_VBE     0x400

typedef struct
{
  uint32_t flags;
  uint32_t mem_lower;
  uint32_t mem_upper;
  uint32_t boot_device;
  uint32_t cmdline;
  uint32_t mods_count;
  uint32_t mods_addr;
  uint32_t num;
  uint32_t size;
  uint32_t addr;
  uint32_t shndx;
  uint32_t mmap_length;
  uint32_t mmap_addr;
  uint32_t drives_length;
  uint32_t drives_addr;
  uint32_t config_table;
  uint32_t boot_loader_name;
  uint32_t apm_table;
  uint32_t vbe_control_info;
  uint32_t vbe_mode_info;
  uint16_t vbe_mode;
  uint16_t vbe_interface_seg;
  uint16_t vbe_interface_off;
  uint16_t vbe_interface_len;
  uint64_t framebuffer_addr;
  uint32_t framebuffer_pitch;
  uint32_t framebuffer_width;
  uint32_t framebuffer_height;
  uint8_t framebuffer_bpp;
} __attribute__((packed)) multiboot_t;

typedef struct
{
  uint32_t size;
  uint32_t base_addr_low;
  uint32_t base_addr_high;
  uint32_t length_low;
  uint32_t length_high;
  uint32_t type;
} __attribute__((packed)) mmap_entry_t;

typedef struct vbe_info {
  char signature[4];   // must be "VESA" to indicate valid VBE support
  uint16_t version;         // VBE version; high byte is major version, low byte is minor version
  uint32_t oem;         // segment:offset pointer to OEM
  uint32_t capabilities;      // bitfield that describes card capabilities
  uint32_t video_modes;      // segment:offset pointer to list of supported video modes
  uint16_t video_memory;      // amount of video memory in 64KB blocks
  uint16_t software_rev;      // software revision
  uint32_t vendor;         // segment:offset to card vendor string
  uint32_t product_name;      // segment:offset to card model name
  uint32_t product_rev;      // segment:offset pointer to product revision
  char reserved[222];      // reserved for future expansion
  char oem_data[256];      // OEM BIOSes store their strings in this area
}__attribute__ ((packed)) vbe_info_t;

typedef struct mode_info_block {
  uint16_t attributes;
  uint8_t windowA, windowB;
  uint16_t granularity;
  uint16_t windowSize;
  uint16_t segmentA, segmentB;
  uint32_t winFuncPtr;
  uint16_t pitch;

  uint16_t resolutionX, resolutionY;
  uint8_t wChar, yChar, planes, bpp, banks;
  uint8_t memoryModel, bankSize, imagePages;
  uint8_t reserved0;

  uint8_t readMask, redPosition;
  uint8_t greenMask, greenPosition;
  uint8_t blueMask, bluePosition;
  uint8_t reservedMask, reservedPosition;
  uint8_t directColorAttributes;

  // linear frame buffer
  uint32_t physbase;
  uint32_t offScreenMemOff;
  uint16_t offScreenMemSize;
  uint8_t  reserved1 [206];
} mode_info_t;

#endif