<map version="0.9.0">
<!-- To view this file, download free mind mapping software FreeMind from http://freemind.sourceforge.net -->
<node CREATED="1318945152110" ID="ID_824451930" MODIFIED="1318957114291" TEXT="&#x6536;&#x5305;">
<font NAME="SansSerif" SIZE="14"/>
<node CREATED="1318945202386" ID="ID_1481011819" MODIFIED="1318957114291" POSITION="right" TEXT="">
<font NAME="SansSerif" SIZE="14"/>
</node>
<node COLOR="#338800" CREATED="1318945205116" ID="ID_1263219094" MODIFIED="1318957114291" POSITION="left">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      &#20013;&#26029;&#19978;&#21322;&#37096;&#65288;ath_isr-&gt;do_ath_isr-&gt;ath_isr_generic&#65289;
    </p>
  </body>
</html>
</richcontent>
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#338800" CREATED="1318945310804" ID="ID_506376501" MODIFIED="1318957114291" TEXT="ath_intr&#xff08;sc_ops-&gt;isr&#xff09;">
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#338800" CREATED="1318945917659" ID="ID_213525764" MODIFIED="1318957114291" TEXT="ath_hal_intrpend">
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1318945932892" ID="ID_413161856" MODIFIED="1318957114291" TEXT="&#x5982;&#x679c;&#x4e2d;&#x65ad;&#x65f6;&#x5171;&#x4eab;&#x7684;&#xff0c;&#x5224;&#x65ad;&#x662f;&#x5426;&#x4e3a;&#x81ea;&#x5df1;&#x7684;&#x4e2d;&#x65ad;">
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
</node>
</node>
<node COLOR="#338800" CREATED="1318946317675" ID="ID_1684043759" MODIFIED="1318957114291" TEXT="sc-&gt;sc_invalid">
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1318946321504" ID="ID_1162645984" MODIFIED="1318957114291" TEXT="&#x5982;&#x679c;&#x786c;&#x4ef6;&#x6ca1;&#x6709;&#x8d77;&#x6765;&#x6216;&#x4e00;&#x4e9b;&#x5176;&#x4ed6;&#x539f;&#x56e0;&#x65e0;&#x6cd5;&#x4f7f;&#x7528;&#x4e2d;&#x65ad;">
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
</node>
</node>
<node COLOR="#338800" CREATED="1318953240708" ID="ID_650078660" MODIFIED="1318957114291" TEXT="ath_hal_getisr&#xff08;ar9300GetPendingInterrupts&#xff09;">
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1318953316989" ID="ID_1452648865" MODIFIED="1318957114291" TEXT="&#x83b7;&#x53d6;&#x4e2d;&#x65ad;&#x72b6;&#x6001;&#xff0c;&#x5224;&#x5b9a;&#x662f;&#x5426;&#x4e3a;&#x6536;&#x5305;&#x4e2d;&#x65ad;">
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
</node>
</node>
<node COLOR="#338800" CREATED="1318953426587" ID="ID_1856339263" MODIFIED="1318957126592">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      ath_common_intr-&gt;ath_rx_edma_intr
    </p>
    <p>
      -&gt;ath_rx_intr
    </p>
  </body>
</html>
</richcontent>
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#ff0000" CREATED="1318954479742" ID="ID_1840852733" MODIFIED="1318957114291" TEXT="do">
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1318956692421" ID="ID_1077865077" MODIFIED="1318957114291" TEXT="&#x5f53;rxifio&#x786c;&#x4ef6;&#x961f;&#x5217;&#x4e0d;&#x4e3a;&#x7a7a;&#x7684;&#x65f6;&#x5019;&#xff0c;&#x4e0d;&#x505c;&#x7684;&#x5904;&#x7406;&#x6570;&#x636e;&#x5305;">
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
</node>
</node>
<node COLOR="#338800" CREATED="1318953431603" ID="ID_193244268" MODIFIED="1318957124720" TEXT="wbuf = rxedma-&gt;rxfifo[rxedma-&gt;rxfifoheadindex]">
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1318954535613" ID="ID_1594970339" MODIFIED="1318957114291">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      rxedma&#26159;&#26377;sc-&gt;sc_rxedma[HP|LP]&#21462;&#20986;&#65292;rxfifo&#26159;wbuf&#30340;&#32467;&#26500;&#30340;&#25968;&#32452;&#65292;
    </p>
    <p>
      &#25351;&#30340;&#26159;&#22312;&#30828;&#20214;&#25910;&#21253;&#38431;&#21015;&#12290;
    </p>
  </body>
