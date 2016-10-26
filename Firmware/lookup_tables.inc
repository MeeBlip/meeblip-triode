;-------------------------------------------------------------------------------------------------------------------
; Meeblip triode		
;
;
;-------------------------------------------------------------------------------------------------------------------
; MIDI Control Change parameter table
;-------------------------------------------------------------------------------------------------------------------
;
; MIDI CC parameters with an offset from MIDICC. They are automatically
; stored for use, just use the variable name to access their value. 
 

MIDICC:         		.byte $80 		; 128 bytes reserved in RAM for table
  .equ MIDIMODWHEEL		= MIDICC + $01

    ; Knobs - potentiometer 0 through 7
  .equ PANEL_LFOLEVEL 	= MIDICC + 48 
  .equ LFOFREQ			= MIDICC + 49
  .equ OSC_DETUNE 		= MIDICC + 50
  .equ PORTAMENTO_KNOB	= MIDICC + 51
  .equ RESONANCE		= MIDICC + 52
  .equ CUTOFF 			= MIDICC + 53
  .equ KNOB_DCF_DECAY 	= MIDICC + 54
  .equ KNOB_AMP_DECAY	= MIDICC + 55
  .equ ACCENT 			= MIDICC + 56 ; Not on front panel, but available through MIDI CC 
  .equ VCFENVMOD		= MIDICC + 57 ; Not on front panel, but available through MIDI CC
  .equ PULSE_KNOB_LIMITED = MIDICC + 58 ; Not on front panel, but available through MIDI CC
  .equ KNOB_DCF_ATTACK	  = MIDICC + 59 ; Not on front panel, but available through MIDI CC
  .equ KNOB_AMP_ATTACK	  = MIDICC + 60 ; Not on front panel, but available through MIDI CC
  .equ PORTAMENTO		= MIDICC + 61 ; Portamento control when in Wavetable mode
  
  ; Panel switches 0..7
  ; Switches 1
  .equ S_SUSTAIN		= MIDICC + 64
  .equ S_SUBOSC			= MIDICC + 65
  .equ S_PWM_SWEEP		= MIDICC + 66	; Not on front panel, but available through MIDI CC
  .equ S_LFO_DEST		= MIDICC + 67
  .equ S_SAWTOOTH		= MIDICC + 68
  .equ S_LFO_RANDOM		= MIDICC + 69	; Not on front panel, but available through MIDI CC
  .equ S_LFO_RETRIGGER	= MIDICC + 70	; Not on front panel, but available through MIDI CC	



;-------------------------------------------------------------------------------------------------------------------



;stack: 0x0A3..0x25F
            .ESEG

;-------------------------------------------------------------------------------------------------------------------
;			V E C T O R   T A B L E
;-------------------------------------------------------------------------------------------------------------------
            .CSEG

		    jmp	RESET		            ; RESET

		    jmp	IRQ_NONE	            ; INT0
		    jmp	IRQ_NONE	            ; INT1
		    jmp	IRQ_NONE	            ; INT2

		    jmp	IRQ_NONE	            ; TIMEr2 COMP
		    jmp	IRQ_NONE	            ; TIMEr2 OVF

		    jmp	IRQ_NONE	            ; TIMEr1 CAPT
		    jmp	IRQ_NONE	            ; TIMEr1 COMPA
		    jmp	IRQ_NONE	            ; TIMEr1 COMPB
    		jmp	IRQ_NONE	            ; TIMEr1 OVF

		    jmp	TIM0_CMP	            ; TIMEr0 COMPA
		    jmp	IRQ_NONE	            ; TIMEr0 OVF

		    jmp	IRQ_NONE	            ; SPI,STC

		    jmp	UART_RXC	            ; UART, RX COMPLETE
		    jmp	IRQ_NONE	            ; UART,UDRE
		    jmp	IRQ_NONE	            ; UART, TX COMPLETE

		    jmp	IRQ_NONE	            ; ADC CONVERSION COMPLETE

		    jmp	IRQ_NONE	            ; EEPROM READY

		    jmp	IRQ_NONE	            ; ANALOG COMPARATOR

            jmp IRQ_NONE                ; 2-Wire Serial Interface

            jmp IRQ_NONE                ; STORE PROGRAM MEMORY READY

IRQ_NONE:
            reti
;-------------------------------------------------------------------------------------------------------------------
;			R O M   T A B L E S
;-------------------------------------------------------------------------------------------------------------------
;
; Note Phase Deltas at 40 kHz sample rate
;
;  				NOTE PHASE DELTA = 2 ^ 24 * Freq / SamplingFreq
;   	So... 	Note zero calc: 2 ^ 24 * 8.175799 / 40000 = 3429.17864 (stored as 00 0D 65.2E)
;-------------------------------------------------------------------------------------------------------------------

    
DELTA_C:
            .DW	0x652E		            ;\
		    .DW	0x000D		            ;/ note  0 ( 8.175799 Hz) 

