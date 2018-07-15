#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <png.h>

int width, height;
png_byte color_type;
png_byte bit_depth;
png_bytep *row_pointers;

void read_png_file(char *filename) {
  FILE *fp = fopen(filename, "rb");

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png) abort();

  png_infop info = png_create_info_struct(png);
  if(!info) abort();

  if(setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  png_read_info(png, info);

  width      = png_get_image_width(png, info);
  height     = png_get_image_height(png, info);
  color_type = png_get_color_type(png, info);
  bit_depth  = png_get_bit_depth(png, info);

  // Read any color_type into 8bit depth, RGBA format.
  // See http://www.libpng.org/pub/png/libpng-manual.txt

  if(bit_depth == 16)
    png_set_strip_16(png);

  if(color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

  // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
  if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

  if(png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  // These color_type don't have an alpha channel then fill it with 0xff.
  if(color_type == PNG_COLOR_TYPE_RGB ||
     color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

  if(color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

  png_read_update_info(png, info);

  row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  for(int y = 0; y < height; y++) {
    row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
  }
  png_read_image(png, row_pointers);

  fclose(fp);
}

int bin_to_dec(int val, int indx){
    return val*pow(2, indx);
}

std::vector<char> get_result(std::vector<int>& v){
    /*for (int i = 0; i < v.size(); ++i){
        std::cout<<v[i]<<' ';
        if(i == v.size()-1)std::cout<<std::endl;
    }*/
    std::vector<char> result;
    int c = 0;
    for (int i = 0; i < v.size(); ++i){
        c += bin_to_dec(v[i], 7-i%8);
        if((i+1)%8 == 0){
            result.push_back(static_cast<char>(c));
            c = 0;
        }
    }
    return result;    
}

void decode_file(std::vector<int>& v) {
    int x,y;
    int a, b, c;
    int i = 0;
    for (y = 0; y < height; y++) {
        png_byte *row = row_pointers[y];
        for (x = 0; x < width; x++) {
            png_byte *ptr = &(row[x * 4]);
            a = ptr[0]&1;
            b = ptr[1]&1;
            c = ptr[2]&1;
            if((a==b) && (b==c) && (a==c)){
                v.push_back(a);
            } else {
                return;    
            }
        }
    }
}

void write_result(char* file_name, std::vector<char>& text){
    std::ofstream file;
    file.open(std::string(file_name), std::ios::out);
    for (int i = 0; i < text.size(); ++i){
        file<<text[i];
    }
    file.close();
}

int main(int argc, char *argv[]) {
  if (argc != 3){
    fprintf(stderr,"Usage: program_name <file_in> <file_out>\n");
    return 0;
  }
  read_png_file(argv[1]);
  std::vector<int> bin;
  decode_file(bin);
  std::vector<char> str(get_result(bin));
  write_result(argv[2], str);

  return 0;
}