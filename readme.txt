CSE522 Team 10
Shubham Nandanwankar 1213350370
Achal Shah 	     1213294158 

Readme file to run main.c, $ZEPHYR_BASE is enviornment variable for your zephyr directory


1. Extract HCSR_app.zip file to "$ZEPHYR_BASE/samples/" directory
2. Go into the directory using "cd $ZEPHYR_BASE/samples/HCSR_app" directory
3. make build directory here using "mkdir build" command
4. Check version of your cmake using "cmake --version" if it is 3.8 or above then go to step 6 else perform step 5 as well
5. Execute a command "source $ZEPHYR_HOME/zephyr-env.sh" this will update our cmake version
6. Now extract the hcsr04 driver files from the hcsr04.zip file into the "$ZEPHYR_BASE/drivers/sensor".So now you will have "CMakeLists.txt","hcsr04.c","hcsr04.h" and "Kconfig" files in "$ZEPHYR_BASE/drivers/sensor/hcsr04" directory.
7. Now you need to include the CMakeLists.txt and Kconfig of hcsr04 folder in that of the sensor folder. For that make the following changes in these files in the "$ZEPHYR_BASE/drivers/sensor" directory:
   - Add "add_subdirectory_ifdef(CONFIG_HCSR		hcsr04)" line in the CMakeLists.txt file
   - Add "source "drivers/sensor/hcsr04/Kconfig" line the Kconfig file 

8. Now extract the EEPROM driver files from the i2c_driver.zip file into the "$ZEPHYR_BASE/drivers/flash".So now you will have "CMakeLists.txt","FC256.c" and "Kconfig" files in "$ZEPHYR_BASE/drivers/sensor/flash" directory. NOTE: If asked to replace the existing files the do replace it as we have sent the files after adding the required commands in the given "CMakeLists.txt" and "Kconfig".
9. Now go to "cd $ZEPHYR_BASE/samples/HCSR_app/build" directory.
10. cmake using "cmake -DBOARD=galileo .."
11. Execute command "make", this command will generate multiple file as well as directory into your "$ZEPHYR_BASE/samples/HCSR_app/build" directory
12. Goto directory "$ZEPHYR_BASE/samples/HCSR_app/build/zephyr"
13. Copy zephyr.strip file into the /kernel directory of your sd card
14 Now connect the distance sensors and EEPROM as follows:
  - Sensors : Trigger pin : IO1
                 Echo pin : IO2
	     (Both sensor instances are created and works properly if the board is booteed again after enabling a particular sensor. But if change the enabling without rebooting then for one of the sensor it works properly but for the next one it loops infinitely in the callback handler)	

  - EEPROM : Write protection pin IO0
             A2, A1, A0 : Connected to Ground (Hardcoded EEPROM address)
             SDA(Pin 5 of EEPROM): IO(18)
             SCL(Pin 6 of EEPROM): IO(19)
             
15.Connect to the console using "putty" or "sudo screen /dev/ttyUSB0 115200"  
16.Reset Galileo board
17. The driver would be initialized at the boot time and shell module needs to be executed. For that hit enter and type "select shell_mod"
18. For enabling a sensor type "enable 1" or "Enable 2" in the shell depending on the sensor you want to use.
19. For writing the pages into the EEPROM type "Start __" in which write the number of pages you want to write on the EEPROM in place of __.
20. For dummping the pages from the EEPROM type "Dump __ __" in which write the range of pages you want to write on the EEPROM in place of __ __, for example Dump 0 4 will show 5 entries from EEPROM from 0 to 4 both including
21. In any case of confusion use "help" command on shell prompt 


References:
1. Zephyr 1.10.0 documentation 
2. sample code of tmp007 driver in zephyr/drivers/sensor
3. sample code of i2c_fujitsu_fram in zephyr/samples/drive
