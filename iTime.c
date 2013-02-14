/**
 * Copyright (C) 2011, Allerta Inc.
 * Author: Jon Bennett (jon@allerta.ca)
 *
 * Permission to use, copy, modify, and/or distribute this software for  
 * any purpose with or without fee is hereby granted, provided that the  
 * above copyright notice and this permission notice appear in all copies.  
 *  
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL  
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED  
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR  
 * BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES  
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,  
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,  
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS  
 * SOFTWARE.
 * 
 * Modifed by David Rosenfeld June 2012
 **/

#include <pulse_os.h>
#include <pulse_types.h>
#include <stdint.h>
#include <stddef.h>

// It is daylight savings time so do this
#define TIMEZONE_OFFSET -1

// Forward Declaration
void handle_button_causing_wakeup();

// Ask the clock for the internet time
int get_internet_time()
{
  int hour;
  int min;
  int sec;
  int beat;

  struct pulse_time_tm now;
  pulse_get_time_date(&now);
  hour = now.tm_hour;
  min = now.tm_min;
  sec = now.tm_sec;

  beat = ((sec + (60 * min) + ((hour + 1 + TIMEZONE_OFFSET) * 3600)) / 86.4);
  if(beat > 1000){
    beat = beat - 1000;
  }
  return beat;
}

// This function is called once after the watch has booted up
// and the OS has loaded
void main_app_init()
{
  draw();
  // Schedule the processor to go to sleep in 5 seconds
  pulse_update_power_down_timer(10000);

  // Register a callback for the woke_from_button event
  pulse_register_callback(ACTION_WOKE_FROM_BUTTON, &handle_button_causing_wakeup);
}

void handle_draw_event(int timeout)
{
  draw();
  pulse_register_timer(timeout, &handle_draw_event, timeout);
}

// Now Draw!
void draw()
{
  int hour;
  int min;
  int sec;

  struct pulse_time_tm now;
  pulse_get_time_date(&now);
  hour = now.tm_hour;
  min = now.tm_min;
  sec = now.tm_sec;

  pulse_oled_set_brightness(100);
  pulse_blank_canvas();
  int beat;
  beat = get_internet_time();
  printf("\n\n    @%d\n", beat);
  printf("\n\n    %d:%d", hour, min);

}

// This function is called when the button is used to wake the processor
void handle_button_causing_wakeup()
{
  // Set the screen brightness to full
  pulse_oled_set_brightness(100);

  // Schedule the processor to go to sleep in 9 seconds
  pulse_update_power_down_timer(9000);
  draw();
}

void main_app_handle_button_down()
{

}

void main_app_handle_button_up()
{

}

// Main loop. This function is called frequently.
// No blocking calls are allowed in this function or else the watch will reset.
// The inPulse watchdog timer will kick in after 5 seconds if a blocking
// call is made.
void main_app_loop()
{

}

// This function is called whenever the processor is about to sleep (to conserve power)
// The sleep functionality is scheduled with pulse_update_power_down_timer(uint32_t)
void main_app_handle_doz()
{
  isAlive = false;
  // Gradually turn down the screen brightness, creating a fade effect
  for (int i = 100; i >= 0; i-=6) {
    pulse_oled_set_brightness(i);
    pulse_mdelay(60);
  }
}

// Prints a status message to the screen when bluetooth is connected or disconnected
void main_app_handle_hardware_update(enum PulseHardwareEvent event)
{

}
