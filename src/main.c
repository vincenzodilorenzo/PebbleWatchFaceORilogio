#include <pebble.h>
#include "num2words.h"



/* static pointer to the variabile Window to handle the main window */
static Window      *s_main_window;
/* static pointers to the variabile TextLayer to display hours and shaded hours */
static TextLayer   *s_hour_layer;
static TextLayer   *s_hour_layer_shadow;
/* static pointer to the variabile TextLayer to display minutes and shaded minutes */
static TextLayer   *s_minute_layer;
static TextLayer   *s_minute_layer_shadow;
/* static pointer to the variabile TextLayer to display days and shaded days */
static TextLayer   *s_day_layer;
static TextLayer   *s_day_layer_shadow;
/* static pointer to the variabile TextLayer to display the date and the shaded date */
static TextLayer   *s_date_layer;
static TextLayer   *s_date_layer_shadow;
/* static pointer to the variabiles GBitmap and BitmapLayer to manage the background image */
static GBitmap     *s_bitmap;
static BitmapLayer *s_bitmap_layer;

/* battery level and static pointer to the variables GBitmap(s) and BitmapLayer to display the battery level */
static int         s_battery_level;
static GBitmap     *s_battery_bitmap_100;
static GBitmap     *s_battery_bitmap_90;
static GBitmap     *s_battery_bitmap_80;
static GBitmap     *s_battery_bitmap_70;
static GBitmap     *s_battery_bitmap_60;
static GBitmap     *s_battery_bitmap_50;
static GBitmap     *s_battery_bitmap_40;
static GBitmap     *s_battery_bitmap_30;
static GBitmap     *s_battery_bitmap_20;
static GBitmap     *s_battery_bitmap_10;
static BitmapLayer *s_battery_bitmap_layer;

/* static pointer to the variables GBitmap(s) and BitmapLayer to display the bluetooth status */
static BitmapLayer *s_bt_bitmap_layer;
static GBitmap     *s_bt_disconnected_bitmap; 
static GBitmap     *s_bt_connected_bitmap;


/* 
   This static function extracts hours, minutes, day of the week and date, 
   converts them in Napolitan Language and puts them in dedicated TextLayers. 
   It's invoked in init() and in tick_handler().
*/
static void update_time() {
  
  /* Get the struct tick_time e put in it the current localtime */
  time_t temp = time(NULL);
  struct tm tick_time = *localtime(&temp);

  /* Handle hours */
  /* Extract hours, convert them in Napolitan Language and fill a char buffer */
  static char s_hour_buffer[30];
  hours_to_common_words(tick_time.tm_hour, s_hour_buffer);
  /* Insert the char bufer into a couple of dedicated TextLayers */
  text_layer_set_text(s_hour_layer, s_hour_buffer);
  text_layer_set_text(s_hour_layer_shadow, s_hour_buffer);

  /* Handle minutes */
  /* Extract minutes, convert them in Napolitan Language and fill a char buffer */
  static char s_minute_buffer[30];
  minutes_to_common_words(tick_time.tm_min, s_minute_buffer);  
  /* Insert the char bufer into a couple of dedicated TextLayers */
  text_layer_set_text(s_minute_layer, s_minute_buffer);
  text_layer_set_text(s_minute_layer_shadow, s_minute_buffer);
  
  /* Handle days of week */
  /* Extract days, convert them in Napolitan Language and fill a char buffer */
  static char s_day_buffer[30];
  days_to_common_words(tick_time.tm_wday, s_day_buffer);
  /* Insert the char bufer into a dedicated couple of TextLayers */
  text_layer_set_text(s_day_layer, s_day_buffer);
  text_layer_set_text(s_day_layer_shadow, s_day_buffer);
  
  /* Handle current date */  
  /* Extract current date, comvert in format DD/MM/YYYY and fill a char buffer */
  static char s_date_buffer[15];
  date_to_string(tick_time.tm_year, tick_time.tm_mon, tick_time.tm_mday, s_date_buffer);
  /* Insert the char bufer into a dedicated couple od TextLayers */
  text_layer_set_text(s_date_layer, s_date_buffer);
  text_layer_set_text(s_date_layer_shadow, s_date_buffer);
}



