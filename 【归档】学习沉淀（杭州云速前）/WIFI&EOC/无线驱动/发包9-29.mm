<map version="0.9.0">
<!-- To view this file, download free mind mapping software FreeMind from http://freemind.sourceforge.net -->
<node CREATED="1316964075323" ID="ID_594223676" MODIFIED="1316964519968" TEXT="&#x53d1;&#x5305;">
<font NAME="SansSerif" SIZE="20"/>
<node COLOR="#669900" CREATED="1316964611073" ID="ID_1956109167" MODIFIED="1317007958551" POSITION="right" TEXT="dev-&gt;hart_start_xmit">
<edge COLOR="#00b439" WIDTH="2"/>
<font NAME="SansSerif" SIZE="12"/>
<node COLOR="#669900" CREATED="1316965140716" ID="ID_1435860875" MODIFIED="1317007956957" TEXT="osif_vap_hartstart">
<edge COLOR="#00b439" WIDTH="2"/>
<node COLOR="#990000" CREATED="1316965716100" ID="ID_999946403" MODIFIED="1317007388282" TEXT="osif_vap_hardstart_aponly">
<edge COLOR="#990000" WIDTH="1"/>
<icon BUILTIN="full-1"/>
<node CREATED="1317018063154" ID="ID_88252408" MODIFIED="1317018107935" TEXT="&#x901a;osif_vap_hardstart_generic&#x7684;&#x903b;&#x8f91;&#x57fa;&#x672c;&#x4e00;&#x81f4;&#xff0c;&#x53ea;&#x662f;&#x5c11;&#x4e86;IBSS&#xff0c;WDS&#x7b49;&#x4e00;&#x4e9b;&#x5224;&#x65ad;&#x64cd;&#x4f5c;&#x3002;"/>
</node>
<node COLOR="#669900" CREATED="1316966767548" ID="ID_900998422" MODIFIED="1317007955207" TEXT="osfi_vap_hardstart_generic">
<edge COLOR="#00b439" WIDTH="2"/>
<icon BUILTIN="full-2"/>
<node COLOR="#669900" CREATED="1317006059441" ID="ID_1550369374" MODIFIED="1317007367079" TEXT="comdev-&gt;flags IFF_RUNING|IFF_UP">
<icon BUILTIN="down"/>
<node CREATED="1317007082735" ID="ID_1629750189" MODIFIED="1317007105079" TEXT="&#x786e;&#x8ba4;wifi&#x662f;up&#x6216;running&#x72b6;&#x6001;"/>
</node>
<node COLOR="#669900" CREATED="1317006100191" ID="ID_1044952194" MODIFIED="1317007370641" TEXT="skb_unshart">
<icon BUILTIN="down"/>
<node CREATED="1317007142032" ID="ID_651274178" MODIFIED="1317007180454" TEXT="&#x5c06;skb&#x5305;copy&#x4e00;&#x4efd;&#xff0c;&#x91ca;&#x653e;&#x539f;&#x6765;&#x7684;skb&#x5f15;&#x7528;"/>
</node>
<node COLOR="#669900" CREATED="1317006113050" ID="ID_668059159" MODIFIED="1317007450172" TEXT="wlan_vap_send">
<node COLOR="#990000" CREATED="1317006288518" ID="ID_791319278" MODIFIED="1317007396329" TEXT="thinap&#x7684;tunnel&#x6a21;&#x5f0f;">
<edge COLOR="#990000" WIDTH="1"/>
<icon BUILTIN="full-1"/>
<node CREATED="1317007846471" ID="ID_1129552000" MODIFIED="1317088643930">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      &#27492;&#24103;&#26102;802.11&#24103;&#65292;&#20808;&#36890;&#36807;ieee80211_find_txnode&#25214;&#21040;ni&#32467;&#28857;&#20043;&#21518;&#65292;
    </p>
    <p>
      &#30452;&#25509;&#21457;&#24448;ath&#23618;&#65288;wifi&#65289;&#30340;&#21457;&#21253;&#12290;&#36825;&#37324;&#20250;&#36339;&#36807;&#21518;&#38754;&#30340;autelan&#30340;&#31169;&#26377;&#21151;&#33021;&#65292;
    </p>
    <p>
      &#22240;&#20026;&#22312;tunnel&#27169;&#24335;&#19979;ac&#36807;&#26469;&#30340;&#21253;&#25105;&#20204;&#26159;&#23436;&#20840;&#20449;&#20219;&#30340;&#12290;
    </p>
  </body>
