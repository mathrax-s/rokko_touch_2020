/* ========================================
 *
 * Copyright MATHRAX LLC., 2024
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * MATHRAX LLC.
 *
 * ========================================
*/
#include <project.h>

int bright;
int slider_pos = 0;

int	current_sensor;	
int prev_sensor;
int	slider_start_pos;

//PROTOTYPE

int main()
{
    CyGlobalIntEnable;      /* Enable global interrupts */
    //CapSense
    CapSense_Start();
    //UART
    UART_1_Start();
    //PWM LED
    LED_CONTROL_Start();
    //RS485_CONTROL
    Pin_1_Write(1);
    
    //CapSense First Calibration
    CapSense_InitializeAllBaselines();
    
    //Start LED
    for(int i=0; i<4; i++){
        LED_CONTROL_WriteCompare(0);
        CyDelay(50);
        LED_CONTROL_WriteCompare(65535);
        CyDelay(50);
    }
    
    //loop
    for(;;)
    {
        if(CapSense_IsBusy() == CapSense_NOT_BUSY){
            
		    CapSense_ProcessAllWidgets();
            
    		current_sensor = CapSense_GetCentroidPos(0);
            
            //UART 9600 bps
            //TOUCH .. 1~99
            //NO TOUCH .. 0
    		if(prev_sensor == 0xFF){
    			slider_start_pos=(int)current_sensor;
                bright = 65535;
    			UART_1_UartPutChar((slider_start_pos)+1);
    		}else {

    			if(current_sensor==0xFF){
                    UART_1_UartPutChar(0);
    				slider_pos = 0;
    			}else{
    				if(current_sensor != prev_sensor){
    					bright = 65535;
    					UART_1_UartPutChar((int)(current_sensor)+1);
    					slider_pos = (int)current_sensor+1;
    				}
    			}
    		}   	
        	prev_sensor = current_sensor;
            
            CapSense_ScanAllWidgets();
        }
        LED_CONTROL_WriteCompare((uint32)bright);
        if(bright > 0){
            bright -= 1;
        }else{
            bright = 0;
        }
    }
    
    return 0;
}