/* 
   This static function initializes the main window. 
   It's linked in init() to the "load" event of the main window.
*/
static void main_window_load(Window *window) {
  
  /* Prepare the background image according to the shape of peble watch */
  #if defined(PBL_RECT)
  s_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TOP_BACKGROUND_IMAGE_SHADOW);
  s_bitmap_layer = bitmap_layer_create(GRect(0, 0, 144, 71));
  bitmap_layer_set_compositing_mode(s_bitmap_layer, GCompOpSet);
  bitmap_layer_set_bitmap(s_bitmap_layer, s_bitmap);
  #elif defined(PBL_ROUND)
  s_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TOP_BACKGROUND_IMAGE_SHADOW_ROUND);
  s_bitmap_layer = bitmap_layer_create(GRect(0, 7, 172, 49));
  bitmap_layer_set_compositing_mode(s_bitmap_layer, GCompOpSet);
  bitmap_layer_set_bitmap(s_bitmap_layer, s_bitmap);   
  #endif
  
  /* Get the root layer of the main window, get the main window limits */
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  /* Create two TextLayers to display hours, one of them is needed to implement a shadow effct */
  /* Base TextLayer for hours */
  s_hour_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(52,72), bounds.size.w, 50));
  /* Define some more additional properties */
  text_layer_set_background_color(s_hour_layer, GColorClear);
  text_layer_set_text_color(s_hour_layer, GColorVividCerulean);
  text_layer_set_font(s_hour_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_hour_layer, GTextAlignmentCenter);
  /* Additional TextLayer for hours to implement a shadow effct */
  s_hour_layer_shadow = text_layer_create(
      GRect(1, PBL_IF_ROUND_ELSE(53,73), bounds.size.w, 50));
  /* Define some more properties */
  text_layer_set_background_color(s_hour_layer_shadow, GColorClear);
  text_layer_set_text_color(s_hour_layer_shadow, GColorBlack);
  text_layer_set_font(s_hour_layer_shadow, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_hour_layer_shadow, GTextAlignmentCenter);

  /* Create two TextLayers to display minutes, one of them is needed to implement a shadow effct */
  /* Base TextLayer for minutes */
  s_minute_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(82,102), bounds.size.w, 50));
  /* Define some more properties */
  text_layer_set_background_color(s_minute_layer, GColorClear);
  text_layer_set_text_color(s_minute_layer, GColorBlack);
  text_layer_set_font(s_minute_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(s_minute_layer, GTextAlignmentCenter);
  /* Additional TextLayer for minutes to implement a shadow effct */
  s_minute_layer_shadow = text_layer_create(
      GRect(1, PBL_IF_ROUND_ELSE(83,103), bounds.size.w, 50));
  /* Define some more properties */
  text_layer_set_background_color(s_minute_layer_shadow, GColorClear);
  text_layer_set_text_color(s_minute_layer_shadow, GColorVividCerulean);
  text_layer_set_font(s_minute_layer_shadow, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(s_minute_layer_shadow, GTextAlignmentCenter);
  
  /* Create two TextLayers to display the day of the week, one of them is needed to implement a shadow effct */
  s_day_layer = text_layer_create(
      GRect(PBL_IF_ROUND_ELSE(0, 3), PBL_IF_ROUND_ELSE(128, 142), bounds.size.w, 50));
  /* Define some more properties */
  text_layer_set_background_color(s_day_layer, GColorClear);
  text_layer_set_text_color(s_day_layer, GColorBlack);
  text_layer_set_font(s_day_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(s_day_layer, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentLeft));
  /* Additional TextLayer for the day of the week to implement a shadow effct */
  s_day_layer_shadow = text_layer_create(
      GRect(PBL_IF_ROUND_ELSE(1, 4), PBL_IF_ROUND_ELSE(129, 143), bounds.size.w, 50));
  /* Define some more properties */
  text_layer_set_background_color(s_day_layer_shadow, GColorClear);
  text_layer_set_text_color(s_day_layer_shadow, GColorVividCerulean);
  text_layer_set_font(s_day_layer_shadow, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(s_day_layer_shadow, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentLeft));

  /* Create two TextLayers to display the date of the year, one of them is needed to implement a shadow effct */
  s_date_layer = text_layer_create(
      GRect(PBL_IF_ROUND_ELSE(0, 0), PBL_IF_ROUND_ELSE(149, 145), PBL_IF_ROUND_ELSE(bounds.size.w,bounds.size.w-2), 50));  
  /* Define some more properties */
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorBlack);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_date_layer, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentRight));
  /* Additional TextLayer for the date of the year to implement a shadow effct */
  s_date_layer_shadow = text_layer_create(
      GRect(PBL_IF_ROUND_ELSE(1, 1), PBL_IF_ROUND_ELSE(150, 146), PBL_IF_ROUND_ELSE(bounds.size.w,bounds.size.w-2), 50));  
  /* Define some more properties */
  text_layer_set_background_color(s_date_layer_shadow, GColorClear);
  text_layer_set_text_color(s_date_layer_shadow, GColorVividCerulean);
  text_layer_set_font(s_date_layer_shadow, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_date_layer_shadow, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentRight));

  /* Create 10 bitmaps a bitmap layer and to display battery level */
  s_battery_bitmap_100 = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_100);
  s_battery_bitmap_90  = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_90);  
  s_battery_bitmap_80  = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_80);
  s_battery_bitmap_70  = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_70);
  s_battery_bitmap_60  = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_60);
  s_battery_bitmap_50  = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_50);
  s_battery_bitmap_40  = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_40);
  s_battery_bitmap_30  = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_30);
  s_battery_bitmap_20  = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_20);
  s_battery_bitmap_10  = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_10);
  s_battery_bitmap_layer = bitmap_layer_create(GRect(PBL_IF_ROUND_ELSE(62, 1), PBL_IF_ROUND_ELSE(44, 1), 25, 10));
  bitmap_layer_set_compositing_mode(s_battery_bitmap_layer, GCompOpSet);
  
  
  /* Create 2 bitmaps a bitmap layer and to display BT connection */
  /* Check position for pebble round */
  s_bt_disconnected_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BT_DISCONNECTED);
  s_bt_connected_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BT_CONNECTED);  
  s_bt_bitmap_layer = bitmap_layer_create(GRect(PBL_IF_ROUND_ELSE(96, 118), PBL_IF_ROUND_ELSE(44, 1), 25, 10));
  bitmap_layer_set_compositing_mode(s_bt_bitmap_layer, GCompOpSet);
  
  /* Add the top BitmapLayer (background image) to the root Layer of the main window */
  layer_add_child(window_layer,bitmap_layer_get_layer(s_bitmap_layer));

  /* Add all TextLayers to the root Layer of the main window */
  /* To get a correct shadow effect the order is important */
  layer_add_child(window_layer, text_layer_get_layer(s_hour_layer_shadow));  
  layer_add_child(window_layer, text_layer_get_layer(s_hour_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_minute_layer_shadow));
  layer_add_child(window_layer, text_layer_get_layer(s_minute_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_day_layer_shadow));  
  layer_add_child(window_layer, text_layer_get_layer(s_day_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer_shadow));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
  
  /* add the battery and BT bitmap layers */
  layer_add_child(window_layer,bitmap_layer_get_layer(s_battery_bitmap_layer));
  layer_add_child(window_layer,bitmap_layer_get_layer(s_bt_bitmap_layer));
}