</html></richcontent>
</node>
</node>
<node COLOR="#669900" CREATED="1317006314611" ID="ID_351706722" MODIFIED="1317007507532" TEXT="ieee80211_find_txnode">
<icon BUILTIN="full-2"/>
<icon BUILTIN="down"/>
<node CREATED="1317007513516" ID="ID_893582635" MODIFIED="1317007543579" TEXT="&#x6839;&#x636e;&#x6e90;MAC&#x5730;&#x5740;&#xff0c;&#x627e;&#x51fa;&#x53d1;&#x5305;&#x8282;&#x70b9;"/>
</node>
<node COLOR="#669900" CREATED="1317006334674" ID="ID_628445334" MODIFIED="1317007567094" TEXT="ieee80211_classify">
<icon BUILTIN="down"/>
<node CREATED="1317007551204" ID="ID_1658584315" MODIFIED="1317007704970" TEXT="&#x5c06;ip&#x5934;&#x4e2d;&#x7684;tos&#x8f6c;&#x5316;&#x6210;&#x65e0;&#x7ebf;wmm&#x7684;ac&#xff08;4&#x4e2a;&#xff09;&#x548c;tid&#xff08;&#x5f88;&#x591a;&#x4e2a;&#xff09;"/>
</node>
<node COLOR="#669900" CREATED="1317006346143" ID="ID_1649001752" MODIFIED="1317007713282" TEXT="autelan&#x79c1;&#x6709;&#x529f;&#x80fd;">
<icon BUILTIN="down"/>
<node CREATED="1317007731892" ID="ID_1128297927" MODIFIED="1317089409080">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      autelan&#31169;&#26377;&#21151;&#33021;&#22788;&#29702;&#65292;vlan_mac&#32465;&#23450;&#65292;&#29992;&#25143;&#38548;&#31163;&#65292;dhcp&#36879;&#20256;&#31561;&#27492;&#22788;&#21028;&#26029;&#65292;&#22914;&#26524;&#19981;&#25104;&#21151;
    </p>
    <p>
      &#23601;&#26089;&#28857;&#20002;&#21253;&#65292;&#20813;&#24471;&#32829;&#35823;cpu&#22788;&#29702;&#26102;&#38388;
    </p>
  </body>
</html></richcontent>
</node>
</node>
<node COLOR="#669900" CREATED="1317006367470" ID="ID_1011006824" MODIFIED="1317007945707" TEXT="&#x5230;ath&#x5c42;&#xff08;wifi&#xff09;&#x7684;&#x53d1;&#x5305;">
<arrowlink DESTINATION="ID_1685704802" ENDARROW="Default" ENDINCLINATION="489;0;" ID="Arrow_ID_652483643" STARTARROW="None" STARTINCLINATION="479;73;"/>
</node>
</node>
</node>
</node>
<node CREATED="1316965143477" ID="ID_1149706434" MODIFIED="1317009047542">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      &#21019;&#24314;vap&#25152;&#24314;&#31435;&#30340;net_device&#32467;&#26500;&#65292;osif_vap_hartstart&#26159;&#20854;&#21457;&#21253;&#30340;&#25346;&#38057;&#20989;&#25968;&#65292;
    </p>
    <p>
      &#22312;&#21457;&#21253;&#30340;&#36807;&#31243;&#38500;&#38750;&#19968;&#20123;&#29305;&#27530;&#24773;&#20917;&#65292;&#22914;&#31649;&#29702;&#24103;&#31561;&#19968;&#20123;&#30452;&#25509;&#35843;&#29992;ath&#23618;&#21457;&#21253;&#20989;&#25968;&#30340;&#65292;
    </p>
    <p>
      &#19968;&#33324;&#35828;&#37117;&#22312;&#20808;&#35843;&#29992;&#21738;&#20010;ieee802.11&#23618;&#36825;&#20010;&#21457;&#21253;&#20989;&#25968;&#65292;&#28982;&#21518;&#25509;&#30528;&#20877;&#35843;&#29992;ath&#23618;&#21457;&#21253;
    </p>
    <p>
      &#20989;&#25968;&#19979;&#21435;&#12290;
    </p>
  </body>