DELTA_CIS:
            .DW	0x3117		            ;\
		    .DW	0x000E		            ;/ note  1 ( 8.661957 Hz) 

DELTA_D:
            .DW	0x091F		            ;\
		    .DW	0x000F		            ;/ note  2 ( 9.177024 Hz) 

DELTA_DIS:
            .DW	0xEE01		            ;\
		    .DW	0x000F		            ;/ note  3 ( 9.722718 Hz) 

DELTA_E:
            .DW	0xE07F		            ;\
		    .DW	0x0010		            ;/ note  4 (10.300861 Hz) 

DELTA_F:
            .DW	0xE167		            ;\
		    .DW	0x0011		            ;/ note  5 (10.913382 Hz) 

DELTA_FIS:
            .DW	0xF197		            ;\
		    .DW	0x0012		            ;/ note  6 (11.562326 Hz) 

DELTA_G:
            .DW	0x11F6		            ;\
		    .DW	0x0014		            ;/ note  7 (12.249857 Hz) 

DELTA_GIS:
            .DW	0x437B		            ;\
		    .DW	0x0015		            ;/ note  8 (12.978272 Hz) 

DELTA_A:
            .DW	0x872B		            ;\
		    .DW	0x0016		            ;/ note  9 (13.750000 Hz) 

DELTA_AIS:
            .DW	0xDE1A		            ;\
		    .DW	0x0017		            ;/ note 10 (14.567618 Hz) 

DELTA_H:
            .DW	0x496D		            ;\
		    .DW	0x0019		            ;/ note 11 (15.433853 Hz) 

DELTA_C1:
            .DW	0xCA5B		            ;\
		    .DW	0x001A		            ;/ note 12 (16.351598 Hz) 


;-----------------------------------------------------------------------------
;
; Lookup Tables
;
; VCF filter cutoff - 128 bytes
; Time to Rate table for calculating amplitude envelopes - 128 bytes
; VCA non-linear level conversion - 256 bytes
;
;-----------------------------------------------------------------------------
; VCF Filter Cutoff
;
; value = (16th root of 2)**(index+24)
;

; Digital VCF table (starts at four)

TAB_VCF:
	.db	  4,   4,   4,   4,   4,   4,   4,   4		;   0
	.db	  4,   4,   4,   4,   4,   5,   5,   5		;   8
	.db	  5,   5,   5,   5,   5,   5,   5,   5		;  16
	.db	  5,   5,   5,   6,   6,   6,   6,   6		;  24
	.db	  6,   6,   6,   6,   6,   7,   7,   7		;  32
	.db	  7,   7,   7,   7,   7,   8,   8,   8		;  40

	.db	  8,   8,   9,   9,   9,  10,  10,  11		;  48
	.db	 11,  12,  12,  13,  14,  14,  15,  16		;  56
	.db	 16,  17,  18,  19,  19,  20,  21,  22		;  64
	.db	 23,  24,  25,  26,  28,  29,  30,  31		;  72
	.db	 33,  34,  36,  38,  39,  41,  43,  45		;  80
	.db	 47,  49,  51,  53,  56,  58,  61,  63		;  88
	.db	 66,  69,  72,  76,  79,  82,  86,  90		;  96
	.db	 94,  98, 103, 107, 112, 117, 122, 127		; 104
	.db	133, 139, 145, 152, 158, 165, 173, 181		; 112
	.db	189, 197, 206, 215, 224, 234, 245, 255		; 120

; Analog VCF table (offset to start at 25)
TAB_VCF2:
	.db	 25, 25, 25, 25, 25, 25, 25, 25
	.db	 25, 25, 25, 25, 25, 25, 25, 26
	.db	 26, 26, 26, 26, 26, 26, 26, 26
	.db	 26, 27, 27, 27, 27, 27, 27, 27
	.db	 28, 28, 28, 28, 28, 29, 29, 29
	.db	 29, 29, 29, 29, 30, 30, 30, 30
	.db	 31, 31, 32, 32, 32, 33, 33, 34
	.db	 34, 35, 35, 36, 37, 37, 38, 38
	.db	 38, 39, 40, 41, 41, 42, 43, 44
	.db	 45, 46, 47, 47, 49, 50, 51, 52
	.db	 54, 55, 56, 58, 59, 61, 63, 65
	.db	 66, 68, 70, 72, 75, 76, 79, 81
	.db	 84, 86, 89, 93, 95, 98, 102, 105
	.db	 109, 112, 117, 121, 125, 130, 134, 139
	.db	 144, 149, 155, 161, 167, 173, 180, 187
	.db	 194, 202, 210, 218, 226, 235, 245, 255


