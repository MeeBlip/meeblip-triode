;-------------------------------------------------------------------------------------------------------------------
; Meeblip triode	
;
;
;-------------------------------------------------------------------------------------------------------------------
;			DCA Output  
;-------------------------------------------------------------------------------------------------------------------
;            
;			Look up DCA value and output the DCA level amount to the second channel of the DAC chip. 


            ;---------------
            ;sound level:
            ;---------------

MLP_VCAENV:
            lds	    r16, ENV_INTEGR		

; added the non-linear vca envelope lookup
			ldi		r30, low( TAB_VCA)	;\
			ldi		r31, high( TAB_VCA)	;/ Z = &Tab
			rcall	TAB_BYTE			; r0 = 2..255 (load LEVEL into r16)

MLP_VCAOK:
			sts		LEVEL,r16

MLP_DCAOUT:

			; Control bit mask xxxx1011
			ldi		r17, 0b00001011 		; outputs signal through DACB 

; Shift MSB data word in r17:r16 left 4 bits, so it's packed 1011 dddd dddd xxxx

; Use DACB for DCA LEVEL amount
			lsl	    r16
		    rol	    r17		; shifted 1 bit left
			lsl	    r16
		    rol	    r17		; shifted 2 bits left
			lsl	    r16
		    rol	    r17		; shifted 3 bits left
			lsl	    r16
		    rol	    r17		; shifted 4 bits left

			; Pack LEVEL into two bytes for output to the DAC in the sample loop
			sts		LEVEL_H, r17
			sts		LEVEL_L, r16

; Pack REZ_H and REZ_L for the Digipot

			ldi		r30, 1
			ldi		r17, 0
			lds	r16, ACCENTED_REZ
			add		r16, r30
			adc		r17, ZERO	; Increase resonance value by 1 because digipot is 8 bits. Carry into LSB or r17 if needed
			sts		REZ_H, r17
			sts		REZ_L, r16
			