;/*****************************************************************************/
;/* STM32F10xOPT.S: STM32F10x Flash Option Bytes                              */
;/*****************************************************************************/
;/* <<< Use Configuration Wizard in Context Menu >>>                          */
;/*****************************************************************************/
;/* This file is part of the uVision/ARM development tools.                   */
;/* Copyright (c) 2005-2008 Keil Software. All rights reserved.               */
;/* This software may only be used under the terms of a valid, current,       */
;/* end user licence from KEIL for a compatible version of KEIL software      */
;/* development tools. Nothing else gives you the right to use this software. */
;/*****************************************************************************/


;// <e> Flash Option Bytes
FLASH_OPT       EQU     1

;//   <o0.0> Flash Read Protection
RDP             EQU     0

;// <h> Flash Write Protection
;//   <o0.0> Page 0..3
;//   <o0.1> Page 4..7
;//   <o0.2> Page 8..11
;//   <o0.3> Page 12..15
;//   <o0.4> Page 16..19
;//   <o0.5> Page 20..23
;//   <o0.6> Page 24..27
;//   <o0.7> Page 28..31
;//   <o1.0> Page 32..35
;//   <o1.1> Page 36..39
;//   <o1.2> Page 40..43
;//   <o1.3> Page 44..47
;//   <o1.4> Page 48..51
;//   <o1.5> Page 52..55
;//   <o1.6> Page 56..59
;//   <o1.7> Page 60..63
;//   <o2.0> Page 64..67
;//   <o2.1> Page 68..71
;//   <o2.2> Page 72..75
;//   <o2.3> Page 76..79
;//   <o2.4> Page 80..83
;//   <o2.5> Page 84..87
;//   <o2.6> Page 88..91
;//   <o2.7> Page 92..95
;//   <o3.0> Page 96..99
;//   <o3.1> Page 100..103
;//   <o3.2> Page 104..107
;//   <o3.3> Page 108..111
;//   <o3.4> Page 112..115
;//   <o3.5> Page 116..119
;//   <o3.6> Page 120..123
;//   <o3.7> Page 124..127
;// </h>
nWRP0           EQU     0x00
nWRP1           EQU     0x00
nWRP2           EQU     0x00
nWRP3           EQU     0x00
WRP0            EQU     nWRP0:EOR:0xFF
WRP1            EQU     nWRP1:EOR:0xFF
WRP2            EQU     nWRP2:EOR:0xFF
WRP3            EQU     nWRP3:EOR:0xFF

;// <h> User Configuration
;//   <o0.0> WDG_SW     
;//          <0=> HW Watchdog <1=> SW Watchdog
;//   <o0.1> nRST_STOP  <i> Generate Reset when entering STOP Mode
;//          <0=> Enabled <1=> Disabled
;//   <o0.2> nRST_STDBY <i> Generate Reset when entering Standby Mode
;//          <0=> Enabled <1=> Disabled
;// </h>
USER            EQU     0xFF
nUSER           EQU     USER:EOR:0xFF

;// <h> User Data
;//   <o0> Byte 0 <0x00-0xFF>
;//   <o1> Byte 1 <0x00-0xFF>
;// </h>
DATA0           EQU     0xFF
DATA1           EQU     0xFF
nDATA0          EQU     DATA0:EOR:0xFF
nDATA1          EQU     DATA1:EOR:0xFF

;// </e>


                IF      FLASH_OPT <> 0
                AREA    |.ARM.__AT_0x1FFFF800|, CODE, READONLY
                IF      RDP <> 0
                DCB     0x00,  0xFF
                ELSE
                DCB     0xA5,  0x5A
                ENDIF
                DCB     USER,  nUSER
                DCB     DATA0, nDATA0, DATA1, nDATA1
                DCB     WRP0,  nWRP0,  WRP1,  nWRP1
                DCB     WRP2,  nWRP2,  WRP3,  nWRP3
                ENDIF


                END
