#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"
#include "drawing_funcs.h"
#include "tctest.h"

#include <inttypes.h>

// an expected color identified by a (non-zero) character code
typedef struct {
  char c;
  uint32_t color;
} ExpectedColor;

// struct representing a "picture" of an expected image
typedef struct {
  ExpectedColor colors[20];
  const char *pic;
} Picture;

typedef struct {
  struct Image small;
  struct Image large;
  struct Image tilemap;
  struct Image spritemap;
} TestObjs;

// dimensions and pixel index computation for "small" test image (objs->small)
#define SMALL_W        8
#define SMALL_H        6
#define SMALL_IDX(x,y) ((y)*SMALL_W + (x))

// dimensions of the "large" test image
#define LARGE_W        24
#define LARGE_H        20

// create test fixture data
TestObjs *setup(void) {
  TestObjs *objs = (TestObjs *) malloc(sizeof(TestObjs));
  init_image(&objs->small, SMALL_W, SMALL_H);
  init_image(&objs->large, LARGE_W, LARGE_H);
  objs->tilemap.data = NULL;
  objs->spritemap.data = NULL;
  return objs;
}

// clean up test fixture data
void cleanup(TestObjs *objs) {
  free(objs->small.data);
  free(objs->large.data);
  free(objs->tilemap.data);
  free(objs->spritemap.data);
  free(objs);
}

uint32_t lookup_color(char c, const ExpectedColor *colors) {
  for (unsigned i = 0; ; i++) {
    assert(colors[i].c != 0);
    if (colors[i].c == c) {
      return colors[i].color;
    }
  }
}
void check_picture2(struct Image *img, Picture *p) {
  unsigned num_pixels = img->width * img->height;
  assert(strlen(p->pic) == num_pixels);

  for (unsigned i = 0; i < num_pixels; i++) {
    char c = p->pic[i];
    uint32_t expected_color = lookup_color(c, p->colors);
    uint32_t actual_color = img->data[i];
    printf("%u", actual_color);
    printf("%u\n", expected_color);
  }
}

void check_picture(struct Image *img, Picture *p) {
  unsigned num_pixels = img->width * img->height;
  assert(strlen(p->pic) == num_pixels);

  for (unsigned i = 0; i < num_pixels; i++) {
    char c = p->pic[i];
    uint32_t expected_color = lookup_color(c, p->colors);
    uint32_t actual_color = img->data[i];
    ASSERT(actual_color == expected_color);
  }
}

// prototypes of test functions
void test_in_bounds(TestObjs *objs);
void test_compute_index(TestObjs *objs);
void test_clamp(TestObjs *objs);
void test_get_r_g_b_a(TestObjs *objs);
void test_blend_components(TestObjs *objs);
void test_blend_colors(TestObjs *objs);
void test_set_pixel(TestObjs *objs);
void test_square(TestObjs *objs);
void test_square_dist(TestObjs *objs);
void test_draw_pixel(TestObjs *objs);
void test_draw_rect(TestObjs *objs);
void test_draw_circle(TestObjs *objs);
void test_draw_circle_clip(TestObjs *objs);
void test_draw_tile(TestObjs *objs);
void test_draw_sprite(TestObjs *objs);

