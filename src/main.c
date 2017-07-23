// SimpleWatchface by siliconninja
// License: MIT

// HEADERS
#include <pebble.h>
#include "main.h"

//#define GColorRed (GColor8){ .argb = 0b11110000 }
  
// STRUCTURE
  
  // HEADERS
  // DECLARATIONS
  // WINDOW
  // MAIN METHODS (init, deinit, main)

// WINDOW
static Window *s_main_window;

// LAYERS
static TextLayer *s_time_layer;

static TextLayer *s_text_layer;

// FONTS
static GFont s_time_font;

// WINDOW METHODS

// Now for the fun part... the time!
static void main_window_load(Window *window) {
  // TIME LAYER
  s_time_font = fonts_get_system_font(FONT_KEY_LECO_32_BOLD_NUMBERS);
  
  //window_create();
  //window_set_background_color(window, (GColor8){ .argb = 0b11010101 });
  //window_stack_push(window, true);
  
  // Create time TextLayer
  // ORIG POSITION 2 55
  s_time_layer = text_layer_create(GRect(0, 35, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorRed);
  text_layer_set_text(s_time_layer, "00:00:00");

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  
  // TEXT LAYER
  
  // Create time TextLayer
  // original parameter 2 is 55 (..., 55, ..., ...)
  s_text_layer = text_layer_create(GRect(0, 105, 144, 50));
  text_layer_set_background_color(s_text_layer, GColorBrightGreen);
  text_layer_set_text_color(s_text_layer, GColorBlack);
  text_layer_set_text(s_text_layer, "Pebble");

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_text_layer));
}

static void main_window_unload(Window *window) {
    // Destroy TextLayers
    text_layer_destroy(s_time_layer);
  
    text_layer_destroy(s_text_layer);
  
    // Unload GFont
    fonts_unload_custom_font(s_time_font);
}

// TIME HANDLERS


// THIS NEEDS TO GO BEFORE tick_handler()
static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00:00"), "%H:%M:%S", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00:00"), "%I:%M:%S", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}




// MAIN METHODS
static void init() {
  // WINDOW EVENTS
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);// Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // ENABLE TIME TICKS
  // REGISTER WITH TIME SERVICE
  // Register with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  
  // Make sure the time is displayed from the start
  update_time();
}

static void deinit(){
  // Destroy Window
    window_destroy(s_main_window);
}

int main(void){
  // INIT
  init();
  
  // RUN APP EVENTS
  app_event_loop();
  
  
  
  // DEINIT
  deinit();
}



