#ifndef __RFM69MODEM_H__
#define __RFM69MODEM_H__

#undef      MODEM_DEBUG_PRINT
#include "local.h"
#include <RH_RF69.h>



typedef struct
{
    char        tag;
    char        addr;    
    uint8_t 	pin_rfm69_rst;
    uint8_t 	pin_led;
    uint32_t 	led_timeout;
} rf_modem_st;

class Rfm69Modem
{
    private:
        rf_modem_st modem;

    public:
		RH_RF69   *_rf69p;
		Rfm69Modem(RH_RF69 *rf69p, char mod_tag, char mod_addr, uint8_t pin_rfm69_rst,uint8_t pin_led);

        static void led_on(uint32_t duration_ms);

        void initialize(uint8_t key[]);

        void modem_task(void);

        void radiate(char *buff);

		void radiate_node_json(char *buff);
		
		bool msg_is_avail(void);
		
		void receive(char *buff, uint8_t max_len, bool clr_avail);

};

#endif