</html>
</richcontent>
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
</node>
</node>
<node COLOR="#338800" CREATED="1318954168197" ID="ID_1484748988" MODIFIED="1318957114291" TEXT="bf = ATH_GET_RX_CONTEXT_BUF(wbuf)">
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1318954737805" ID="ID_1386132872" MODIFIED="1318957114276">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      wbuf&#20013;&#30340;data&#26159;&#25351;&#21521;&#25910;&#21253;&#29366;&#24577;&#21644;&#25910;&#21253;&#25968;&#25454;&#19968;&#36215;&#30340;&#37027;&#22359;&#20869;&#23384;&#30340;&#25351;&#38024;&#65292;
    </p>
    <p>
      &#23427;&#26412;&#36523;&#36824;&#23384;&#30528;&#36319;&#22810;&#30340;&#25968;&#25454;&#65292;&#22914;&#23427;&#26377;&#19968;&#20010;&#25351;&#38024;&#21464;&#37327;&#25351;&#21521;wbuf&#25152;&#23646;&#30340;
    </p>
    <p>
      bf&#32467;&#26500;&#65292;bf&#32467;&#26500;&#20013;&#30340;bf_mpdu&#21644;&#36825;&#20010;rxfifo[xx]&#22320;&#22336;&#24212;&#35813;&#19968;&#33268;&#12290;
    </p>
  </body>
</html>
</richcontent>
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
</node>
</node>
<node COLOR="#338800" CREATED="1318954207502" ID="ID_432636816" MODIFIED="1318957114276" STYLE="fork">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      OS_SYNC_SINGLE (bf-&gt;bf_buf_addr[0], sc-&gt;sc_rxstatuslen, BUS_DMA_FROMDEVICE)
    </p>
  </body>
</html>
</richcontent>
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1318955072987" ID="ID_1745549794" MODIFIED="1318957114276" STYLE="fork">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      &#36890;&#36807;bf&#25214;&#21040;wbuf-&gt;data&#30340;&#29289;&#29702;&#22320;&#22336;&#65292;&#24182;&#19988;&#36890;&#36807;dma&#21516;&#27493;&#21040;cpu&#36825;&#37324;&#65292;
    </p>
    <p>
      &#21516;&#27493;&#30340;&#25968;&#25454;&#21482;&#26377;sc_rxstatuslen&#22823;&#23567;&#65292;&#23601;&#26159;&#35828;&#21482;&#21516;&#27493;&#20102;&#29366;&#24577;&#38271;&#24230;&#65292;&#21363;
    </p>
    <p>
      &#20026;&#25910;&#21253;&#25551;&#36848;&#31526;&#30340;&#22823;&#23567;&#12290;
    </p>
  </body>
</html>
</richcontent>
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
</node>
</node>
<node COLOR="#338800" CREATED="1318954324003" ID="ID_650262540" MODIFIED="1318957114260" TEXT="ath_hal_rxprocdescfast&#xff08;ar9300ProcRxDescFast&#xff09;">
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1318955395752" ID="ID_241166586" MODIFIED="1318957114260">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      &#19978;&#19968;&#27493;&#24050;&#32463;&#36890;&#36807;dma&#21516;&#27493;&#24471;&#21040;&#20102;&#25910;&#21253;&#25551;&#36848;&#31526;&#65292;&#36825;&#20010;&#20989;&#25968;&#21017;&#26159;&#36890;&#36807;
    </p>
    <p>
      &#25910;&#21253;&#25551;&#36848;&#31526;&#26469;&#21021;&#22987;&#21270;bf-&gt;bf_desc&#36825;&#20010;&#32467;&#26500;&#12290;
    </p>
  </body>
</html>
</richcontent>
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
</node>
</node>
<node COLOR="#338800" CREATED="1318954438620" ID="ID_1169121467" MODIFIED="1318957114260" TEXT="TAILQ_INSERT_TAIL(&amp;rxedma-&gt;rxqueue, bf, bf_list);">
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1318955578382" ID="ID_1372271618" MODIFIED="1318957114260">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      &#25910;&#21253;&#25551;&#36848;&#31526;&#20449;&#24687;&#24050;&#32463;&#23384;&#21040;bf-&gt;bf_desc&#20043;&#21518;&#65292;&#19978;&#21322;&#37096;&#20998;&#30340;&#20107;&#24773;&#24050;&#32463;&#22522;&#26412;&#23436;&#25104;&#65292;
    </p>
    <p>
      &#35762;bf&#23384;&#20837;&#21040;rxedma-&gt;rxqueue&#38431;&#21015;&#20013;&#65292;&#22312;&#19979;&#21322;&#37096;&#21017;&#36890;&#36807;xedma-&gt;rxqueue&#21462;&#20986;
    </p>
    <p>
      bf&#65292;&#32487;&#32493;&#22788;&#29702;&#20854;&#20013;&#30340;&#25968;&#25454;&#12290;
    </p>
  </body>