</html></richcontent>
</node>
</node>
<node COLOR="#00b439" CREATED="1316964611073" ID="ID_1685704802" MODIFIED="1317007945707" POSITION="right" TEXT="dev-&gt;hart_start_xmit">
<edge COLOR="#00b439" WIDTH="2"/>
<node COLOR="#00b439" CREATED="1316966611988" ID="ID_41805022" MODIFIED="1316966846375" TEXT="ath_netdev_hardstart">
<edge COLOR="#00b439" WIDTH="2"/>
<node COLOR="#990000" CREATED="1316966637010" ID="ID_790597912" MODIFIED="1317008216598" TEXT="ath_netdev_hartstart_aponly">
<edge COLOR="#990000" WIDTH="1"/>
<icon BUILTIN="full-1"/>
<node CREATED="1317018063154" ID="ID_415884642" MODIFIED="1317018128935" TEXT="&#x901a;ath_netdev_hardstart_generic&#x7684;&#x903b;&#x8f91;&#x57fa;&#x672c;&#x4e00;&#x81f4;&#xff0c;&#x53ea;&#x662f;&#x5c11;&#x4e86;IBSS&#xff0c;WDS&#x7b49;&#x4e00;&#x4e9b;&#x5224;&#x65ad;&#x64cd;&#x4f5c;&#x3002;"/>
</node>
<node COLOR="#00b439" CREATED="1316965749577" ID="ID_835449347" MODIFIED="1317018168654" TEXT="ath_netdev_hardstart_generic">
<edge COLOR="#00b439" WIDTH="2"/>
<icon BUILTIN="full-2"/>
<node COLOR="#00b439" CREATED="1316965982286" HGAP="33" ID="ID_1880133862" MODIFIED="1317009344043" TEXT="scn-&gt;sc_ops-&gt;txq_depth" VSHIFT="-15">
<edge COLOR="#00b439" WIDTH="2"/>
<icon BUILTIN="down"/>
<node CREATED="1317008584645" ID="ID_1701713474" MODIFIED="1317008754849">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      &#26597;&#30475;&#30828;&#20214;&#38431;&#21015;&#38271;&#24230;&#65292;&#26681;&#25454;skb&#30340;&#20248;&#20808;&#32423;&#65292;&#36716;&#21270;&#25104;&#30828;&#20214;&#38431;&#21015;&#32034;&#24341;&#21495;
    </p>
    <p>
      (scn-&gt;sc_ac2q[skb-&gt;priority])&#65292;&#38271;&#24230;&#19981;&#24471;&#22823;&#20110;25&#12290;&#36825;&#26159;&#19968;&#33324;&#38431;&#21015;
    </p>
  </body>
</html></richcontent>
</node>
</node>
<node COLOR="#00b439" CREATED="1316966083598" HGAP="38" ID="ID_239300813" MODIFIED="1317009347825" TEXT="scn-&gt;sc_ops-&gt;txq_aggr_depth" VSHIFT="-51">
<edge COLOR="#00b439" WIDTH="2"/>
<icon BUILTIN="down"/>
<node CREATED="1317008583067" ID="ID_738015265" MODIFIED="1317008949384">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      &#21516;&#26679;&#26159;&#26681;&#25454;&#20248;&#20808;&#32423;&#21035;&#26597;&#30475;&#38271;&#24230;&#65292;&#19981;&#36807;&#36825;&#26159;&#32858;&#21512;&#38431;&#21015;&#65292;
    </p>
    <p>
      &#38271;&#24230;&#23567;&#20110;512/ATH_FRAG_PER_MSDU - 40
    </p>
  </body>
</html></richcontent>
</node>
</node>
<node COLOR="#00b439" CREATED="1316966101343" ID="ID_1004688179" MODIFIED="1317009384028" TEXT="ath_tx_send">
<edge COLOR="#00b439" WIDTH="2"/>
<node COLOR="#00b439" CREATED="1316966115463" HGAP="33" ID="ID_1603533225" MODIFIED="1317090121818" TEXT="&#x662f;&#x5426;&#x652f;&#x6301;uapsd&#x529f;&#x80fd;" VSHIFT="21">
<edge COLOR="#00b439" WIDTH="2"/>
<icon BUILTIN="down"/>
<icon BUILTIN="full-1"/>
</node>
<node COLOR="#990000" CREATED="1316966155343" ID="ID_978219876" MODIFIED="1317090417959" TEXT="ath_amsdu_send">
<edge COLOR="#990000" WIDTH="1"/>
<icon BUILTIN="forward"/>
<icon BUILTIN="full-2"/>
<node COLOR="#990000" CREATED="1316966195055" ID="ID_1296476104" MODIFIED="1317090252725" TEXT="retrun  0">
<edge COLOR="#990000" WIDTH="1"/>
<icon BUILTIN="button_ok"/>
<icon BUILTIN="full-1"/>
<node COLOR="#111111" CREATED="1317009193371" ID="ID_105722258" MODIFIED="1317090146225" TEXT="&#x5728;ath_tx_send&#x8c03;&#x7528;ath_amsdu_send&#x4e4b;&#x540e;&#xff0c;&#x4fbf;&#x4f1a;&#x76f4;&#x63a5;&#x8fd4;&#x56de;&#xff0c;&#x56e0;&#x4e3a;&#x5728;&#x51fd;&#x6570;&#x4e2d;&#x5df2;&#x7ecf;&#x5b8c;&#x6210;&#x53d1;&#x5305;&#x8fc7;&#x7a0b;&#x3002;">
<edge COLOR="#111111" WIDTH="thin"/>
</node>
</node>
<node COLOR="#00b439" CREATED="1316966261673" ID="ID_1583625397" MODIFIED="1317090269803" TEXT="ieee80211amsdu_encap">
<edge COLOR="#00b439" WIDTH="2"/>
<icon BUILTIN="down"/>
<icon BUILTIN="full-2"/>
<node COLOR="#111111" CREATED="1317009405168" ID="ID_1861027631" MODIFIED="1317090280803">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      &#23553;&#35013;&#25104;amsdu&#24103;&#65292;&#22914;&#26524;&#36825;&#26102;amsdu&#24103;&#20026;&#31354;&#65292;&#21017;&#20250;&#21019;&#24314;&#19968;&#20010;&#65292;&#21453;&#20043;&#21017;
    </p>
    <p>
      &#30452;&#25509;&#23558;wbuf&#21152;&#20837;&#24050;&#26377;&#30340;&#24103;&#22836;&#20043;&#20013;&#12290;&#25509;&#30528;&#20256;&#20837;&#30340;wbuf&#20250;&#34987;&#37322;&#25918;&#25481;&#12290;
    </p>
  </body>
</html></richcontent>
<edge COLOR="#111111" WIDTH="thin"/>
</node>
</node>
<node COLOR="#00b439" CREATED="1316966285767" ID="ID_184832697" MODIFIED="1317090273585" TEXT="ath_amsdu_stageq_flush">
<edge COLOR="#00b439" WIDTH="2"/>
<node COLOR="#00b439" CREATED="1316966441480" ID="ID_585152325" MODIFIED="1317090299741" TEXT="ieee80211_encap">
<edge COLOR="#00b439" WIDTH="2"/>
<icon BUILTIN="down"/>
<node COLOR="#111111" CREATED="1317009641293" ID="ID_1933386999" MODIFIED="1317090310569" TEXT="&#x5c01;&#x88c5;&#x6210;80211&#x5e27;&#xff08;&#x5176;&#x5b9e;&#x8fd9;&#x4e2a;&#x51fd;&#x6570;&#x4e5f;&#x80fd;&#x5c01;&#x8f6c;802.3&#x5e27;&#xff09;&#xff0c;&#x540c;&#x4e0b;&#xff1b;">
<edge COLOR="#111111" WIDTH="thin"/>
</node>
</node>
<node COLOR="#00b439" CREATED="1316966450987" ID="ID_955879681" MODIFIED="1317090302569" TEXT="ath_tx_prapare">
<edge COLOR="#00b439" WIDTH="2"/>
<icon BUILTIN="down"/>
<node COLOR="#111111" CREATED="1317009688137" ID="ID_690255144" MODIFIED="1317090317600">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      &#22788;&#29702;&#24103;&#30340;&#20256;&#36755;&#25511;&#21046;&#32467;&#26500;txctl&#65292;&#22788;&#29702;&#21152;&#23494;&#65292;&#21152;&#20837;&#37027;&#20010;&#30828;&#20214;&#38431;&#21015;&#31561;&#12290;
    </p>
  </body>
