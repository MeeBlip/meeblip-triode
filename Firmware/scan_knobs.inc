;-------------------------------------------------------------------------------------------------------------------
; Meeblip triode		
;
;
;-------------------------------------------------------------------------------------------------------------------
;
;--------------------
;read potentiometers:
;--------------------

		    rcall	ADC_END			    ; r16 = AD(i)
		    lds	    r18, ADC_CHAN		;\
			sts		PREV_ADC_CHAN, r18   ; keep track of which ADC channel we're processing. 
			ldi		r28, low(ADC_0)
			ldi		r29, high(ADC_0)
			add		r28, r18
			adc		r29, ZERO
		    st	    Y, r16			    ; AD(i) --> ADC_i

;next channel:
		    inc	    r18
		    andi	r18, 0x07
		    sts	    ADC_CHAN,r18
		    rcall	ADC_START	        ; start conversion of next channel
			
; ------------------------------------------------------------------------------------------------------------------------
; Read potentiometer values
; ------------------------------------------------------------------------------------------------------------------------
;
			lds		r20, PREV_ADC_CHAN	; Only process the most recently scanned pot, store ADC channel 0..7 in r20

CHECK_0:
			cpi		r20, 0				; Update knob 0, LFO Depth?
			brne	CHECK_1
			rcall	POT_SCAN			; If so, check if parameter should be updated with pot value in r16
			tst		r17					
			breq	EXIT_POTS			; Skip update if pot hasn't been updated
		    sts	    LFOLEVEL,r16		
			sts		PANEL_LFOLEVEL, r16	

			; Check if MIDI set switch has been pressed. If so, set MIDI CHANNEL
			lds		r16, BUTTON_STATUS
			tst		r16
			breq	EXIT_POTS				; button hasn't been pressed, so skip
			cpi		r16, 3
			breq	EXIT_POTS
			ldi		r16, 8					; MIDI CHANNEL 8
			rcall	SET_MIDI_CHANNEL

			rjmp	END_POTS

CHECK_1:
			cpi		r20, 1				; Update knob 1, LFO rate?
			brne	CHECK_2
			rcall	POT_SCAN			; If so, check if parameter should be updated with pot value in r16
			tst		r17					
			breq	EXIT_POTS
			sts	    LFOFREQ,r16

			; Check if MIDI set switch has been pressed. If so, set MIDI CHANNEL
			lds		r16, BUTTON_STATUS
			tst		r16
			breq	EXIT_POTS				; button hasn't been pressed, so skip
			cpi		r16, 3
			breq	EXIT_POTS
			ldi		r16, 7					; MIDI CHANNEL 7
			rcall	SET_MIDI_CHANNEL
			rjmp	END_POTS

CHECK_2:
			cpi		r20, 2				; Update knob 2, Detune?
			brne	CHECK_3
			rcall	POT_SCAN			; If so, check if parameter should be updated with pot value in r16
			tst		r17					
			breq	EXIT_CHECK2			; Skip update if pot hasn't been updated
			sts		OSC_DETUNE, r16

			; Check if MIDI set switch has been pressed. If so, set MIDI CHANNEL
			lds		r16, BUTTON_STATUS
			tst		r16
			breq	EXIT_CHECK2				; button hasn't been pressed, so skip
			cpi		r16, 3
			breq	EXIT_CHECK2
			ldi		r16, 6					; MIDI CHANNEL 6
			rcall	SET_MIDI_CHANNEL

EXIT_CHECK2:
			lds		r16, OSC_DETUNE		; grab the patch value, just in case it hasn't been updated
			cpi		r16, 129			; lock oscillator frequencies if knob is centred (knob value of 127). 
			brlo	EXIT_CHECK2A
CONTINUE_CHECK2:
		    rcall	NONLINPOT		    ; AD6.1 --> DCO B detune with non-linear knob (center is tuned)
		    subi	r17, 128		     
		    sts	    DETUNEB_FRAC, r16	; Value -128.000..+127.996
		    sts	    DETUNEB_INTG, r17	
