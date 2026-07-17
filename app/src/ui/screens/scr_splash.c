#include <lvgl.h>
#include "display_module.h"

extern const lv_img_dsc_t sami_walk_logo;

static lv_obj_t *scr_splash;

void draw_scr_splash(enum scroll_dir m_scroll_dir)
{
    scr_splash = lv_obj_create(NULL);

    lv_obj_set_style_bg_color(scr_splash, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_border_width(scr_splash, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr_splash, 0, LV_PART_MAIN);

    lv_obj_t *img = lv_img_create(scr_splash);
    lv_img_set_src(img, &sami_walk_logo);
    lv_obj_center(img);

    hpi_disp_set_curr_screen(SCR_SPLASH);
    hpi_show_screen(scr_splash, m_scroll_dir);
}