</html></richcontent>
<edge COLOR="#111111" WIDTH="thin"/>
</node>
</node>
<node COLOR="#00b439" CREATED="1316966482366" ID="ID_1838147936" MODIFIED="1317090325475" TEXT="ath_tx_start">
<edge COLOR="#00b439" WIDTH="2"/>
<arrowlink DESTINATION="ID_234665646" ENDARROW="Default" ENDINCLINATION="291;0;" ID="Arrow_ID_940483182" STARTARROW="None" STARTINCLINATION="690;95;"/>
<node COLOR="#111111" CREATED="1317009744872" ID="ID_772002302" MODIFIED="1317090324897" TEXT="&#x901a;&#x8fc7;ath&#x5c42;&#xff0c;&#x51c6;&#x5907;&#x5411;hal&#x5c42;&#x8fdb;&#x884c;&#x53d1;&#x5305;&#x8bbe;&#x7f6e;&#x4e86;">
<edge COLOR="#111111" WIDTH="thin"/>
</node>
</node>
<node COLOR="#111111" CREATED="1317009594418" ID="ID_1647048108" MODIFIED="1317090277928" TEXT="&#x53d1;&#x9001;amsdu&#x5e27;">
<edge COLOR="#111111" WIDTH="thin"/>
</node>
</node>
</node>
<node COLOR="#00b439" CREATED="1316966172355" ID="ID_959058916" MODIFIED="1317009367231" TEXT="ieee80211_encap">
<edge COLOR="#00b439" WIDTH="2"/>
<icon BUILTIN="down"/>
<node CREATED="1317009641293" ID="ID_1837788125" MODIFIED="1317017846857" TEXT="&#x5c01;&#x88c5;&#x6210;80211&#x5e27;&#xff08;&#x5176;&#x5b9e;&#x8fd9;&#x4e2a;&#x51fd;&#x6570;&#x4e5f;&#x80fd;&#x5c01;&#x8f6c;802.3&#x5e27;&#xff09;&#xff0c;&#x5982;&#x679c;&#x662f;thinap&#x7684;tunnel&#x6a21;&#x5f0f;&#xff0c;&#xa;&#x5219;&#x5df2;&#x7ecf;&#x662f;80211&#x5e27;&#xff0c;&#x7a0d;&#x4f5c;&#x5904;&#x7406;&#x4fbf;&#x53ef;&#x3002;"/>
</node>
<node COLOR="#00b439" CREATED="1316966334213" ID="ID_1327715199" MODIFIED="1317018020139" TEXT="ath_tx_prapare">
<edge COLOR="#00b439" WIDTH="2"/>
<icon BUILTIN="down"/>
<node CREATED="1317009688137" ID="ID_1476279858" MODIFIED="1317009739075">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      &#22788;&#29702;&#24103;&#30340;&#20256;&#36755;&#25511;&#21046;&#32467;&#26500;txctl&#65292;&#22788;&#29702;&#21152;&#23494;&#65292;&#21152;&#20837;&#37027;&#20010;&#30828;&#20214;&#38431;&#21015;&#31561;&#12290;
    </p>
  </body>
