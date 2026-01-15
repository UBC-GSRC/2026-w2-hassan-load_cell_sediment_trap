# Work Log Procedure:

- Use this page to describe the nitty gritty of what has been done towards a project on a specific date.  

- Always use the date for each entry and tag your initials (YYYY-MM-DD AF).  

- Always put a new entry at the top of the page so you can see the newest work first.  

- If anything important occurs, note it in the 'Project Updates' in a more concise way than you would in this log.  

- Do not delete entries, this is meant to serve as a rudimentary version control and archive.

- Leave the Procedure at the top of the page.  

## 2026-01-15

Figured out a lot today. I first started with trying to test everything in the water. I quickly realized the conversion factor to convert the load cell readings to kg varies from being in the air and underwater. That was intuitive but I wish it wasn't the case. I was able to calibrate each of the load cells underwater using a 1/2" threaded rod with like 50 washers around it which ended up being about 3.55 kg. I used this on each of the load cells individually to get a conversion factor for each one. When I then put in the sediment trap, tared it, and put in the test weight, the three load cells summed to be 3.55 no matter where I put the test weight. 

From there, I tried using the blue gravel by the Toy Flume. I put in ~11kg of gravel and the cells read 8 kg. This is because a straight conversion factor does not account for the buoyancy of the material inside it. Now this is where the accuracy gets more complicated. If there's a variety of rocks coming into the trap, it's possible that they all have different densities. So no matter what there's going to be some model fitting going on here.

There will be some good data to fit to though. Because we will start with reading zero kg. Then it will accumulate and show the waves of sediment that get caught every 30 seconds or so. After a few weeks, Sol will take all of the sediment out of the trap and weigh it. He'll also do a grain size distribution of the sample. It would be great to also do get an average density or a density distribution. He also has water temperature readings which is necessary. These data will be enough to get almost instantaneous sediment transport readings. 

Marwan is happy with just a proportional calibration to the start and end mass of the trap.

## 2026-01-14

Added a screw terminal for the batteries. I decided that I was just going to use WAGO's and put two LiFePo4 batteries in parallel to get enough juice. I put a switch inline with the +V and the MCU. I also made a calibration script to help find the scale ratio by putting a known weight on each load cell. Made the readings be about every 10s which is estimated to have about a 30 day lifespan. So this should be fine with the frequency that Sol will be check in on it. Set the RTC to be correct and got the deep sleep working.

## 2026-01-13

First day revisiting the project after a long time. Previously, the deployment used INA114 instrument amplifiers and I was never able to get super stable readings from them. When we were at the electronics store, we stumbled upon some load cell amplifier modules and picked up a few. This rendition will be using those instead.

Water had got into the pelican case because we didn't check the silicone job that had been done in it in the past. I re-siliconed the hole and it seems pretty sturdy. 

Got the basic code down and wired up three HX711's to the load cells. Everything seems to work now. Moving forward, gotta make a state machine and implement sleep correctly. I didn't wire up the power batteries or the RTC battery yet. 