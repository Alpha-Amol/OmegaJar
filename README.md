# OmegaJar
An Internet Connected Smart Jar

For the demo purpose i placed the ordinary jar on the Acrylic Loadcell platform then switched on the NodeMCU device after the device boots i placed a 200 gm weight inside the jar just before it connects to the Wi-Fi after the AP mode.

Then afterwards it prints the weight on Serial monitor and if the weight decreases than the threshold set (100 gm) then it orders the product using DRS.

Now so when i removed the weight for the demo purpose the NodeMCU gets about 2 to 3gm weight which is below the threshold so it orders the product using the DRS API.
