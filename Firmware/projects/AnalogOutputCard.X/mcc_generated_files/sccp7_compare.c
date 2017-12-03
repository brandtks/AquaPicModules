
/**
  SCCP7 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    sccp7.c

  @Summary
    This is the generated header file for the SCCP7 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides APIs for driver for SCCP7.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - pic24-dspic-pic32mm : v1.35
        Device            :  PIC32MM0064GPM028
    The generated drivers are tested against the following:
        Compiler          :  XC32 1.42
        MPLAB 	          :  MPLAB X 3.60
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include "sccp7_compare.h"

/** OC Mode.

  @Summary
    Defines the OC Mode.

  @Description
    This data type defines the OC Mode of operation.

*/

static uint16_t         gSCCP7Mode;

/**
  Section: Driver Interface
*/


void SCCP7_COMPARE_Initialize (void)
{
    // ON disabled; MOD Dual Edge Compare, Buffered; ALTSYNC disabled; SIDL disabled; OPS Each Time Base Period Match; CCPSLP disabled; TMRSYNC disabled; RTRGEN disabled; CCSEL disabled; ONESHOT disabled; TRIGEN disabled; T32 16 Bit; SYNC None; OPSSRC Timer Interrupt Event; TMRPS 1:1; CLKSEL SYSCLK; 
    CCP7CON1 = (0x5 & 0xFFFF7FFF); //Disabling CCPON bit
    //ASDGM disabled; ICGSM Level-Sensitive mode; ICS ICM7; SSDG disabled; AUXOUT Disabled; ASDG None; PWMRSEN disabled; OCAEN enabled; OENSYNC disabled; 
    CCP7CON2 = 0x1000000;
    //OETRIG disabled; OSCNT None; POLACE disabled; PSSACE Tri-state; 
    CCP7CON3 = 0x0;
    //SCEVT disabled; TRSET disabled; ICOV disabled; ASEVT disabled; ICGARM disabled; RBWIP disabled; TRCLR disabled; RAWIP disabled; TMRHWIP disabled; TMRLWIP disabled; PRLWIP disabled; 
    CCP7STAT = 0x0;
    //TMRL 0; TMRH 0; 
    CCP7TMR = 0x0;
    //PRH 0; PRL 1200; 
    CCP7PR = 0x4B0;
    //CMPA 0; 
    CCP7RA = 0x0;
    //CMPB 0; 
    CCP7RB = 0x0;
    //BUFL 0; BUFH 0; 
    CCP7BUF = 0x0;


    gSCCP7Mode = CCP7CON1bits.MOD;

}

void SCCP7_COMPARE_Tasks( void )
{
    if(IFS2bits.CCP7IF)
    {
        IFS2CLR= 1 << _IFS2_CCP7IF_POSITION;
    }
}


void SCCP7_COMPARE_TimerTasks( void )
{
    if(IFS2bits.CCT7IF)
    {
        IFS2CLR= 1 << _IFS2_CCT7IF_POSITION;
    }
}

void SCCP7_COMPARE_Start( void )
{
    /* Start the Timer */
    CCP7CON1SET = (1 << _CCP7CON1_ON_POSITION);
}
void SCCP7_COMPARE_Stop( void )
{
    /* Start the Timer */
    CCP7CON1CLR = (1 << _CCP7CON1_ON_POSITION);
}

void SCCP7_COMPARE_SingleCompare16ValueSet( uint16_t value )
{   
    CCP7RA = value;
}


void SCCP7_COMPARE_DualCompareValueSet( uint16_t priVal, uint16_t secVal )
{
    
    CCP7RA = priVal;
	
    CCP7RB = secVal;
}
void SCCP7_COMPARE_DualEdgeBufferedConfig( uint16_t priVal, uint16_t secVal )
{
    
    CCP7RA = priVal;
	
    CCP7RB = secVal;
}
void SCCP7_COMPARE_CenterAlignedPWMConfig( uint16_t priVal, uint16_t secVal )
{
    
    CCP7RA = priVal;
	
    CCP7RB = secVal;
}
void SCCP7_COMPARE_VariableFrequencyPulseConfig( uint16_t priVal )
{
    CCP7RA = priVal;
	
}
bool SCCP7_COMPARE_IsCompareCycleComplete( void )
{
    return(IFS2bits.CCP7IF);
}
bool SCCP7_COMPARE_TriggerStatusGet( void )
{
    return( CCP7STATbits.CCPTRIG );
    
}
void SCCP7_COMPARE_TriggerStatusSet( void )
{
    CCP7STATSET = (1 << _CCP7STAT_TRSET_POSITION);
}
void SCCP7_COMPARE_TriggerStatusClear( void )
{
    /* Clears the trigger status */
    CCP7STATCLR = (1 << _CCP7STAT_TRCLR_POSITION);
}
bool SCCP7_COMPARE_SingleCompareStatusGet( void )
{
    return( CCP7STATbits.SCEVT );
}
void SCCP7_COMPARE_SingleCompareStatusClear( void )
{
    /* Clears the trigger status */
    CCP7STATCLR = (1 << _CCP7STAT_SCEVT_POSITION);
    
}
/**
 End of File
*/
