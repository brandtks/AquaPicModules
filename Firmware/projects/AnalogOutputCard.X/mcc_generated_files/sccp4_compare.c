
/**
  SCCP4 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    sccp4.c

  @Summary
    This is the generated header file for the SCCP4 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides APIs for driver for SCCP4.
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
#include "sccp4_compare.h"

/** OC Mode.

  @Summary
    Defines the OC Mode.

  @Description
    This data type defines the OC Mode of operation.

*/

static uint16_t         gSCCP4Mode;

/**
  Section: Driver Interface
*/


void SCCP4_COMPARE_Initialize (void)
{
    // ON disabled; MOD Dual Edge Compare, Buffered; ALTSYNC disabled; SIDL disabled; OPS Each Time Base Period Match; CCPSLP disabled; TMRSYNC disabled; RTRGEN disabled; CCSEL disabled; ONESHOT disabled; TRIGEN disabled; T32 16 Bit; SYNC None; OPSSRC Timer Interrupt Event; TMRPS 1:1; CLKSEL SYSCLK; 
    CCP4CON1 = (0x5 & 0xFFFF7FFF); //Disabling CCPON bit
    //ASDGM disabled; ICGSM Level-Sensitive mode; ICS ICM4; SSDG disabled; AUXOUT Disabled; ASDG None; PWMRSEN disabled; OCAEN enabled; OENSYNC disabled; 
    CCP4CON2 = 0x1000000;
    //OETRIG disabled; OSCNT None; POLACE disabled; PSSACE Tri-state; 
    CCP4CON3 = 0x0;
    //SCEVT disabled; TRSET disabled; ICOV disabled; ASEVT disabled; ICGARM disabled; RBWIP disabled; TRCLR disabled; RAWIP disabled; TMRHWIP disabled; TMRLWIP disabled; PRLWIP disabled; 
    CCP4STAT = 0x0;
    //TMRL 0; TMRH 0; 
    CCP4TMR = 0x0;
    //PRH 0; PRL 1200; 
    CCP4PR = 0x4B0;
    //CMPA 0; 
    CCP4RA = 0x0;
    //CMPB 0; 
    CCP4RB = 0x0;
    //BUFL 0; BUFH 0; 
    CCP4BUF = 0x0;


    gSCCP4Mode = CCP4CON1bits.MOD;

}

void SCCP4_COMPARE_Tasks( void )
{
    if(IFS2bits.CCP4IF)
    {
        IFS2CLR= 1 << _IFS2_CCP4IF_POSITION;
    }
}


void SCCP4_COMPARE_TimerTasks( void )
{
    if(IFS2bits.CCT4IF)
    {
        IFS2CLR= 1 << _IFS2_CCT4IF_POSITION;
    }
}

void SCCP4_COMPARE_Start( void )
{
    /* Start the Timer */
    CCP4CON1SET = (1 << _CCP4CON1_ON_POSITION);
}
void SCCP4_COMPARE_Stop( void )
{
    /* Start the Timer */
    CCP4CON1CLR = (1 << _CCP4CON1_ON_POSITION);
}

void SCCP4_COMPARE_SingleCompare16ValueSet( uint16_t value )
{   
    CCP4RA = value;
}


void SCCP4_COMPARE_DualCompareValueSet( uint16_t priVal, uint16_t secVal )
{
    
    CCP4RA = priVal;
	
    CCP4RB = secVal;
}
void SCCP4_COMPARE_DualEdgeBufferedConfig( uint16_t priVal, uint16_t secVal )
{
    
    CCP4RA = priVal;
	
    CCP4RB = secVal;
}
void SCCP4_COMPARE_CenterAlignedPWMConfig( uint16_t priVal, uint16_t secVal )
{
    
    CCP4RA = priVal;
	
    CCP4RB = secVal;
}
void SCCP4_COMPARE_VariableFrequencyPulseConfig( uint16_t priVal )
{
    CCP4RA = priVal;
	
}
bool SCCP4_COMPARE_IsCompareCycleComplete( void )
{
    return(IFS2bits.CCP4IF);
}
bool SCCP4_COMPARE_TriggerStatusGet( void )
{
    return( CCP4STATbits.CCPTRIG );
    
}
void SCCP4_COMPARE_TriggerStatusSet( void )
{
    CCP4STATSET = (1 << _CCP4STAT_TRSET_POSITION);
}
void SCCP4_COMPARE_TriggerStatusClear( void )
{
    /* Clears the trigger status */
    CCP4STATCLR = (1 << _CCP4STAT_TRCLR_POSITION);
}
bool SCCP4_COMPARE_SingleCompareStatusGet( void )
{
    return( CCP4STATbits.SCEVT );
}
void SCCP4_COMPARE_SingleCompareStatusClear( void )
{
    /* Clears the trigger status */
    CCP4STATCLR = (1 << _CCP4STAT_SCEVT_POSITION);
    
}
/**
 End of File
*/
