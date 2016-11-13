;-------------------------------------------------------------------------------------------------------------------
; Meeblip triode		
;
;
;-------------------------------------------------------------------------------------------------------------------
;			Envelope Generation - Dual envelopes
;-------------------------------------------------------------------------------------------------------------------
;
;			Envelope is routed to amplitude and filter cutoff

;check envelope phase:
		    lds	    r17, ENVPHASE
		    lds	    r16, KNOB_AMP_ATTACK

MLP_PHASE:  cpi	    r17, 1
		    breq    MLP_ENVAR		    ; when "attack"
			lds		r16, DECAYTIME
			cpi		r17, 2
			breq	MLP_ENVAR			; when "decay"
		    lds	    r16, RELEASETIME
		    cpi	    r17, 4
		    breq	MLP_ENVAR		    ; when "release"
			lds		r22, SUSTAINLEVEL
			cpi		r17, 3			; when sustain
			breq	MLP_ESUSTAIN
			rjmp	MLP_EEXIT		    ; when "stop" or "sustain"

;calculate dL:

MLP_ENVAR:
            rcall	ADCTORATE           ; r19:r18:r17:r16 = rate of rise/fall
		    lds	    r22, DELTAT_L		;\
		    lds	    r23, DELTAT_H		;/ r23:r22 = dT
		    rcall	MUL32X16		    ; r18:r17:r16 = dL

;add/subtract dL to/from L:
		    lds	    r19, ENV_FRAC_L		;\
		    lds	    r20, ENV_FRAC_H		; > r21:r20:r19 = L
    		lds	    r21, ENV_INTEGR		;/
		    lds	    r22, ENVPHASE
		    cpi	    r22, 4
		    breq    MLP_ERELEASE

MLP_EATTACK:
			cpi	    r22, 2				
		    breq    MLP_EDECAY			
		    add	    r19, r16		    ;\
		    adc	    r20, r17		    ; > r21:r20:r19 = L + dL
		    adc	    r21, r18		    ;/
		    brcc	MLP_ESTORE

;L reached top limit:
		    ldi	    r19, 255		    ;\
		    ldi	    r20, 255		    ; > L = Lmax
		    ldi	    r21, 255		    ;/
		    ldi	    r16, 2			    ; now decay
		    rjmp	MLP_ESTOREP

MLP_EDECAY:
            sub	    r19, r16		    ;\
		    sbc	    r20, r17		    ; > r21:r20:r19 = L - dL
		    sbc	    r21, r18		    ;/		
			brcs	MLP_BOTTOM 			; Exit if we went past bottom level
			lds 	r22, SUSTAINLEVEL
			cp		r22, r21				
			brlo 	MLP_ESTORE			; Keep going if we haven't hit sustain level
			ldi		r16, 3			; now sustain
			sts		ENVPHASE, r16		; store phase
MLP_ESUSTAIN:
			clr		r19			; correct sustain level after decay is done
			clr		r20
			mov		r21, r22
			rjmp	MLP_ESTORE
			
MLP_ERELEASE:
            sub	    r19, r16		    ;\
		    sbc	    r20, r17		    ; > r21:r20:r19 = L - dL
		    sbc	    r21, r18		    ;/
		    brcc	MLP_ESTORE

;L reached bottom limit:
MLP_BOTTOM:
		    ldi	    r19, 0			    ;\
		    ldi	    r20, 0			    ; > L = 0
		    ldi	    r21, 0			    ;/
		    ldi	    r16, 0			    ; stop

			sts		OSC_GATE, r16		; Turn off oscillators (Store 0 in OSC_GATE)

MLP_ESTOREP:
            sts	ENVPHASE, r16		    ; store phase

MLP_ESTORE:
            sts	    ENV_FRAC_L, r19		;\
		    sts	    ENV_FRAC_H, r20		; > store L
		    sts	    ENV_INTEGR, r21		;/

MLP_EEXIT:

			;----
            ;ENV 2 (VCF):
            ;----
;check envelope phase:
		    lds	    r17, ENVPHASE2
		    lds	    r16, KNOB_DCF_ATTACK

MLP_PHASE2:  
			cpi	    r17, 1
		    breq    MLP_ENVAR2		    ; when "attack"
			lds		r16, DECAYTIME2
			cpi		r17, 2
			breq	MLP_ENVAR2			; when "decay"
		    lds	    r16, RELEASETIME2
		    cpi	    r17, 4
		    breq	MLP_ENVAR2		    ; when "release"
			lds		r22, SUSTAINLEVEL2
			cpi		r17, 3				; when sustain
			breq	MLP_ESUSTAIN2
		    rjmp	MLP_EEXIT2		    ; when "stop" or "sustain"

;calculate dL:

MLP_ENVAR2:
            rcall	ADCTORATE           ; r19:r18:r17:r16 = rate of rise/fall
		    lds	    r22, DELTAT_L		;\
		    lds	    r23, DELTAT_H		;/ r23:r22 = dT
		    rcall	MUL32X16		    ; r18:r17:r16 = dL

;add/subtract dL to/from L:
		    lds	    r19, ENV_FRAC_L2		;\
		    lds	    r20, ENV_FRAC_H2		; > r21:r20:r19 = L
    		lds	    r21, ENV_INTEGr2		;/
		    lds	    r22, ENVPHASE2
		    cpi	    r22, 4
		    breq    MLP_ERELEASE2

MLP_EATTACK2:
			cpi	    r22, 2				
		    breq    MLP_EDECAY2			
		    add	    r19, r16		    ;\
		    adc	    r20, r17		    ; > r21:r20:r19 = L + dL
		    adc	    r21, r18		    ;/
		    brcc	MLP_ESTORE2

;L reached top limit:
		    ldi	    r19, 255		    ;\
		    ldi	    r20, 255		    ; > L = Lmax
		    ldi	    r21, 255		    ;/
		    ldi	    r16, 2			    ; now decay
		    rjmp	MLP_ESTOREP2

MLP_EDECAY2:
            sub	    r19, r16		    ;\
		    sbc	    r20, r17		    ; > r21:r20:r19 = L - dL
		    sbc	    r21, r18		    ;/		
			brcs	MLP_BOTTOM2 			; Exit if we went past bottom level
			lds 	r22, SUSTAINLEVEL2
			cp		r22, r21				
			brlo 	MLP_ESTORE2			; Keep going if we haven't hit sustain level
			ldi		r16, 3			; now sustain
			sts		ENVPHASE2, r16		; store phase
MLP_ESUSTAIN2:
			clr		r19			; correct sustain level after decay is done
			clr		r20
			mov		r21, r22
			rjmp	MLP_ESTORE2
			
MLP_ERELEASE2:
            sub	    r19, r16		    ;\
		    sbc	    r20, r17		    ; > r21:r20:r19 = L - dL
		    sbc	    r21, r18		    ;/
		    brcc	MLP_ESTORE2

;L reached bottom limit:
MLP_BOTTOM2:
		    ldi	    r19, 0			    ;\
		    ldi	    r20, 0			    ; > L = 0
		    ldi	    r21, 0			    ;/
		    ldi	    r16, 0			    ; stop
			sts		VCF_STATUS, r16		; Flag VCF as off when we hit the bottom limit. 

MLP_ESTOREP2:
            sts	ENVPHASE2, r16		    ; store phase

MLP_ESTORE2:
            sts	    ENV_FRAC_L2, r19		;\
		    sts	    ENV_FRAC_H2, r20		; > store L
		    sts	    ENV_INTEGR2, r21		;/

			; End of Envelope 2

MLP_EEXIT2: