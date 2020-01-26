Åí¹¤

¸üÐÂfw

Attached two firmware 

1. fw-1013-maxrfgain.zip 
Use lower RF again if ANI level is high (>= 7). Expected sensitivity decrease about 3dB. 

2. fw-1013-maxrfgain-xlna(-V2).zip 
Use lower RF again if ANI level is high (>= 6). Expected sensitivity decrease about 3dB. 
Turn off xLNA if ANI level is higher (>= 8). Expected sensitivity decrease about 6-10dB. 

Please use correct BDF bin file, enable xLNA in BDF.




Updated firmware fw-1013-maxrfgain-xlna-v2.zip 
Use lower RF again if ANI level is high (>= 6). Expected sensitivity decrease about 3dB. 
Turn off xLNA if ANI level is higher (>= 8). Expected sensitivity decrease about 6-10dB. 

Usage: 
Replace firmware in /lib/firmware/IPQ4019/hw.1/ 
Run the following commands 
wifi reload 
iwpriv wifi0 dl_modoff 21 
iwpriv wifi0 dl_loglevel 1 

You should see log like below on console 
[ 1572.272638] [wifi0] FWLOG: [734358] ANI arg1 1 arg2 7 arg3 32 
