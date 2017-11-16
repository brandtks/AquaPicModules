### Notes
 * Switched to PIC32MM0064GPM028 because there will be quite a bit of 16 and 32 bit math including division. The PIC16F is terrible at this especially with the free compiler.
 * Using bi-directional level shifts for the TX/RX lines, because it seems to be the cheapest option. 

### Voltage divider protection:
Vout = (Vin * 10) / (24.99 - RVx)
The voltage divider servers two purposes: 
 * First it limits current and 
 * Second it can be used to increase the measured voltage because most sensor devices don't output 5Vdc at full range. This is still limited. With RVx at 0R the minimum input voltage to reach full scale is 3.07V.


