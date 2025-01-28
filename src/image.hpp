#include "types.hpp"
#include <cstdio>
#include <cstdlib>

struct Image
{
    u32 width;
    u32 height;
    u32 *pixels;
};

#pragma pack(push, 1)
struct BitmapHeader
{
    u16 file_type;
    u32 file_size;
    u16 reserved1;
    u16 reserved2;
    u32 bitmap_offset;
    u32 size;
    s32 width;
    s32 height;
    u16 planes;
    u16 bits_per_pixel;
    u32 compression;
    u32 size_of_bitmap;
    s32 horz_resolution;
    s32 vert_resolution;
    u32 colors_used;
    u32 colors_important;
    /*u32 red_mask;*/
    /*u32 green_mask;*/
    /*u32 blue_mask;*/
};
#pragma pop

static u32 get_image_pixel_size(Image image)
{
    return image.width * image.height * sizeof(u32);
}

static void create_image_bitmap_file(Image image, const char *file_path)
{
    u32 image_pixel_size = get_image_pixel_size(image);

    BitmapHeader header{};
    header.file_type = 0x4D42;
    header.file_size = sizeof(BitmapHeader) + image_pixel_size;
    header.bitmap_offset = sizeof(BitmapHeader);
    header.size = sizeof(BitmapHeader) - 14;
    header.width = image.width;
    header.height = -image.height; // Inverting so that the top left corner is (0, 0)
    header.planes = 1;
    header.bits_per_pixel = 32;
    header.compression = 0;
    header.size_of_bitmap = image_pixel_size;

    // Optional
    header.horz_resolution = 0;
    header.vert_resolution = 0;
    header.colors_used = 0;
    header.colors_important = 0;

    FILE *output_file;
    if (!fopen_s(&output_file, file_path, "wb"))
    {
        fwrite(&header, sizeof(BitmapHeader), 1, output_file);
        fwrite(image.pixels, image_pixel_size, 1, output_file);
        fclose(output_file);
    }
    else
    {
        printf("%s", "Failed to write file");
    }
}

static Image allocate_image(u32 width, u32 height)
{
    Image image{};
    image.width = width;
    image.height = height;
    image.pixels = (u32 *)malloc(get_image_pixel_size(image));
    return image;
}

static void deallocate_image(Image image)
{
    free(image.pixels);
}