/* This static function destroys the main window and releases all resources. 
   It is linked in init() to the event "unload" of the main window.
*/
static void main_window_unload(Window *window) {
  
  /* Free all resources dynamically allocated */
  text_layer_destroy(s_hour_layer);
  text_layer_destroy(s_hour_layer_shadow);
  text_layer_destroy(s_minute_layer);
  text_layer_destroy(s_minute_layer_shadow);
  text_layer_destroy(s_day_layer);
  text_layer_destroy(s_day_layer_shadow);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_date_layer_shadow);  
  
  bitmap_layer_destroy(s_bitmap_layer);
  gbitmap_destroy(s_bitmap);
  
  bitmap_layer_destroy(s_battery_bitmap_layer);
  gbitmap_destroy(s_battery_bitmap_100);
  gbitmap_destroy(s_battery_bitmap_90);
  gbitmap_destroy(s_battery_bitmap_80);
  gbitmap_destroy(s_battery_bitmap_70);
  gbitmap_destroy(s_battery_bitmap_60);
  gbitmap_destroy(s_battery_bitmap_50);
  gbitmap_destroy(s_battery_bitmap_40);
  gbitmap_destroy(s_battery_bitmap_30);
  gbitmap_destroy(s_battery_bitmap_20);
  gbitmap_destroy(s_battery_bitmap_10);
  
  bitmap_layer_destroy(s_bt_bitmap_layer);
  gbitmap_destroy(s_bt_connected_bitmap);
  gbitmap_destroy(s_bt_disconnected_bitmap);
}



