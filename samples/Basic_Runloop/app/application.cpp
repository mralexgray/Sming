
#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <Libraries/Runloop/Runloop.h>


Led led1 = Led(LEDPIN_1);


void init()
{

	WifiStation.config(WIFI_SSID, WIFI_PWD); 
	WifiStation.enable(true);
 
 	led1.setTimer(1000);
  led1.blink(true);

}
