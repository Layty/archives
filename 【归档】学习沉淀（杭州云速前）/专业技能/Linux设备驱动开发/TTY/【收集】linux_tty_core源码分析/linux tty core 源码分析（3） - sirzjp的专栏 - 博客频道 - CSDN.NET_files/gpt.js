(function(){var a=this;var c=function(b,e){var d=parseFloat(b);return isNaN(d)||1<d||0>d?e:d},f=function(b,e){var d=parseInt(b,10);return isNaN(d)?e:d},g=/^([\w-]+\.)*([\w-]{2,})(\:[0-9]+)?$/,h=function(b,e){if(!b)return e;var d=b.match(g);return d?d[0]:e};var k=c("0.02",0),l=c("0.0",0);var m=c("0.005",0),n=c("0",0),p=c("0.001",0),q=f("1500",1500),r=c("0.01",0),t=c("1.0",0),u=c("0.5",0),v=c("",.001),w=f("",200),x=c("0.01",
0),y=/^true$/.test("")?!0:!1,z=c("0.01",0),A=c("0.01",0),B=c("0.1",0),C=c("0.01",0),D=c("1",0),E=c("",.001),F=c("0.01",0),G=c("0.5",0),H=c("0.0",0),I=c("0.001",0),J=c("0.0001",
0),aa=c("0.0",0),K=f("75",0),ba=f("75",0),L=c("0",0);var ca=/^true$/.test("true")?!0:!1;var M=function(){return a.googletag||(a.googletag={})},N=function(b,e){var d=M();d.hasOwnProperty(b)||(d[b]=e)};var O={};O["#1#"]=h("","pagead2.googlesyndication.com");O["#2#"]=h("","pubads.g.doubleclick.net");O["#3#"]=h("","securepubads.g.doubleclick.net");O["#4#"]=h("","partner.googleadservices.com");O["#6#"]=function(b){try{for(var e=null;e!=b;e=b,b=b.parent)switch(b.location.protocol){case "https:":return!0;case "http:":case "file:":return!1}}catch(d){}return!0}(window);O["#7#"]=k;O["#10#"]=n;O["#11#"]=p;
O["#12#"]=m;O["#13#"]=q;O["#16#"]=r;O["#17#"]=t;O["#18#"]=u;O["#20#"]=l;O["#23#"]=v;O["#24#"]=w;O["#27#"]=x;O["#28#"]=z;O["#29#"]=A;O["#31#"]=B;O["#33#"]=h("","pagead2.googlesyndication.com");O["#34#"]=D;O["#36#"]=y;O["#37#"]=C;O["#38#"]=E;O["#39#"]="";O["#40#"]=F;O["#42#"]=G;O["#43#"]=H;O["#45#"]=I;O["#46#"]=ca;O["#47#"]=J;O["#48#"]=aa;O["#49#"]=(new Date).getTime();O["#52#"]=0;O["#53#"]="";N("_vars_",O);N("getVersion",function(){return"75"});var da={};var P=document;N("cmd",[]);
if("function"==function(){var b=googletag.evalScripts,e=typeof b;if("object"==e)if(b){if(b instanceof Array)return"array";if(b instanceof Object)return e;var d=Object.prototype.toString.call(b);if("[object Window]"==d)return"object";if("[object Array]"==d||"number"==typeof b.length&&"undefined"!=typeof b.splice&&"undefined"!=typeof b.propertyIsEnumerable&&!b.propertyIsEnumerable("splice"))return"array";if("[object Function]"==d||"undefined"!=typeof b.call&&"undefined"!=typeof b.propertyIsEnumerable&&
!b.propertyIsEnumerable("call"))return"function"}else return"null";else if("function"==e&&"undefined"==typeof b.call)return"object";return e}())googletag.evalScripts();else{var Q,R=ba||"75",S="",T;if(K&&L){a:{var U=[K,R],ea=2*L;if(!(1E-4>Math.random())){var V=Math.random();if(V<ea){var fa=window;try{var W=new Uint32Array(1);fa.crypto.getRandomValues(W);V=W[0]/65536/65536}catch(b){V=Math.random()}T=U[Math.floor(V*U.length)];break a}}T=null}T==K?O["#53#"]="108809094":T==R&&(O["#53#"]="108809093",S=
"?v="+K)}T=T||R;Q=(da["#6#"]||M()._vars_["#6#"]?"https:":"http:")+"//partner.googleadservices.com/gpt/pubads_impl_"+T+".js"+S;var X=P.currentScript;if(!("complete"==P.readyState||"loaded"==P.readyState||X&&X.async)){var Y="gpt-impl-"+Math.random();try{P.write('<script id="'+Y+'" src="'+Q+'">\x3c/script>')}catch(b){}P.getElementById(Y)&&(M()._loadStarted_=!0)}if(!M()._loadStarted_){var Z=P.createElement("script");Z.src=Q;Z.async=!0;(P.head||P.body||P.documentElement).appendChild(Z);M()._loadStarted_=
!0}};})()
