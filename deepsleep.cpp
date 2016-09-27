#include <Arduino.h>
#include "deepsleep.h"

#define RTC_OFFSET_MAGIC 65
#define RTC_OFFSET_STATE 66
#define RTC_OFFSET_DATA 67
#define MAGIC_1 0x55
#define MAGIC_2 0xaa

extern "C" {
  #include "user_interface.h"
  // provides system_rtc_mem_{read,write}
}

byte buf[10] __attribute__((aligned(4)));

uint8_t getState(void *des_addr, uint16 load_size) {
  Serial.print("Getting state... ");
  system_rtc_mem_read(RTC_OFFSET_MAGIC, buf, 2);
  Serial.print(buf[0]);
  Serial.print(" ");
  Serial.print(buf[1]);
  Serial.print(" ");
  if ((buf[0] != MAGIC_1) || (buf[1] != MAGIC_2)) {
    Serial.println(". No previous state. Assuming cold start with WiFi enabled.");
    return STATE_COLDSTART;
  } else {
    system_rtc_mem_read(RTC_OFFSET_STATE, buf, 1);
    Serial.print(buf[0]);
    Serial.print(". State is ");
    Serial.println(buf[0]);
    if (load_size) {
      system_rtc_mem_read(RTC_OFFSET_DATA, des_addr, load_size);
    }
    return buf[0];
  }
}

void setState(uint8_t state) {
  buf[0] = MAGIC_1;
  buf[1] = MAGIC_2;
  Serial.println("Writing magic number to RTC memory.");
  if (!system_rtc_mem_write(RTC_OFFSET_MAGIC, buf, 2)) {
    Serial.println("Writing magic number to RTC memory failed!");
  }
  buf[0] = state;
  Serial.print("Writing state to RTC memory: ");
  Serial.println(state);
  if (!system_rtc_mem_write(RTC_OFFSET_STATE, buf, 1)) {
    Serial.println("Writing state to RTC memory failed!");
  }
}

void reboot(uint32_t time_us, RFMode mode, const void *src_addr, uint16 save_size) {
  setState(mode);
  if (save_size) {
    Serial.print("Writing ");
    Serial.print(save_size);
    Serial.print(" bytes to RTC memory: ");
    for (int i=0; i<save_size; ++i) {
      Serial.print(((uint8_t *)src_addr)[i]);
      Serial.print(" "); }
    Serial.println("");
    if (!system_rtc_mem_write(RTC_OFFSET_DATA, src_addr, save_size)) {
      Serial.println("Writing measurement data to RTC memory failed!");
    }
  }
  Serial.print("Rebooting in mode ");
  Serial.println(mode);
  ESP.deepSleep(time_us, mode);
  delay(100);
  Serial.println("Should have rebooted by now.");
}