;-----------------------------------------------------------------------------
; 
; Time to Rate conversion table for envelope timing.
; 
; lfo:
;  update values for 32us update rate
;  LFO_INTEGR overflows all 256*32us = 8.192 ms
;
; formula  Tof = 256*32us*2^16/N
;          LFOfreq = 1/Tof

; Rate value =	Rmin * Q^i with Q = (Rmax/Rmin)^(1/31) = 1,286111766

; xyzzy AWE's 128 value table
TIMETORATE:

	;	value		  index	 time
	.dw	50936		; 0	   10,540
	.dw	47997		; 1	   11,185
	.dw	45227		; 2	   11,870
	.dw	42617		; 3	   12,597
	.dw	40158		; 4	   13,369
	.dw	37841		; 5	   14,187
	.dw	35657		; 6	   15,056
	.dw	33600		; 7	   15,978
	.dw	31661		; 8	   16,957
	.dw	29834		; 9	   17,995
	.dw	28112		; 10	   19,097
	.dw	26490		; 11	   20,266
	.dw	24962		; 12	   21,508
	.dw	23521		; 13	   22,825
	.dw	22164		; 14	   24,222
	.dw	20885		; 15	   25,706
	.dw	19680		; 16	   27,280
	.dw	18544		; 17	   28,950
	.dw	17474		; 18	   30,723
	.dw	16466		; 19	   32,605
	.dw	15516		; 20	   34,601
	.dw	14620		; 21	   36,720
	.dw	13777		; 22	   38,969
	.dw	12982		; 23	   41,355
	.dw	12233		; 24	   43,888
	.dw	11527		; 25	   46,575
	.dw	10862		; 26	   49,427
	.dw	10235		; 27	   52,454
	.dw	 9644		; 28	   55,666
	.dw	 9088		; 29	   59,075
	.dw	 8563		; 30	   62,693
	.dw	 8069		; 31	   66,532
	.dw	 7604		; 32	   70,606
	.dw	 7165		; 33	   74,930
	.dw	 6751		; 34	   79,518
	.dw	 6362		; 35	   84,388
	.dw	 5995		; 36	   89,555
	.dw	 5649		; 37	   95,039
	.dw	 5323		; 38	  100,859
	.dw	 5016		; 39	  107,036
	.dw	 4726		; 40	  113,590
	.dw	 4454		; 41	  120,546
	.dw	 4197		; 42	  127,928
	.dw	 3954		; 43	  135,762
	.dw	 3726		; 44	  144,076
	.dw	 3511		; 45	  152,899
	.dw	 3309		; 46	  162,262
	.dw	 3118		; 47	  172,198
	.dw	 2938		; 48	  182,743
	.dw	 2768		; 49	  193,934
	.dw	 2609		; 50	  205,810
	.dw	 2458		; 51	  218,413
	.dw	 2316		; 52	  231,788
	.dw	 2183		; 53	  245,982
	.dw	 2057		; 54	  261,046
	.dw	 1938		; 55	  277,031
	.dw	 1826		; 56	  293,996
	.dw	 1721		; 57	  312,000
	.dw	 1621		; 58	  331,106
	.dw	 1528		; 59	  351,382
	.dw	 1440		; 60	  372,899
	.dw	 1357		; 61	  395,735
	.dw	 1278		; 62	  419,968
	.dw	 1205		; 63	  445,686
	.dw	 1135		; 64	  472,979
	.dw	 1070		; 65	  501,943
	.dw	 1008		; 66	  532,680
	.dw	  950		; 67	  565,300
	.dw	  895		; 68	  599,918
	.dw	  843		; 69	  636,655
	.dw	  795		; 70	  675,642
	.dw	  749		; 71	  717,017
	.dw	  706		; 72	  760,925
	.dw	  665		; 73	  807,522
	.dw	  626		; 74	  856,972
	.dw	  590		; 75	  909,451
	.dw	  556		; 76	  965,143
	.dw	  524		; 77	 1024,246
	.dw	  494		; 78	 1086,968
	.dw	  465		; 79	 1153,531
	.dw	  439		; 80	 1224,170
	.dw	  413		; 81	 1299,135
	.dw	  389		; 82	 1378,691
	.dw	  367		; 83	 1463,118
	.dw	  346		; 84	 1552,715
	.dw	  326		; 85	 1647,800
	.dw	  307		; 86	 1748,706
	.dw	  289		; 87	 1855,792
	.dw	  273		; 88	 1969,436
	.dw	  257		; 89	 2090,039
	.dw	  242		; 90	 2218,028
	.dw	  228		; 91	 2353,854
	.dw	  215		; 92	 2497,997
	.dw	  203		; 93	 2650,968
	.dw	  191		; 94	 2813,306
	.dw	  180		; 95	 2985,586
	.dw	  169		; 96	 3168,415
	.dw	  160		; 97	 3362,440
	.dw	  150		; 98	 3568,347
	.dw	  142		; 99	 3786,863
	.dw	  134		; 100	 4018,760
	.dw	  126		; 101	 4264,858
	.dw	  119		; 102	 4526,027
	.dw	  112		; 103	 4803,189
	.dw	  105		; 104	 5097,323
	.dw	   99		; 105	 5409,469
	.dw	   94		; 106	 5740,731
	.dw	   88		; 107	 6092,278
	.dw	   83		; 108	 6465,353
	.dw	   78		; 109	 6861,273
	.dw	   74		; 110	 7281,439
	.dw	   69		; 111	 7727,335
	.dw	   65		; 112	 8200,537
	.dw	   62		; 113	 8702,716
	.dw	   58		; 114	 9235,647
	.dw	   55		; 115	 9801,213
	.dw	   52		; 116	10401,413
	.dw	   49		; 117	11038,367
	.dw	   46		; 118	11714,328
	.dw	   43		; 119	12431,682
	.dw	   41		; 120	13192,964
	.dw	   38		; 121	14000,866
	.dw	   36		; 122	14858,242
	.dw	   34		; 123	15768,121
	.dw	   32		; 124	16733,718
	.dw	   30		; 125	17758,446
	.dw	   28		; 126	18845,926
	.dw	   27		; 127	20000,000




