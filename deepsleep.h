// define a probably unused enum constant for coldstart
#define STATE_COLDSTART WAKE_RFCAL + WAKE_RF_DISABLED
#define STATE_WIFI WAKE_RFCAL
#define STATE_NO_WIFI WAKE_RF_DISABLED

uint8_t getState(void *des_addr, uint16 load_size);
void reboot(uint32_t time_us, RFMode mode, const void *src_addr=NULL, uint16 save_size=0);

