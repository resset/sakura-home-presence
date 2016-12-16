# sakura-home-presence
PIR events logger based on Renesas GR-SAKURA board

## About

This project incorporates two cheap PIR modules based on BISS0001 chip and
Renesas GR-SAKURA Arduino-like board with RX63N (R5F563NBDDFP) MCU.

* https://learn.adafruit.com/pir-passive-infrared-proximity-motion-sensor/overview
* http://gadget.renesas.com/en/product/sakura.html

Actually, I use GR-SAKURA-FULL flavor since logger sends data via ethernet.

Idea is simple. Each of PIR modules can trigger an event after which data are
being send to the server. There is a distinction of the event source in value of
HTTP GET variable. Everything basically works in a big loop. Yeah, I wasted
~100 MHz core just for that. The board was gathering dust anyway. Now I can
gather data of home presence... and check if the cat is still alive.

Oh, and it uses this strange web compiler.
