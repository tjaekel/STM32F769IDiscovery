/**
  ******************************************************************************
  * @file    app_ethernet.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22-April-2016
  * @brief   Ethernet specefic module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lwip/opt.h"
#include "main.h"
//#include "lcd_log.h"
#include "lwip/dhcp.h"
#include "app_ethernet.h"
#include "ethernetif.h"

#include "config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define MAX_DHCP_TRIES 				4
__IO uint8_t DHCP_state;

uint8_t IP_ADDR[4] = {0,0,0,0};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Notify the User about the network interface config status
  * @param  netif: the network interface
  * @retval None
  */
void User_notification(struct netif *netif) 
{
  if (netif_is_up(netif))
  {
    /* Update DHCP state machine */
    DHCP_state = DHCP_START;
  }
  else
  {  
    /* Update DHCP state machine */
    DHCP_state = DHCP_LINK_DOWN;
    //LCD_UsrLog ("The network cable is not connected \n");
  }

  if ( ! (SysConfig.netConfig & NET_DHCP))
  {
	  ip_addr_t ipaddr;
	  ip_addr_t netmask;
	  ip_addr_t gw;

      IP4_ADDR(&ipaddr, IP_ADDR0 ,IP_ADDR1 , IP_ADDR2 , IP_ADDR3 );
      IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
      IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
      netif_set_addr(netif, &ipaddr , &netmask, &gw);

      //inform GUI about alternative IP address
      IP_ADDR[0] = IP_ADDR0;
      IP_ADDR[1] = IP_ADDR1;
      IP_ADDR[2] = IP_ADDR2;
      IP_ADDR[3] = IP_ADDR3;
  }
}

/**
* @brief  DHCP Process
* @param  argument: network interface
* @retval None
*/
void DHCP_thread(void const * argument)
{
  struct netif *netif = (struct netif *) argument;
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;
  uint32_t IPaddress;
  
  for (;;)
  {
    switch (DHCP_state)
    {
    case DHCP_START:
      {
        netif->ip_addr.addr = 0;
        netif->netmask.addr = 0;
        netif->gw.addr = 0;
        IPaddress = 0;
        dhcp_start(netif);
        DHCP_state = DHCP_WAIT_ADDRESS;
        //LCD_UsrLog ("  State: Looking for DHCP server ...\n");
      }
      break;
      
    case DHCP_WAIT_ADDRESS:
      {
        /* Read the new IP address */
        IPaddress = netif->ip_addr.addr;
        
        if (IPaddress!=0) 
        {
          DHCP_state = DHCP_ADDRESS_ASSIGNED;	
          
          /* Stop DHCP */
          dhcp_stop(netif);

          IP_ADDR[3] = (uint8_t)(IPaddress >> 24);
          IP_ADDR[2] = (uint8_t)(IPaddress >> 16);
          IP_ADDR[1] = (uint8_t)(IPaddress >> 8);
          IP_ADDR[0] = (uint8_t)(IPaddress);
        }
        else
        {
          /* DHCP timeout */
          if (netif->dhcp->tries >= MAX_DHCP_TRIES)
          {
            DHCP_state = DHCP_TIMEOUT;
            
            /* Stop DHCP */
            dhcp_stop(netif);

            /* Static address used */
            IP4_ADDR(&ipaddr, IP_ADDR0 ,IP_ADDR1 , IP_ADDR2 , IP_ADDR3 );
            IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
            IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
            netif_set_addr(netif, &ipaddr , &netmask, &gw);

            //inform GUI about alternative IP address - but it does not work
#if 0
            IP_ADDR[0] = IP_ADDR0;
            IP_ADDR[1] = IP_ADDR1;
            IP_ADDR[2] = IP_ADDR2;
            IP_ADDR[3] = IP_ADDR3;
#endif
          }
        }
      }
      break;
    //XXXX why such one was not here?
    case DHCP_ADDRESS_ASSIGNED:
    case DHCP_TIMEOUT:
    	//terminate the thread
    	osThreadTerminate(NULL);
    	break;
      
    default: break;
    }
    
    /* wait 250 ms - not reliable with large delay ! */
    osDelay(250);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
