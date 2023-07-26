# How to update the colour

1. Install the ch341 drivers for your system
2. Clone this repository, and open the file "newgalaxyjar.ino" in arduino
3. Find the line **"int h = 1"**, and change that value to any number less than 8.
4. All pre-set colour patterns can be found in the **"WriteLED()"** function. (feel free to write your own within the switch statement haha)
5. Disconnect the power cables from the arduino (on the pins "vin" and "gnd" with red and black wires) and connect your arduino to your computer and upload the code to your arduino (make sure to select the right board type)
(If the code won't upload, try disconnecting the other wires from the arduino, but remember where they go back in)

# How to update wifi

1. Follow the same setup steps in the previous set of instructions
2. Find the lines near the top of the file with the **"const char * SSID[]"** and **"const char * PASS[]"**
3. Add the new wifi network and password as the first element in both arrays.
4. Same steps as earlier from here (disconnect power and upload code)