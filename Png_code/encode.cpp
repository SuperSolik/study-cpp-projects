#include <iostream>
#include <vector>
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
  if(!png){
    std::cout<<"Error with opening"<<std::endl;
    return;
  }
  png_infop info = png_create_info_struct(png);
  if(!info) {
    std::cout<<"Error with opening"<<std::endl;
    return;
  }

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

void write_png_file(char *filename) {
  int y;

  FILE *fp = fopen(filename, "wb");
  if(!fp){
    std::cout<<"Error with writing"<<std::endl;
    return;
  }

  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png){
    std::cout<<"Error with writing"<<std::endl;
    return;
  }

  png_infop info = png_create_info_struct(png);
  if (!info){
    std::cout<<"Error with writing"<<std::endl;
    return;
  }

  if (setjmp(png_jmpbuf(png))) {
    std::cout<<"Error with writing"<<std::endl;
    return;
  }

  png_init_io(png, fp);

  // Output is 8bit depth, RGBA format.
  png_set_IHDR(
    png,
    info,
    width, height,
    8,
    PNG_COLOR_TYPE_RGBA,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );
  png_write_info(png, info);

  // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
  // Use png_set_filler().
  //png_set_filler(png, 0, PNG_FILLER_AFTER);

  png_write_image(png, row_pointers);
  png_write_end(png, NULL);

  for(int y = 0; y < height; y++) {
    free(row_pointers[y]);
  }
  free(row_pointers);

  fclose(fp);
}

void process_png_file(const std::vector<int>& v) {
  int i = 0;
  for(int y = 0; y < height; y++) {
    png_bytep row = row_pointers[y];
    for(int x = 0; x < width; x++) {
      png_bytep px = &(row[x * 4]);
      if(i < v.size()){
        if(v[i] == 0){
          px[0] = px[0]&254;
          px[1] = px[1]&254;
          px[2] = px[2]&254;
        } else {
          px[0] = px[0]|1;
          px[1] = px[1]|1;
          px[2] = px[2]|1;  
        }
      }
      if(i == v.size()){
        px[0] = ~px[1];  
      }
      i++;
    }
  }
}

std::vector<int> get_bin(std::string& str){ 
  std::vector<int> result; 
  int c = 0; 
  int it = 0; 
  int temp = 0; 
  for (int i = 0; i < str.size(); ++i){ 
    c = static_cast<int>(str[i]); 
    temp = c;
    while(c != 0){ 
      if(c%2 == 0){ 
        result.insert(result.begin()+it, 0); 
      } else { 
        result.insert(result.begin()+it, 1); 
      } 
        c = c/2; 
    } 
    if(result.size() %8 != 0){ 
      result.insert(result.begin()+it, 0);
      if(temp < 64 && temp >= 32) result.insert(result.begin()+it, 0);  
    } 
    it += 8; 
  } 
  /*for (int i = 0; i < result.size(); ++i){
    std::cout<<result[i];
  }*/
  return result; 
}

int main(int argc, char *argv[]) {
  if(argc != 3){
    std::cout<<""<<std::endl;
    return 0;
  }
  std::string text;
  std::cout<<"Enter a text to code"<<std::endl;
  std::getline (std::cin, text);
  read_png_file(argv[1]);
  std::vector<int> v(get_bin(text));
  process_png_file(v);
  write_png_file(argv[2]);
  return 0;
}
