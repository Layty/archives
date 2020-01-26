ah->ah_flags
/* ah_flags */
#define AH_USE_EEPROM   0x1
#define AH_UNPLUGGED    0x2 /* The card has been physically removed. */
#define AH_FASTCC       0x4

sc->sc_flags
#define SC_OP_INVALID                BIT(0)
#define SC_OP_BEACONS                BIT(1)
#define SC_OP_RXAGGR                 BIT(2)
#define SC_OP_TXAGGR                 BIT(3)
#define SC_OP_OFFCHANNEL             BIT(4)
#define SC_OP_PREAMBLE_SHORT         BIT(5)
#define SC_OP_PROTECT_ENABLE         BIT(6)
#define SC_OP_RXFLUSH                BIT(7)
#define SC_OP_LED_ASSOCIATED         BIT(8)
#define SC_OP_LED_ON                 BIT(9)
#define SC_OP_TSF_RESET              BIT(11)
#define SC_OP_BT_PRIORITY_DETECTED   BIT(12)
#define SC_OP_BT_SCAN		     BIT(13)
#define SC_OP_ANI_RUN		     BIT(14)
#define SC_OP_PRIM_STA_VIF	     BIT(15)