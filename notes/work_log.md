# Work Log Procedure:

- Use this page to describe the nitty gritty of what has been done towards a project on a specific date.  

- Always use the date for each entry and tag your initials (YYYY-MM-DD AF).  

- Always put a new entry at the top of the page so you can see the newest work first.  

- If anything important occurs, note it in the 'Project Updates' in a more concise way than you would in this log.  

- Do not delete entries, this is meant to serve as a rudimentary version control and archive.  

- Leave the Procedure at the top of the page.  

## 2026-01-14

Added a screw terminal for the batteries. I decided that I was just going to use WAGO's and put two LiFePo4 batteries in parallel to get enough juice. I put a switch inline with the +V and the MCU. I also made a calibration script to help find the scale ratio by putting a known weight on each load cell. Made the readings be about every 10s which is estimated to have about a 30 day lifespan. So this should be fine with the frequency that Sol will be check in on it. Set the RTC to be correct and got the deep sleep working.

## 2026-01-13

First day revisiting the project after a long time. Previously, the deployment used INA114 instrument amplifiers and I was never able to get super stable readings from them. When we were at the electronics store, we stumbled upon some load cell amplifier modules and picked up a few. This rendition will be using those instead.

Water had got into the pelican case because we didn't check the silicone job that had been done in it in the past. I re-siliconed the hole and it seems pretty sturdy. 

Got the basic code down and wired up three HX711's to the load cells. Everything seems to work now. Moving forward, gotta make a state machine and implement sleep correctly. I didn't wire up the power batteries or the RTC battery yet. 