int main(int argc, char **argv) {
  if (argc > 1) {
    // user specified a specific test function to run
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  // test helper functions
  TEST(test_in_bounds);
  TEST(test_compute_index);
  TEST(test_clamp);
  TEST(test_get_r_g_b_a);
  TEST(test_blend_components);
  TEST(test_blend_colors);
  TEST(test_set_pixel);
  TEST(test_square);
  TEST(test_square_dist);

  //test draw
  TEST(test_draw_pixel);
  TEST(test_draw_rect);
  TEST(test_draw_circle);
  TEST(test_draw_circle_clip);
  //TEST(test_draw_tile);
  //TEST(test_draw_sprite);

  TEST_FINI();
}

//test helper functions
void test_in_bounds(TestObjs *objs){
  ASSERT(in_bounds(&objs->small, 0, 0));
  ASSERT(in_bounds(&objs->small, 4, 3));
  ASSERT(in_bounds(&objs->small, SMALL_W-1, SMALL_H-1));
  ASSERT(in_bounds(&objs->large, 0, 0));
  ASSERT(in_bounds(&objs->large, 4, 3));
  ASSERT(in_bounds(&objs->large, LARGE_W-1, LARGE_H-1));

  ASSERT(!in_bounds(&objs->small, -1, -1));
  ASSERT(!in_bounds(&objs->small, SMALL_W, SMALL_H));
  ASSERT(!in_bounds(&objs->large, -1, -1));
  ASSERT(!in_bounds(&objs->large, LARGE_W, LARGE_H));
}
void test_compute_index(TestObjs *objs){
  ASSERT(SMALL_IDX(0,0) == compute_index(&objs->small, 0, 0));
  ASSERT(SMALL_IDX(4,3) == compute_index(&objs->small, 4, 3));
  ASSERT(SMALL_IDX(SMALL_W-1,SMALL_H-1) == compute_index(&objs->small, SMALL_W-1, SMALL_H-1));
}
void test_clamp(TestObjs *obs){
  ASSERT(clamp(-5,0,SMALL_W) == 0);
  ASSERT(clamp(0,0,SMALL_W) == 0);
  ASSERT(clamp(SMALL_W+1,0,SMALL_W) == SMALL_W);
}
void test_get_r_g_b_a(TestObjs *objs){
  uint32_t no_color = 0;
  ASSERT(get_r(no_color) == 0);
  ASSERT(get_g(no_color) == 0);
  ASSERT(get_b(no_color) == 0);
  ASSERT(get_a(no_color) == 0);
  uint32_t opaque_red = 0xFF0000FFU;
  ASSERT(get_r(opaque_red) == 0xFFU);
  ASSERT(get_g(opaque_red) == 0);
  ASSERT(get_b(opaque_red) == 0);
  ASSERT(get_a(opaque_red) == 0xFFU);
  uint32_t half_opaque_cyan = 0x00FFFF80U;
  ASSERT(get_r(half_opaque_cyan) == 0);
  ASSERT(get_g(half_opaque_cyan) == 0xFFU);
  ASSERT(get_b(half_opaque_cyan) == 0xFFU);
  ASSERT(get_a(half_opaque_cyan) == 0x80U);
  uint32_t opaque_white = 0xFFFFFFFFU;
  ASSERT(get_r(opaque_white) == 0xFFU);
  ASSERT(get_g(opaque_white) == 0xFFU);
  ASSERT(get_b(opaque_white) == 0xFFU);
  ASSERT(get_a(opaque_white) == 0xFFU);
}
void test_blend_components(TestObjs *objs){
  ASSERT(blend_components(0,0,0) == 0);
  ASSERT(blend_components(0xFFU,0x8AU,0) == 0x8AU);
  ASSERT(blend_components(0,0,0xFFU) == 0);
  ASSERT(blend_components(0x8AU,0xFFU,0xFFU) == 0x8AU);
  ASSERT(blend_components(0xFFU,0xFFU,0x80U) == 0xFFU);
  ASSERT(blend_components(0,0xFFU,0x80U) == 0x7FU);
  ASSERT(blend_components(0x3AU,0xF3U,0xB7U) == 0x6EU);
}
void test_blend_colors(TestObjs *objs){
  ASSERT(blend_colors(0,0) == 0xFFU);
  ASSERT(blend_colors(0,0xFFFFFFFFU) == 0xFFFFFFFFU);
  ASSERT(blend_colors(0xFFFFFFFFU,0) == 0xFFFFFFFFU);
  ASSERT(blend_colors(0x00CB00D3U,0x715300FFU) == 0x13B600FFU);
  ASSERT(blend_colors(0xFFFFFF00U,0x873B68ABU) == 0x873B68FFU);
}
void test_set_pixel(TestObjs *objs){
  set_pixel(&objs->small, 0, 0);
  ASSERT(objs->small.data[0] == 0xFFU);
  set_pixel(&objs->small, SMALL_IDX(3, 2), 0x13008AFFU);
  ASSERT(objs->small.data[SMALL_IDX(3, 2)] == 0x13008AFFU);
  set_pixel(&objs->small, SMALL_IDX(3, 2), 0xFFB7FFFFU);
  ASSERT(objs->small.data[SMALL_IDX(3, 2)] == 0xFFB7FFFFU);
  set_pixel(&objs->small, SMALL_IDX(3, 2), 0xFFB76300U);
  ASSERT(objs->small.data[SMALL_IDX(3, 2)] == 0xFFB7FFFFU);
  set_pixel(&objs->small, SMALL_IDX(3, 2), 0xFFA96380U);
  ASSERT(objs->small.data[SMALL_IDX(3, 2)] == 0xFFAFB0FFU);
}
void test_square(TestObjs *objs){
  ASSERT(square(0) == 0);
  ASSERT(square(1) == 1);
  ASSERT(square(-1) == 1);
  ASSERT(square(16) == 256);
}
void test_square_dist(TestObjs *objs){
  ASSERT(square_dist(0,0,0,0) == 0);
  ASSERT(square_dist(0,0,1,1) == 2);
  ASSERT(square_dist(-1,-1,1,1) == 8);
  ASSERT(square_dist(0,0,3,4) == 25);
}

//default tests
void test_draw_pixel(TestObjs *objs) {
  // initially objs->small pixels are opaque black
  ASSERT(objs->small.data[SMALL_IDX(3, 2)] == 0x000000FFU);
  ASSERT(objs->small.data[SMALL_IDX(5, 4)] == 0x000000FFU);

  // test drawing completely opaque pixels
  draw_pixel(&objs->small, 3, 2, 0xFF0000FF); // opaque red
  ASSERT(objs->small.data[SMALL_IDX(3, 2)] == 0xFF0000FF);
  draw_pixel(&objs->small, 5, 4, 0x800080FF); // opaque magenta (half-intensity)
  ASSERT(objs->small.data[SMALL_IDX(5, 4)] == 0x800080FF);

  // test color blending
  draw_pixel(&objs->small, 3, 2, 0x00FF0080); // half-opaque full-intensity green
  ASSERT(objs->small.data[SMALL_IDX(3, 2)] == 0x7F8000FF);
  draw_pixel(&objs->small, 4, 2, 0x0000FF40); // 1/4-opaque full-intensity blue
  ASSERT(objs->small.data[SMALL_IDX(4, 2)] == 0x000040FF);
}

void test_draw_rect(TestObjs *objs) {
  struct Rect red_rect = { .x = 2, .y = 2, .width=3, .height=3 };
  struct Rect blue_rect = { .x = 3, .y = 3, .width=3, .height=3 };
  draw_rect(&objs->small, &red_rect, 0xFF0000FF); // red is full-intensity, full opacity
  draw_rect(&objs->small, &blue_rect, 0x0000FF80); // blue is full-intensity, half opacity

  const uint32_t red   = 0xFF0000FF; // expected full red color
  const uint32_t blue  = 0x000080FF; // expected full blue color
  const uint32_t blend = 0x7F0080FF; // expected red/blue blend color
  const uint32_t black = 0x000000FF; // expected black (background) color

  Picture expected = {
    { {'r', red}, {'b', blue}, {'n', blend}, {' ', black} },
    "        "
    "        "
    "  rrr   "
    "  rnnb  "
    "  rnnb  "
    "   bbb  "
  };

  //check_picture2(&objs->small, &expected);
  check_picture(&objs->small, &expected);
}

void test_draw_circle(TestObjs *objs) {
  Picture expected = {
    { {' ', 0x000000FF}, {'x', 0x00FF00FF} },
    "   x    "
    "  xxx   "
    " xxxxx  "
    "  xxx   "
    "   x    "
    "        "
  };

  draw_circle(&objs->small, 3, 2, 2, 0x00FF00FF);

  check_picture(&objs->small, &expected);
}

void test_draw_circle_clip(TestObjs *objs) {
  Picture expected = {
    { {' ', 0x000000FF}, {'x', 0x00FF00FF} },
    " xxxxxxx"
    " xxxxxxx"
    "xxxxxxxx"
    " xxxxxxx"
    " xxxxxxx"
    "  xxxxx "
  };

  draw_circle(&objs->small, 4, 2, 4, 0x00FF00FF);

  check_picture(&objs->small, &expected);
}

void test_draw_tile(TestObjs *objs) {
  ASSERT(read_image("img/PrtMimi.png", &objs->tilemap) == IMG_SUCCESS);

  struct Rect r = { .x = 4, .y = 2, .width = 16, .height = 18 };
  draw_rect(&objs->large, &r, 0x1020D0FF);

  struct Rect grass = { .x = 0, .y = 16, .width = 16, .height = 16 };
  draw_tile(&objs->large, 3, 2, &objs->tilemap, &grass);

  Picture pic = {
    {
      { ' ', 0x000000ff },
      { 'a', 0x52ad52ff },
      { 'b', 0x1020d0ff },
      { 'c', 0x257b4aff },
      { 'd', 0x0c523aff },
    },
    "                        "
    "                        "
    "             a     b    "
    "            a      b    "
    "            a     ab    "
    "           ac      b    "
    "           ac a    b    "
    "      a a  ad  a   b    "
    "     a  a aad  aa ab    "
    "     a  a acd aaacab    "
    "    aa  cdacdaddaadb    "
    "     aa cdaddaaddadb    "
    "     da ccaddcaddadb    "
    "    adcaacdaddddcadb    "
    "   aaccacadcaddccaab    "
    "   aacdacddcaadcaaab    "
    "   aaaddddaddaccaacb    "
    "   aaacddcaadacaaadb    "
    "    bbbbbbbbbbbbbbbb    "
    "    bbbbbbbbbbbbbbbb    "
  };

  check_picture(&objs->large, &pic);
}

void test_draw_sprite(TestObjs *objs) {
  ASSERT(read_image("img/NpcGuest.png", &objs->spritemap) == IMG_SUCCESS);

  struct Rect r = { .x = 1, .y = 1, .width = 14, .height = 14 };
  draw_rect(&objs->large, &r, 0x800080FF);

  struct Rect sue = { .x = 128, .y = 136, .width = 16, .height = 15 };
  draw_sprite(&objs->large, 4, 2, &objs->spritemap, &sue);

  Picture pic = {
    {
      { ' ', 0x000000ff },
      { 'a', 0x800080ff },
      { 'b', 0x9cadc1ff },
      { 'c', 0xefeae2ff },
      { 'd', 0x100000ff },
      { 'e', 0x264c80ff },
      { 'f', 0x314e90ff },
    },
    "                        "
    " aaaaaaaaaaaaaa         "
    " aaaaaaaaaaaaaa         "
    " aaaaaaaaaaaaaa         "
    " aaaaaaabccccccbc       "
    " aaaaacccccccccccc      "
    " aaaacbddcccddcbccc     "
    " aaacbbbeccccedbccc     "
    " aaacbbceccccebbbccc    "
    " aaabbbccccccccbbccc    "
    " aaaabbbcccccccb ccb    "
    " aaaabaaaaabbaa  cb     "
    " aaaaaaaaafffea         "
    " aaaaaaaaaffeea         "
    " aaaaaaacffffcc         "
    "        cffffccb        "
    "         bbbbbbb        "
    "                        "
    "                        "
    "                        "
  };

  check_picture(&objs->large, &pic);
}