</html></richcontent>
</node>
</node>
<node COLOR="#00b439" CREATED="1316966368369" ID="ID_234665646" MODIFIED="1317090325475" TEXT="ath_tx_start">
<edge COLOR="#00b439" WIDTH="2"/>
<node COLOR="#00b439" CREATED="1316966387403" HGAP="38" ID="ID_185611802" MODIFIED="1317018042607" TEXT="__ath_tx_prarare" VSHIFT="16">
<edge COLOR="#00b439" WIDTH="2"/>
<icon BUILTIN="down"/>
<node CREATED="1317009888575" ID="ID_607324561" MODIFIED="1317017909326" TEXT="&#x5904;&#x7406;&#x5e27;&#x7684;&#x4f20;&#x8f93;&#x63a7;&#x5236;&#x7ed3;&#x6784;txctl&#x4e2d;&#x7684;&#x901f;&#x7387;&#x3001;&#x529f;&#x7387;&#x7b49;"/>
</node>
<node COLOR="#00b439" CREATED="1316966410278" HGAP="44" ID="ID_1873330834" MODIFIED="1317009875543" TEXT="wbuf_map_sg" VSHIFT="5">
<edge COLOR="#00b439" WIDTH="2"/>
</node>
<node CREATED="1317017920857" ID="ID_1392630163" MODIFIED="1317018013357" TEXT="&#x7ba1;&#x7406;&#x5e27;&#x4ea4;&#x4e92;&#x65f6;&#xff0c;&#x4f1a;&#x76f4;&#x63a5;&#x901a;&#x8fc7;ath&#x5c42;&#xff0c;&#x8c03;&#x7528;&#x8be5;&#x51fd;&#x6570;&#x8fdb;&#x884c;&#x53d1;&#x5305;&#x3002;"/>
</node>
<node CREATED="1317009070229" ID="ID_375972258" MODIFIED="1317009147824" TEXT="&#x4f18;&#x5148;&#x7ea7;&#x522b;&#x4e0e;&#x786c;&#x4ef6;&#x961f;&#x5217;&#x5e76;&#x975e;&#x4e00;&#x4e00;&#x5bf9;&#x5e94;&#xff0c;&#x901a;&#x5e38;&#x786c;&#x4ef6;&#x961f;&#x5217;&#x8f83;&#x5c11;&#xff0c;&#x751a;&#x81f3;&#x53ea;&#x5bf9;&#x5e94;&#x4e00;&#x4e2a;&#x786c;&#x4ef6;&#x961f;&#x5217;"/>
</node>
</node>
</node>
<node CREATED="1317008278598" HGAP="52" ID="ID_1798213232" MODIFIED="1317008550192" VSHIFT="-18">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      &#22312;wifi&#35774;&#22791;&#34987;&#21021;&#22987;&#21270;&#30340;&#26102;&#20505;&#65292;&#21019;&#24314;net_device&#32467;&#26500;&#65292;&#25152;&#25346;&#38057;&#30340;&#21457;&#21253;&#20989;&#25968;&#65292;
    </p>
    <p>
      &#22240;&#27492;&#36825;&#26159;wifi&#30340;&#21457;&#21253;&#20989;&#25968;&#12290;&#36825;&#20010;&#20989;&#25968;&#21487;&#20197;&#20174;ieee802.11&#23618;&#21457;&#21253;&#20989;&#25968;&#35843;&#29992;
    </p>
    <p>
      &#19979;&#26469;&#65292;&#20063;&#21487;&#20197;&#26159;&#19968;&#20123;&#20989;&#25968;&#30452;&#25509;&#35843;&#29992;&#65292;&#21482;&#35201;skb-&gt;dev&#20013;&#26159;wifi&#21363;&#21487;&#12290;
    </p>
    <p>
      &#26368;&#21518;&#23427;&#20250;&#22312;hal&#23618;&#36827;&#34892;&#20123;&#35843;&#29992;&#36827;&#34892;&#30828;&#20214;&#21457;&#21253;&#65292;&#27604;&#22914;dma&#25805;&#20316;&#65292;&#35774;&#32622;&#21457;&#21253;
    </p>
    <p>
      &#23492;&#23384;&#22120;&#12290;
    </p>
  </body>
</html></richcontent>
</node>
</node>
<node CREATED="1317219574177" ID="ID_321896081" MODIFIED="1317219606765" POSITION="left" TEXT="wbuf_map_sg">
<node CREATED="1317219735483" ID="ID_813238595" MODIFIED="1317219745563" TEXT="__wbuf_map_sg">
<node CREATED="1317219748470" ID="ID_856834084" MODIFIED="1317219776739" TEXT="bus_map_single"/>
<node CREATED="1317219777213" ID="ID_1062135751" MODIFIED="1317219801933" TEXT="wbuf_start_dma">
<node CREATED="1317220140267" ID="ID_1851912951" MODIFIED="1317220175260" TEXT="ath_tx_start_dma">
<node CREATED="1317221244723" ID="ID_1654388093" MODIFIED="1317221271814" TEXT="txctl-&gt;ps || avp-&gt;av_mcastq.axq_depth">
<node CREATED="1317221279519" ID="ID_1764977997" MODIFIED="1317221524886">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      &#24403;&#23458;&#25143;&#31471;&#22312;&#33410;&#30005;&#27169;&#24335;&#19979;&#65292;&#22914;&#26524;&#26377;&#24191;&#25773;&#24103;&#22312;&#24191;&#25773;&#38431;&#21015;&#20013;&#65292;
    </p>
    <p>
      &#38459;&#27490;&#20854;&#21457;&#36865;&#65292;&#25918;&#20837;cab&#38431;&#21015;&#20013;&#65292;&#31561;&#24453;&#26377;beancon&#24103;&#21518;&#20877;&#26469;
    </p>
    <p>
      &#21457;&#36865;
    </p>
  </body>
