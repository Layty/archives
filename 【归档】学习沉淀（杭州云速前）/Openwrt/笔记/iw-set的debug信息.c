-bash-3.2# ./iw.dat --debug reg set CN
-- Debug: Sent Message:
--------------------------   BEGIN NETLINK MESSAGE ---------------------------
  [HEADER] 16 octets
    .nlmsg_len = 28
    .nlmsg_type = 19 <0x13>
    .nlmsg_flags = 5 <REQUEST,ACK>
    .nlmsg_seq = 1351584717
    .nlmsg_pid = 4549
  [PAYLOAD] 12 octets
    1b 00 00 00 07 00 21 00 43 4e 00 00                   ......!.CN..
---------------------------  END NETLINK MESSAGE   ---------------------------
-- Debug: Received Message:
--------------------------   BEGIN NETLINK MESSAGE ---------------------------
  [HEADER] 16 octets
    .nlmsg_len = 36
    .nlmsg_type = 2 <ERROR>
    .nlmsg_flags = 0 <>
    .nlmsg_seq = 1351584717
    .nlmsg_pid = 4549
  [ERRORMSG] 20 octets
    .error = 0 "Success"
  [ORIGINAL MESSAGE] 16 octets
    .nlmsg_len = 16
    .nlmsg_type = 19 <0x13>
    .nlmsg_flags = 5 <REQUEST,ACK>
    .nlmsg_seq = 1351584717
    .nlmsg_pid = 4549
---------------------------  END NETLINK MESSAGE   ---------------------------