;-----------------------------------------------------------------------------
;
; VCA non-linear level conversion 
;
; Amplitude level lookup table. Envelopes levels are calculated as linear 
; and then converted to approximate an exponential saturation curve.
;
; polynomial y = a	+ bx + cx2 + dx3
; with coefficients…
;    a  0
;    b  0.210841569
;    c  0.000177823
;    d  1.14E-05

TAB_VCA:
	.db	  0,   0,   0,   1,   1,   1,   1,   1		;   0
	.db	  2,   2,   2,   2,   3,   3,   3,   3		;   8
	.db	  3,   4,   4,   4,   4,   5,   5,   5		;  16
	.db	  5,   6,   6,   6,   6,   7,   7,   7		;  24
	.db	  7,   8,   8,   8,   8,   9,   9,   9		;  32
	.db	  9,  10,  10,  10,  11,  11,  11,  11		;  40
	.db	 12,  12,  12,  13,  13,  13,  14,  14		;  48
	.db	 14,  15,  15,  15,  16,  16,  16,  17		;  56
	.db	 17,  18,  18,  18,  19,  19,  20,  20		;  64
	.db	 20,  21,  21,  22,  22,  23,  23,  23		;  72
	.db	 24,  24,  25,  25,  26,  26,  27,  27		;  80
	.db	 28,  28,  29,  29,  30,  30,  31,  31		;  88
	.db	 32,  33,  33,  34,  34,  35,  35,  36		;  96
	.db	 37,  37,  38,  39,  39,  40,  41,  41		; 104
	.db	 42,  43,  43,  44,  45,  45,  46,  47		; 112
	.db	 48,  48,  49,  50,  51,  51,  52,  53		; 120

	.db	 54,  55,  56,  56,  57,  58,  59,  60		; 128
	.db	 61,  62,  63,  63,  64,  65,  66,  67		; 136
	.db	 68,  69,  70,  71,  72,  73,  74,  75		; 144
	.db	 76,  77,  78,  80,  81,  82,  83,  84		; 152
	.db	 85,  86,  87,  89,  90,  91,  92,  93		; 160
	.db	 95,  96,  97,  98, 100, 101, 102, 104		; 168
	.db	105, 106, 108, 109, 110, 112, 113, 115		; 176
	.db	116, 118, 119, 120, 122, 123, 125, 126		; 184
	.db	128, 130, 131, 133, 134, 136, 138, 139		; 192
	.db	141, 142, 144, 146, 148, 149, 151, 153		; 200
	.db	154, 156, 158, 160, 162, 164, 165, 167		; 208
	.db	169, 171, 173, 175, 177, 179, 181, 183		; 216
	.db	185, 187, 189, 191, 193, 195, 197, 199		; 224
	.db	201, 203, 206, 208, 210, 212, 214, 217		; 232
	.db	219, 221, 224, 226, 228, 231, 233, 235		; 240
	.db	238, 240, 243, 245, 247, 250, 252, 255		; 248