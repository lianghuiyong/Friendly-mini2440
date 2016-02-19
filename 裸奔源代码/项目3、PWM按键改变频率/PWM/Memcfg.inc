;************************************************
; NAME    : MEMCFG.A
; DESC	  : Memory bank configuration file
; Revision: 02.28.2002 ver 0.0
; Revision: 03.11.2003 ver 0.0	Attatched for 2440
;************************************************

;Memory Area
;GCS6 32bit(64MB) SDRAM(0x3000_0000-0x33ff_ffff)


;BWSCON
DW8			EQU	(0x0)
DW16		EQU	(0x1)
DW32		EQU	(0x2)
WAIT		EQU	(0x1<<2)
UBLB		EQU	(0x1<<3)

	ASSERT :DEF:BUSWIDTH
    [ BUSWIDTH=16
B1_BWSCON	EQU (DW16)
B2_BWSCON	EQU (DW16)
B3_BWSCON	EQU (DW16)
B4_BWSCON	EQU (DW16)
B5_BWSCON	EQU (DW16)
B6_BWSCON	EQU (DW16)
B7_BWSCON	EQU (DW16)
    | ;BUSWIDTH=32		; 2440 EV board.
B1_BWSCON	EQU (DW32)	; Intel Strata(28F128), 32-bit, for nCS1 when NOR Flash booting.
B2_BWSCON	EQU (DW16)	; PCMCIA(PD6710), 16-bit
B3_BWSCON	EQU (DW16)	; Ethernet(CS8900), 16-bit
B4_BWSCON	EQU (DW16)	; NOR flash(AM29LV800B), 16-bit,  for nCS4 when NAND booting.
B5_BWSCON	EQU (DW16)	; A400/A410 Ext, 16-bit
B6_BWSCON	EQU (DW32)	; SDRAM(K4S561632C) 32MBx2, 32-bit
B7_BWSCON	EQU (DW32)	; N.C.
    ]

;BANK0CON

B0_Tacs		EQU	0x0	;0clk
B0_Tcos		EQU	0x0	;0clk
B0_Tacc		EQU	0x7	;14clk
B0_Tcoh		EQU	0x0	;0clk
B0_Tah		EQU	0x0	;0clk
B0_Tacp		EQU	0x0
B0_PMC		EQU	0x0	;normal

;BANK1CON
B1_Tacs		EQU	0x0	;0clk
B1_Tcos		EQU	0x0	;0clk
B1_Tacc		EQU	0x7	;14clk
B1_Tcoh		EQU	0x0	;0clk
B1_Tah		EQU	0x0	;0clk
B1_Tacp		EQU	0x0
B1_PMC		EQU	0x0	;normal

;Bank 2 parameter
B2_Tacs		EQU	0x0	;0clk
B2_Tcos		EQU	0x0	;0clk
B2_Tacc		EQU	0x7	;14clk
B2_Tcoh		EQU	0x0	;0clk
B2_Tah		EQU	0x0	;0clk
B2_Tacp		EQU	0x0
B2_PMC		EQU	0x0	;normal

;Bank 3 parameter
B3_Tacs		EQU	0x0	;0clk
B3_Tcos		EQU	0x0	;0clk
B3_Tacc		EQU	0x7	;14clk
B3_Tcoh		EQU	0x0	;0clk
B3_Tah		EQU	0x0	;0clk
B3_Tacp		EQU	0x0
B3_PMC		EQU	0x0	;normal

;Bank 4 parameter
B4_Tacs		EQU	0x0	;0clk
B4_Tcos		EQU	0x0	;0clk
B4_Tacc		EQU	0x7	;14clk
B4_Tcoh		EQU	0x0	;0clk
B4_Tah		EQU	0x0	;0clk
B4_Tacp		EQU	0x0
B4_PMC		EQU	0x0	;normal

;Bank 5 parameter
B5_Tacs		EQU	0x0	;0clk
B5_Tcos		EQU	0x0	;0clk
B5_Tacc		EQU	0x7	;14clk
B5_Tcoh		EQU	0x0	;0clk
B5_Tah		EQU	0x0	;0clk
B5_Tacp		EQU	0x0
B5_PMC		EQU	0x0	;normal

;Bank 6 parameter
B6_MT		EQU	0x3	;SDRAM
B6_Trcd		EQU	0x2	;4clk
B6_SCAN		EQU	0x1	;9bit

;Bank 7 parameter
B7_MT		EQU	0x3	;SDRAM
B7_Trcd		EQU	0x2	;4clk
B7_SCAN		EQU	0x1	;9bit

;REFRESH parameter
REFEN		EQU	0x1	;Refresh enable
TREFMD		EQU	0x0	;CBR(CAS before RAS)/Auto refresh
Trp			EQU	0x2	;4clk
Trc			EQU	0x2	;6clk

Tchr		EQU	0x2	;3clk
REFCNT		EQU	1653	;period=7.8us, HCLK=50.8Mhz, (2048+1-7.8*50.8)

	END
