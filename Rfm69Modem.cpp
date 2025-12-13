#include "Arduino.h"
#include "local.h"
#include <RH_RF69.h>
#include "Rfm69Modem.h"
#include "uart.h"
#include "rfm69.h"



static uint32_t led_timeout_ms;

static void led_on(uint32_t duration_ms)
{
    led_timeout_ms = millis() + duration_ms;

}


Rfm69Modem::Rfm69Modem(RH_RF69 *rf69p, uint8_t pin_rfm69_rst,uint8_t pin_led)
{
    _rf69p = rf69p;
    modem.pin_rfm69_rst   = pin_rfm69_rst;
    modem.pin_led         = pin_led;
    modem.led_timeout     = millis() + 4000;
    led_timeout_ms        = millis() + 4000;
}


void Rfm69Modem::initialize(char mod_tag, char mod_addr, uint8_t key[]){
	modem.tag             = mod_tag;
	modem.addr            = mod_addr;
    pinMode(modem.pin_rfm69_rst, OUTPUT);
    pinMode(modem.pin_led, OUTPUT);
    rfm69_initialize(_rf69p, modem.pin_rfm69_rst, key);
    uart_initialize(modem.tag, modem.addr);
    rfm69_set_led_cb(led_on);

}

void Rfm69Modem::modem_task(void){
    uart_rx_task();
	rfm69_receive_task();
    // rfm69_receive_message();
    // Serial.print("led_timeout_ms= "); Serial.print(led_timeout_ms);
    // Serial.print(" > "); Serial.println(millis());
    if (millis() < led_timeout_ms) digitalWrite(modem.pin_led,HIGH);
    else digitalWrite(modem.pin_led,LOW);
}


void Rfm69Modem::radiate(
	char to_tag, 
	char to_addr, 
	char func,
	char findx,
	char action,
	char *buff)
{
	char b[MAX_MESSAGE_LEN] = {0};
	b[0] = '<';
	b[1] = to_tag;
	b[2] = to_addr;
	b[3] = modem.tag;
	b[4] = modem.addr;
	b[5] = func;
	b[6] = findx;
	b[7] = action;
	strncpy(&b[8], buff, MAX_MESSAGE_LEN-8);
	uint8_t len = strlen(b);
	b[len] = '>';
	b[len+1] = 0x00;
	
    rfm69_radiate_msg(b);
}

//void Rfm69Modem::radiate_node_json(char *buff)
//{
//	uart_radiate_node_json(buff);
//}

void Rfm69Modem::radiate_node_json(
	char to_tag, 
	char to_addr, 
	char func,
	char findx,
	char action,
	char *buff)
{
	char b[MAX_MESSAGE_LEN] = {0};
	b[0] = '<';
	b[1] = to_tag;
	b[2] = to_addr;
	b[3] = modem.tag;
	b[4] = modem.addr;
	b[5] = func;
	b[6] = findx;
	b[7] = action;
	strncpy(&b[8], buff, MAX_MESSAGE_LEN-8);
	uint8_t len = strlen(b);
	b[len] = '>';
	b[len+1] = 0x00;
	
	uart_radiate_node_json(b);
}


bool Rfm69Modem::msg_is_avail(void)
{
	return rfm69_receive_message_is_avail();
}

void Rfm69Modem::get_msg(char *buff, uint8_t max_len, bool clr_avail)
{
	rfm69_get_message(buff, max_len, clr_avail);
}

void Rfm69Modem::get_msg_decode(char *buff, uint8_t max_len, bool clr_avail)
{
	uart_get_decoded_msg(buff, max_len, clr_avail);
	
}

int16_t Rfm69Modem::get_last_rssi(void)
{
	return rfm69_get_last_rssi(); 
}
