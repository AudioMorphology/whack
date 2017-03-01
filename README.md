# whack
Arduino Nano-based Eurorack drum sample player

This is a very simple dum sample player based on the Drum8 AVR code by Jan Ostman of DSP Synthesisers in Sweden, and shared under a GNU General Public License.

Jan's original code used 8-Bit samples of 808-Style drums. I have used these as an initial starting point, though the intention is to replace them with something a little more original.

The hardware is intended to be extremely simple to build, making it ideally suited to a home-built project. The Arduino Nano is an ideal component to base the design around, as it includes 12v tolerant on-board regulators, that generate 5v rails, which can be used by the prepheral devices, greatly simplifying the circuitry.