</html>
</richcontent>
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
</node>
</node>
<node COLOR="#ff0000" CREATED="1318954506059" ID="ID_513502673" MODIFIED="1318957114260" TEXT="while">
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
</node>
<node COLOR="#338800" CREATED="1318954468674" ID="ID_1142318704" MODIFIED="1318957114260" TEXT="ath_rx_addbuffer">
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1318956158484" ID="ID_1103325004" MODIFIED="1318957114229">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      rxedma-&gt;rxfifohwsize&#20195;&#34920;&#24050;&#32463;&#20174;hw&#20256;&#21040;hal&#30340;&#25968;&#25454;&#20010;&#25968;&#65292;&#22240;&#20026;&#20256;&#21040;hal&#21518;&#65292;
    </p>
    <p>
      rxfifo&#25968;&#32452;&#30340;&#30456;&#24212;&#20803;&#32032;&#20415;&#20250;&#28165;&#31354;&#65292;&#22240;&#27492;&#36825;&#20010;&#20989;&#25968;&#30340;&#24037;&#20316;&#23601;&#26159;sc-&gt;sc_rxbuf&#20013;&#30340;
    </p>
    <p>
      bf-&gt;bf_mpdu&#36827;&#34892;dma&#21516;&#27493;&#32473;&#35774;&#22791;&#65292;&#28982;&#21518;&#32473;rxfifo&#30828;&#20214;&#38431;&#21015;&#65292;&#32487;&#32493;&#36827;&#34892;&#25910;&#21253;&#12290;
    </p>
  </body>
</html>
</richcontent>
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
</node>
</node>
<node COLOR="#111111" CREATED="1318953509696" ID="ID_981862542" MODIFIED="1318957114229">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      &#20013;&#26029;&#29366;&#24577;&#30721;&#20026;&#25910;&#21253;&#29366;&#24577;&#26102;&#65292;&#36827;&#20837;ath_rx_intr&#20013;&#65292;&#36827;&#34892;&#25910;&#21253;&#30340;&#20013;&#26029;&#19978;&#21322;&#37096;&#20998;
    </p>
    <p>
      &#30340;&#22788;&#29702;&#65292;&#29366;&#24577;&#30721;&#26377;&#20998;&#25910;&#21253;&#26159;HP&#65288;&#39640;&#20248;&#20808;&#32423;&#65289;&#21644;LP&#65288;&#20302;&#20248;&#20808;&#32423;&#65289;&#12290;
    </p>
  </body>
</html>
</richcontent>
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
</node>
</node>
</node>
<node COLOR="#111111" CREATED="1318945329530" ID="ID_1981121766" MODIFIED="1318957114198">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      &#22823;&#27010;&#22240;&#20026;&#26080;&#32447;&#39537;&#21160;&#36890;&#24120;&#19981;&#20250;&#25484;&#31649;&#22826;&#22810;&#26080;&#32447;&#32593;&#21345;&#65292;&#20063;&#21487;&#33021;&#26159;&#22240;&#20026;
    </p>
    <p>
      &#26080;&#32447;&#38656;&#35201;&#24456;&#22909;&#30340;&#23454;&#26102;&#24615;&#65292;&#20063;&#26377;&#21487;&#33021;&#26080;&#32447;&#30340;&#36895;&#29575;&#24182;&#27809;&#26377;&#36798;&#21040;&#19968;&#23450;
    </p>
    <p>
      &#30340;&#39640;&#36895;&#65292;&#25152;&#26377;&#20174;&#39537;&#21160;&#21487;&#20197;&#30475;&#20986;&#37319;&#21462;&#30340;&#26159;&#20013;&#26029;&#25910;&#21253;&#65292;&#32780;&#38750;NAPI&#12290;
    </p>
  </body>
</html>
</richcontent>
<edge COLOR="#111111" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="14"/>
</node>
</node>
</node>
</map>