/* 
   This static function is the handler to manage change of time events. 
   It is registered in init().
*/
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  /* call the final business funcion to handle times and dates */
  update_time();
}



/*
   This static function is the handler to manage changes of battery level.
   It is registered and immediately called in init().
*/
static void battery_callback(BatteryChargeState state) {
  /* Get a new battery level */
  s_battery_level = state.charge_percent;
  
  /* Select the right icon according to the battery level */
  switch(s_battery_level) {
  case 100:
    bitmap_layer_set_bitmap(s_battery_bitmap_layer, s_battery_bitmap_100);
    break;
  case 90:
    bitmap_layer_set_bitmap(s_battery_bitmap_layer, s_battery_bitmap_90);
    break;    
  case 80:
    bitmap_layer_set_bitmap(s_battery_bitmap_layer, s_battery_bitmap_80);
    break;  
  case 70:
    bitmap_layer_set_bitmap(s_battery_bitmap_layer, s_battery_bitmap_70);
    break; 
  case 60:
    bitmap_layer_set_bitmap(s_battery_bitmap_layer, s_battery_bitmap_60);
    break;
  case 50:
    bitmap_layer_set_bitmap(s_battery_bitmap_layer, s_battery_bitmap_50);
    break;   
  case 40:
    bitmap_layer_set_bitmap(s_battery_bitmap_layer, s_battery_bitmap_40);
    break;  
  case 30:
    bitmap_layer_set_bitmap(s_battery_bitmap_layer, s_battery_bitmap_30);
    break; 
  case 20:
    bitmap_layer_set_bitmap(s_battery_bitmap_layer, s_battery_bitmap_20);
    break;
  case 10:
    bitmap_layer_set_bitmap(s_battery_bitmap_layer, s_battery_bitmap_10);
    break; 
  case 0: 
    bitmap_layer_set_bitmap(s_battery_bitmap_layer, s_battery_bitmap_10);  
  }

}



/*
   This static function is the handler to manage changes of bluetooth status.
   It is registered  and immediately called in init().
*/
static void bluetooth_callback(bool connected) {
  
  if(!connected) {
    /* Not conected here */
    vibes_short_pulse();
    bitmap_layer_set_bitmap(s_bt_bitmap_layer, s_bt_disconnected_bitmap);
  } else {
    /* Conected here */
    bitmap_layer_set_bitmap(s_bt_bitmap_layer, s_bt_connected_bitmap);
  }
}



/* 
   This static function initalizes the watchface.
*/
static void init() {
  /* Create a main window */
  s_main_window = window_create();

  /* Set the main window handlers to manage load and unload */
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  /* Register the service TickTimerService to invoke the tick_handler function every minute */
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  /* Register the service BatteryStateService to invoke the battery_callback function for battery level updates */
  battery_state_service_subscribe(battery_callback);
  
  /* Register the service ConnectionService to invoke the bluetooth_callback function for Bluetooth connection updates */
  connection_service_subscribe((ConnectionHandlers) {
  .pebble_app_connection_handler = bluetooth_callback
  });

  /* Add the main window to the stack so that it can be displayed, animated=true */
  window_stack_push(s_main_window, true);
  
  /* Display time, day of the week and current date immediately */
  update_time();
  
  /* Display the battery level immediately */
  battery_callback(battery_state_service_peek());
  
  /* Display the current state of the BT connection immediately */
  bluetooth_callback(connection_service_peek_pebble_app_connection());
}



/*
   This static function destroys the watchface.
*/
static void deinit() {
  window_destroy(s_main_window);
}



/*
   Finally here is the main() function.
*/
int main(void) {
  
  /* initalize the watchface */
  init();
  
  /* wait for inputs and events */
  app_event_loop();
  
  /* close everything and release all resources */
  deinit();
}

