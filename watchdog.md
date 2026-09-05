rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)  
configsip: 0, SPIWP:0xee  
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00  
mode:DIO, clock div:2  
load:0x3fff0030,len:1184  
load:0x40078000,len:13232  
load:0x40080400,len:3028  
entry 0x400805e4  
Mutex created  
Task one has mutex  
E (10130) task_wdt: Task watchdog got triggered. The following tasks did not reset the watchdog in time:  
E (10130) task_wdt:  - IDLE0 (CPU 0)  
E (10130) task_wdt: Tasks currently running:  
E (10130) task_wdt: CPU 0: taskForceOne  
E (10130) task_wdt: CPU 1: IDLE1  
E (10130) task_wdt: Aborting.  
abort() was called at PC 0x400d930d on core 0  
Backtrace: 0x4008344d:0x3ffbea0c |<-CORRUPTED  
ELF file SHA256: 8eeafc51c5c74e8c  
Rebooting...   
ets Jul 29 2019 12:21:46  