</html>
</richcontent>
</node>
</node>
<node CREATED="1317221274956" ID="ID_1026478372" MODIFIED="1317221838251" TEXT="ath_tx_get_buf">
<node CREATED="1317222842580" ID="ID_1123018224" MODIFIED="1317223837875">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      &#25152;&#26377;&#30340;&#21457;&#21253;ath_buf&#37117;&#20250;&#23384;&#22312;sc-&gt;sc_txbuf&#20013;&#65292;&#36825;&#20010;&#20989;&#25968;&#20174;sc_txbuf&#20013;&#31532;&#19968;&#20010;
    </p>
    <p>
      &#21462;&#20986;&#26469;&#65292;&#32473;&#21442;&#25968;&amp;bf&#65292;&#22312;&#20989;&#25968;&#20013;&#20250;&#32473;bf&#20013;&#25968;&#25454;&#30340;dma&#29289;&#29702;&#22320;&#22336;&#21644;&#38271;&#24230;&#36171;&#20540;&#65292;
    </p>
    <p>
      &#22312;&#22806;&#38754;&#20250;&#25509;&#30528;&#32473;bf&#20854;&#20182;&#20540;&#36171;&#20540;&#65292;&#27604;&#22914;&#21457;&#21253;&#30340;wbuf&#23601;&#26159;&#36825;&#26102;&#25346;&#36733;&#19978;bf&#30340;
    </p>
  </body>
</html>
</richcontent>
</node>
</node>
<node CREATED="1317223325463" ID="ID_467062122" MODIFIED="1317223328553" TEXT="ath_hal_setdesclink">
<node CREATED="1317223329214" ID="ID_568320668" MODIFIED="1317223634049">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      &#36890;&#36807;bf&#24471;&#21040;&#21644;&#36825;&#20010;bf&#20851;&#32852;&#22312;&#19968;&#36215;&#30340;&#25551;&#36848;&#31526;ds&#65288;&#32593;&#21345;&#21021;&#22987;&#21270;&#26102;&#23601;&#20851;&#32852;&#22909;&#20102;&#65289;&#65292;
    </p>
    <p>
      &#20989;&#25968;&#37324;&#38754;&#21021;&#22987;&#21270;&#20102;&#25551;&#36848;&#31526;&#23383;&#27573;2-&#25351;&#21521;&#25551;&#36848;&#31526;&#26412;&#36523;&#30340;&#25351;&#38024;&#65292;&#23383;&#27573;10-&#25551;&#36848;&#31526;&#30340;
    </p>
    <p>
      &#26657;&#39564;&#30721;
    </p>
  </body>
</html>
</richcontent>
</node>
</node>
<node CREATED="1317223853949" ID="ID_470412587" MODIFIED="1317223857805" TEXT="OS_COPY_DMA_MEM_CONTEXT">
<node CREATED="1317223858434" ID="ID_130703626" MODIFIED="1317223936327" TEXT="&#x5c06;dma&#x4e0a;&#x4e0b;&#x6587;&#x7684;&#x4e00;&#x4e9b;&#x6570;&#x636e;&#x8d4b;&#x503c;&#x5230;bf&#x4e2d;"/>
</node>
<node CREATED="1317224067106" ID="ID_1051714647" MODIFIED="1317224086249" TEXT="&#x662f;&#x5426;&#x4e3a;&#x7b2c;&#x4e00;&#x4e2a;&#x63cf;&#x8ff0;&#x7b26;"/>
</node>
</node>
<node CREATED="1317219816276" ID="ID_284429275" MODIFIED="1317220263065" TEXT="bus_unmap_single">
<icon BUILTIN="full-2"/>
</node>
</node>
</node>
</node>
</map>
