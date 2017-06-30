# ns2_wcv
A WCV(Wireless Chargeable Vehicle) extension for Network Simulator 2

A DoC (Denial of Charge) attack model research

## Overview

Written as a DiffApp. Put themm in diffusion3/apps/ping. And copy Makefile to root. Then `make`

A research project on Denial of Charge attach, in Wireless Rechargeable Sensor Network.

## FEATURES:

1. WCV runs a FSM, collecting request, make plans, heading destination, charging sensors.
2. Sensors publish charging request, either benign, or malicious. 
3. Malicious sensors track neighbors, determining weight coefficient, deciding fake request.
4. WCV exports a simple interface : run
5. Sensors exports also simple interface : publish [auto|fake energy float]
