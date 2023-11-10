# GalaxyJarGift

The perfect gift you could give to your special someone. A tech-y twist on a cute gift.

"You stepped into my world like a galaxy bursting in front of an astonomers telescope. Tell me, with all those speckles on your skin, did anyone tell you that you are a constellation waiting to be loved and explored"
^^ Tumblr quote I thought was kinda cute and semi-inspired me to start working on this project

# What it is / What it does

When you tap the lid of the mason jar, it lights up LEDs in the interior of the LED, as well as the lights on any other Galaxy Jar connected to the internet.

![](./img/video1.mp4)
![](./img/video2.mp4)

(if you want me to make you a set, send me an email: n.biancolin[at]mail.utoronto.ca)

# MATERIALS NEEDED:

## Tech Stuffs (from amazon):

- 2 ESP8266 Development Boards (I used these ones from KeeYees on Amazon, but any board like it will do: https://www.amazon.ca/dp/B07S5Z3VYZ?psc=1&ref=ppx_yo2ov_dt_b_product_details )
- WS2182B light strip (Mine was 1m with 60 leds/m, but this part is up to you)
- 4 x 2xAAA battery holders
- 8 x AAA batteries
- 2 x 220 ohm resistor, 4 x 10k ohm resistor
- A bunch of misc wires and stuff
- Soldering iron + solder & flux


## Crafty Stuff
*(You should know I am by no means a crafty person, and a lot of these went to waste bc it took me a few tries to get everything right)*
- 2 small-ish mason jars 
- Poly-fill "stuffing" (think like stuffed-animal stuffing)
- Rit DyeMore synthetic dye, graphite colour
- 2 toilet paper rolls

## Other stuff I used that I had lying around
- A bunch of wires
- Heat-Shrink tubing

# How I made it

## Step 1 - Prep Jars

I dyed the poly-fill to be as dark as I could so that the lights would look like stars in the sky, and I did that by boiling some water, putting the dye in there, and soaking the dye.\\
![Dying the poly-fill](./img/step1-1.jpg) 

I also cut the led strips and wrapped them around empty toilet paper roles, using electrical tape to fasten the strips to the roll. \\
![LED strips taped on](./img/step1-2.jpg)

## Step 2 - Write code

This step was suprisingly difficult. I dealt with lots of hardware issues (cheap components giving up midway through) but once all of those issues were sorted it was relatively straight-forward. My friend Nathan (Spoink2022) helped me set up a node.js socket.io server to communicate between all the arduinos. This way, whenever there is an update (tap), the server just sends out a ping to all the arduinos to turn on, and once the signal turns off it sends another ping. You can see all of this code in the "NewGalaxyJar.ino" file. \\
![Starting to write code with the LEDs](./img/step2-1.jpg)

## Step 3 - Assemble

Put it all together! This involved soldering all the wires to pin headers (so that Im not soldering directly on the arduino bc I'd like to use them again), and actually assembling the jars. They ended up being quite a tight fit with the batteries and the arduino and the wires that went with it, but I managed to fit it all in, and the result was amazing.\\
![Assembling everything](./img/step3-1.jpg)


This project was really fun, it was a great way to get more experience dealing with hardware, and the end result ended up being the perfect gift.



\*cue Ted from HIMYM's Voice\*: *"It really wasn't that hard. 
All I had to do was buy some microcontrollers and some other parts off of amazon. Realize that you need to buy other LEDs. Go and buy other LEDs. Learn how Socket.IO works, and try and build a server. Give up and beg your friend to set it up for you. (in hindsight it lowkey was not that complex and and I could have probably figure it out, but I am so grateful for Nathan's help). Realize that the wifi modules don't work. Buy new microcontrollers with wifi modules built in. Try 5 different libraries for connecting the controllers to socket.io server with no luck. Finally get it to work on a whim. Realize that the batteries you bought have too high of a voltage and are going to fry the controllers. Find a way to make the right voltage frantically a couple nights before. Solder everything together, Re-solder half of the connections because they were soldered incorrectly. Realize that capacitive touch sensors don't work when powered solely by batteries bc there is no ground, and frantically try and find a way to make it work. Finally get everything working after a month and a bit of every single hurdle that could ever*