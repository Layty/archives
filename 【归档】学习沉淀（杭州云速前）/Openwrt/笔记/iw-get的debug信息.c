-bash-3.2# ./iw.dat --debug reg get
-- Debug: Sent Message:
--------------------------   BEGIN NETLINK MESSAGE ---------------------------
  [HEADER] 16 octets
    .nlmsg_len = 20
    .nlmsg_type = 19 <0x13>
    .nlmsg_flags = 5 <REQUEST,ACK>
    .nlmsg_seq = 1351584450
    .nlmsg_pid = 4513
  [PAYLOAD] 4 octets
    1f 00 00 00                                           ....
---------------------------  END NETLINK MESSAGE   ---------------------------
-- Debug: Received Message:
--------------------------   BEGIN NETLINK MESSAGE ---------------------------
  [HEADER] 16 octets
    .nlmsg_len = 292
    .nlmsg_type = 19 <0x13>
    .nlmsg_flags = 0 <>
    .nlmsg_seq = 1351584450
    .nlmsg_pid = 4513
  [PAYLOAD] 276 octets
    1f 01 00 00 07 00 21 00 30 30 00 00 08 01 22 00 34 00 ......!.00....".4.
    00 00 08 00 01 00 00 00 00 00 08 00 02 00 d0 a6 24 00 ................$.
    08 00 03 00 40 b8 25 00 08 00 04 00 40 9c 00 00 08 00 ....@.%.....@.....
    05 00 58 02 00 00 08 00 06 00 d0 07 00 00 34 00 01 00 ..X...........4...
    08 00 01 00 80 01 00 00 08 00 02 00 a8 7d 25 00 08 00 .............}%...
    03 00 50 df 25 00 08 00 04 00 20 4e 00 00 08 00 05 00 ..P.%..... N......
    58 02 00 00 08 00 06 00 d0 07 00 00 34 00 02 00 08 00 X...........4.....
    01 00 81 01 00 00 08 00 02 00 10 c0 25 00 08 00 03 00 ............%.....
    30 0e 26 00 08 00 04 00 20 4e 00 00 08 00 05 00 58 02 0.&..... N......X.
    00 00 08 00 06 00 d0 07 00 00 34 00 03 00 08 00 01 00 ..........4.......
    80 01 00 00 08 00 02 00 50 e3 4e 00 08 00 03 00 d0 1b ........P.N.......
    50 00 08 00 04 00 40 9c 00 00 08 00 05 00 58 02 00 00 P.....@.......X...
    08 00 06 00 d0 07 00 00 34 00 04 00 08 00 01 00 80 01 ........4.........
    00 00 08 00 02 00 58 82 57 00 08 00 03 00 f8 08 59 00 ......X.W.......Y.
    08 00 04 00 40 9c 00 00 08 00 05 00 58 02 00 00 08 00 ....@.......X.....
    06 00 d0 07 00 00                                     ......
---------------------------  END NETLINK MESSAGE   ---------------------------
country 00:
        (2402 - 2472 @ 40), (6, 20)
        (2457 - 2482 @ 20), (6, 20), PASSIVE-SCAN, NO-IBSS
        (2474 - 2494 @ 20), (6, 20), NO-OFDM, PASSIVE-SCAN, NO-IBSS
        (5170 - 5250 @ 40), (6, 20), PASSIVE-SCAN, NO-IBSS
        (5735 - 5835 @ 40), (6, 20), PASSIVE-SCAN, NO-IBSS
-- Debug: Received Message:
--------------------------   BEGIN NETLINK MESSAGE ---------------------------
  [HEADER] 16 octets
    .nlmsg_len = 36
    .nlmsg_type = 2 <ERROR>
    .nlmsg_flags = 0 <>
    .nlmsg_seq = 1351584450
    .nlmsg_pid = 4513
  [ERRORMSG] 20 octets
    .error = 0 "Success"
  [ORIGINAL MESSAGE] 16 octets
    .nlmsg_len = 16
    .nlmsg_type = 19 <0x13>
    .nlmsg_flags = 5 <REQUEST,ACK>
    .nlmsg_seq = 1351584450
    .nlmsg_pid = 4513
---------------------------  END NETLINK MESSAGE   ---------------------------