---------------------------------------------------------------------------------------------------
                     _     _ _         _        _           _      
                    | |   | (_)       | |      (_)         | |     
 _ __ ___   ___  ___| |__ | |_ _ __   | |_ _ __ _  ___   __| | ___ 
| '_ ` _ \ / _ \/ _ \ '_ \| | | '_ \  | __| '__| |/ _ \ / _` |/ _ \
| | | | | |  __/  __/ |_) | | | |_) | | |_| |  | | (_) | (_| |  __/
|_| |_| |_|\___|\___|_.__/|_|_| .__/   \__|_|  |_|\___/ \__,_|\___|
                              | |                                  
                              |_|                                                                                   
---------------------------------------------------------------------------------------------------

Change log

V1.10 2016.11.12 
- Updated MIDI code to exit interrupt immediately if MIDI command is unrecognized
- Increased front panel switch scanning rate (10X faster)
- Reduced pitchbend to ± 3 semitones
- Altered filter decay envelope to ignore SUSTAIN switch so that filter sweeps continue when key is released
- Removed digital pre-filtering code (previously used on OSC A&B)
- Removed digital enveloping of OSC A & B
- Gate oscillators when amplitude envelope hits zero (prevents audio bleed through)
- Changed filter cutoff curve (use table from MeeBlip SE)
- Scale filter cutoff knob to cover 25%-100% of knob range
- Scale filter resonance knob to cover 25%-100% of knob range
- Added bandlimited square wave tables (pulse waves were previously generated with offset sawtooth ramps)
- Raw calculated waveforms are now used in the lower 4 octaves, then switch to bandlimited wavetables. 
-Flipped waveforms to match phase and avoid a pop when crossing the cut-over point
 
V1.01 2016.10.24 
- Ensure MIDI channel settings are saved correctly when powered off for Channel 1-8 (implement MAXMIDI variable)
- Allow MIDI CC control of GLIDE knob to select waves when in wavetable mode (CC 61 now controls GLIDE in wavetable mode)
- Updated MIDI code to exit interrupt immediately if MIDI command is unrecognized
- Increased front panel switch scanning rate (10X faster)

V1.00 2016.10.19 
- Initial release

---------------------------------------------------------------------------------------------------

  MeeBlip hardware is released under a Creative Commons Attribution-ShareAlike 4.0 International 
	(CC-BY-SA 4.0) license. You are free to share and transform this work, even for commercial
	purposes, providing you:

	1. Give appropriate credit, provide a link to the license, and indicate if changes were made. 
	   You can say something such as "This project is based upon the MeeBlip anode synthesizer. 
	   For more information, visit meeblip.com or download source code and design files at 
	   https://github.com/MeeBlip"

	2. If you remix, transform, or build upon the material, you must distribute your contributions
	   under the same license as the original. That means making source code, design files and PC 
	   board layout files available so someone can build and modify their own version of the project. 

	3. The MeeBlip name is our intellectual property. You are not allowed to release commercial 
	   devices based on our designs using the MeeBlip brand name. 
	
	Here's are links to the Creative Commons license, for the design:
	http://creativecommons.org/licenses/by-sa/4.0/
	http://creativecommons.org/licenses/by-sa/4.0/legalcode

	MeeBlip source code and documentation is released under a GPLv3 license. 

  A copy is available in the MeeBlip repository at:
	https://github.com/MeeBlip/meeblip-anode/blob/master/LICENSE


	MeeBlip Contributors

	Jarek Ziembicki	- Created the original AVRsynth, upon which this project is based.
	Laurie Biddulph	- Worked with Jarek to translate his comments into English, ported to Atmega16
	Daniel Kruszyna	- Extended AVRsynth (several of his ideas are incorporated in MeeBlip)
 	Julian Schmidt	- Original Meeblip digital filter algorithm
	Axel Werner		- Code optimization, bug fixes and new bandlimited waveforms 
	James Grahame 	- Meeblip hardware and firmware development