EXIT_POTS:
			rjmp	END_POTS
EXIT_CHECK2A:
			cpi		r16, 125
			brlo	CONTINUE_CHECK2		; If knob value is 125 - 128, set detune to zero and lock oscillators 
			ldi		r16, 0
			ldi		r17, 0
		    sts	    DETUNEB_FRAC, r16	; Detune 0
		    sts	    DETUNEB_INTG, r17	
			rjmp	END_POTS

CHECK_3:
			cpi		r20, 3				; Update knob 3, PORTAMENTO/GLIDE?
			brne	CHECK_4	
			rcall	POT_SCAN			; If so, check if parameter should be updated with pot value in r16
			tst		r17					
			breq	EXIT_CHECK3			; Skip update if pot hasn't been updated
			sts     PORTAMENTO_KNOB, r16

			; Check if MIDI set switch has been pressed. If so, set MIDI CHANNEL
			lds		r16, BUTTON_STATUS
			tst		r16
			breq	EXIT_CHECK3				; button hasn't been pressed, so skip
			cpi		r16, 3
			breq	EXIT_CHECK3
			ldi		r16, 5					; MIDI CHANNEL 5
			rcall	SET_MIDI_CHANNEL

EXIT_CHECK3:
			lds		r16, PORTAMENTO_KNOB ; grab the patch value, just in case it hasn't been updated
			lds		r17, FIRMWARE
			sbrs	r17, 0	
			sts		PORTAMENTO, r16		; If we're running wavetable firmware, don't update portamento using knob

			lsr		r16					; 0..127
			lsr		r16					; 0..63
			lsr		r16					; 0..31
			lsr		r16					; 0..15
			lsr		r16					; 0..8
			sts		WAVETABLE, r16		; Knob value selects one of 8 wavetables
			rjmp	END_POTS

CHECK_4:								; Filter resonance
			cpi		r20, 4				; Update knob 4, Filter resonance?
			brne	CHECK_5		
			rcall	POT_SCAN			; If so, check if parameter should be updated with pot value in r16
			tst		r17					
			breq	CHECK4_EXIT				; Skip update if pot hasn't been updated
			;cpi		r16, 0
			;brne	CHECK_4A
			;ldi		r16, 1
CHECK_4A:

			; scaling
			lsr		r16					; 50%
			mov		r17, r16
			lsr		r16					; 25%
			add		r16, r17			; 50% + 25%
			ldi		r17, 64
			add		r16, r17			; Add the value scaled to 75% + 64. 
			sts	    RESONANCE,r16	
			
			; Check if MIDI set switch has been pressed. If so, set MIDI CHANNEL
			lds		r16, BUTTON_STATUS
			tst		r16
			breq	CHECK4_EXIT				; button hasn't been pressed, so skip
			cpi		r16, 3
			breq	CHECK4_EXIT
			ldi		r16, 4					; MIDI CHANNEL 4
			rcall	SET_MIDI_CHANNEL							

CHECK4_EXIT:
			rjmp	END_POTS



CHECK_5:
			cpi		r20, 5				; Update knob 5, Filter Cutoff?
			brne	CHECK_6		
			rcall	POT_SCAN			; If so, check if parameter should be updated with pot value in r16
			tst		r17					
			breq	EXIT_CHECK5				; Skip update if pot hasn't been updated
			
			; scaling
			lsr		r16					; 50%
			mov		r17, r16
			lsr		r16					; 25%
			add		r16, r17			; 50% + 25%
			ldi		r17, 64
			add		r16, r17			; Add the value scaled to 75% + 64. 
			sts	    CUTOFF,r16	
			
			; Check if MIDI set switch has been pressed. If so, set MIDI CHANNEL
			lds		r16, BUTTON_STATUS
			tst		r16
			breq	EXIT_CHECK5				; button hasn't been pressed, so skip
			cpi		r16, 3
			breq	EXIT_CHECK5
			ldi		r16, 3					; MIDI CHANNEL 3
			rcall	SET_MIDI_CHANNEL	
