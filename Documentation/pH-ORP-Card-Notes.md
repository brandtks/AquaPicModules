### Notes
 * The load resister on the output of the isolation power supply can and probably will be left off. Its there just in case stability is an issue.
 * Thinking about adding a mosfet before the isolation power supply. That way if one of the channels is disabled I can turn off all power going to that channel, thus reducing current draw.

### TODO list
 * Add EEPROM emulation in flash to persist each channel's low pass filter factor value and enable state