EXIT_CHECK5:										
			rjmp	END_POTS

CHECK_6:	
			cpi		r20, 6				; Update knob 6, Filter Decay?
			brne	CHECK_7	
			rcall	POT_SCAN			; If so, check if parameter should be updated with pot value in r16	
			tst		r17					
			breq	CHECK_6B				; Skip update if pot hasn't been updated		
			sts	    KNOB_DCF_DECAY, r16	

			; Check if MIDI set switch has been pressed. If so, set MIDI CHANNEL
			lds		r16, BUTTON_STATUS
			tst		r16
			breq	CHECK_6B				; button hasn't been pressed, so skip
			cpi		r16, 3
			breq	CHECK_6B
			ldi		r16, 2					; MIDI CHANNEL 2
			rcall	SET_MIDI_CHANNEL

CHECK_6B:
			lds	    r16, KNOB_DCF_DECAY 		; Grab the value, in case it hasn't been updated (IE: MIDI CC parameter update)

			; xyzzy comment this out to change the sustain behaviour
			lds		r19, PATCH_SWITCH1	
			sbrs	r19, SW_SUSTAIN			
			rjmp	DCF_SUSTAIN_OFF				; Skip if sustain is off...
			ldi		r19, 255					; Sustain is on, so...
			sts		SUSTAINLEVEL2, r19			; Set sustain to maximum
			sts		DECAYTIME2, r19				; Set decay to maximum
			sts		RELEASETIME2, r16			; Set release time to value of decay knob
			rjmp	END_POTS
			; xyzzy end sustain

DCF_SUSTAIN_OFF:
			ldi		r19, 0						; Sustain is off, so...		
			sts		SUSTAINLEVEL2, r19			; Set sustain to minimum
			sts		DECAYTIME2, r16				; Set decay time to value of decay knob
			sts		RELEASETIME2, r16			; Set release time to value of decay knob
			rjmp	END_POTS	

CHECK_7:	
			cpi		r20, 7				; Update knob 7, Amp Decay?
			brne	END_POTS	
			rcall	POT_SCAN			; If so, check if parameter should be updated with pot value in r16	
			tst		r17					
			breq	CHECK_7B				; Skip update if pot hasn't been updated		
			sts	    KNOB_AMP_DECAY, r16	

			; Check if MIDI set switch has been pressed. If so, set MIDI CHANNEL
			lds		r16, BUTTON_STATUS
			tst		r16
			breq	CHECK_7B				; button hasn't been pressed, so skip
			cpi		r16, 3
			breq	CHECK_7B
			ldi		r16, 1					; MIDI CHANNEL 1
			rcall	SET_MIDI_CHANNEL

CHECK_7B:
			lds	    r16, KNOB_AMP_DECAY 		; Grab the value, in case it hasn't been updated (IE: MIDI CC parameter update)
			lds		r19, PATCH_SWITCH1
			sbrs	r19, SW_SUSTAIN				
			rjmp	AMP_SUSTAIN_OFF		    ; Skip if sustain is off...
			ldi		r19, 255					; Sustain is on, so...
			sts		SUSTAINLEVEL, r19			; Set sustain to maximum
			sts		DECAYTIME, r19				; Set decay to maximum
			sts		RELEASETIME, r16			; Set release time to value of decay knob
			rjmp	END_POTS

AMP_SUSTAIN_OFF:
			ldi		r19, 0						; Sustain is off, so...		
			sts		SUSTAINLEVEL, r19			; Set sustain to minimum
			sts		DECAYTIME, r16				; Set decay time to value of decay knob
			sts		RELEASETIME, r16			; Set release time to value of decay knob	

END_POTS: