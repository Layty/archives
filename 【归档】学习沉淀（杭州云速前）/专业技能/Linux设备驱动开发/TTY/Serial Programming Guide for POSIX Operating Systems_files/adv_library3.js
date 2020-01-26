// UP-LIB START
window.IOLOBJ=window.IOLOBJ||{};window.IOLOBJ.IS_LOADED=window.IOLOBJ.IS_LOADED||false;window.IOLOBJ.Profile=window.IOLOBJ.Profile||{};window.IOLOBJ.Profile.FUP=window.IOLOBJ.Profile.FUP||{};window.UTL=window.UTL||{};window.JSON=window.JSON||{};JSON.stringify=JSON.stringify||function(obj){var t=typeof(obj);if(t!="object"||obj===null){if(t=="string")
obj='"'+obj+'"';return String(obj);}else{var n,v,json=[],arr=(obj&&obj.constructor==Array);for(n in obj){v=obj[n];t=typeof(v);if(t=="string")
v='"'+v+'"';else if(t=="object"&&v!==null)
v=JSON.stringify(v);json.push((arr?"":'"'+n+'":')+String(v));}
return(arr?"[":"{")+String(json)+(arr?"]":"}");}};JSON.parse=JSON.parse||function(str){if(str==="")
str='""';eval("var p="+str+";");return p;};UTL.cdf=function(t){var i,r="",p;for(i=t.length-1;i>=0;i--)
r+=(p=t.charCodeAt(i).toString(16))<10?"0"+p:p;return r;};UTL.dcdf=function(t){var i,r="",p;for(i=t.length-2;i>=0;i-=2)
r+=String.fromCharCode(parseInt(t.substr(i,2),16));return r;};UTL.isEmptyObj=function(obj){if(UTL.isEmpty(obj))
return true;for(var p in obj)
if(obj.hasOwnProperty(p))
return false;return true;};UTL.emptyFunction=function(){};UTL.toStringOf=function(obj){return Object.prototype.toString.call(obj);};UTL.getGMTString=function(days){dt=new Date();dt.setTime(dt.getTime()+86400000*days);return dt.toGMTString();}
UTL.isArray=('isArray'in Array)?Array.isArray:function(value){return UTL.toStringOf(value)==='[object Array]';};UTL.isEmpty=function(value,allowEmptyString){return(typeof(value)=='undefined')||(value===null)||(!allowEmptyString?value==='':false)||(UTL.isArray(value)&&value.length===0);};UTL.getHostName=function(){return document.location.host;};UTL.getDomainFromHostName=function(hostNameV){hostName=hostNameV||UTL.getHostName();var tmp=hostName.split('.');if(tmp.length>2){return tmp[tmp.length-2]+"."+tmp[tmp.length-1];}else{return hostName;}};UTL.getCookie=function(_name){if(UTL.isEmpty(_name))
return null;var nameEQ=_name+"=";nameEQ=nameEQ.toLowerCase();var v_cookies=document.cookie||'';var v_cookies_low=v_cookies.toLowerCase();var cookieStartIndex=v_cookies_low.indexOf(nameEQ);if(cookieStartIndex==-1)
return null;var cookieEndIndex=v_cookies.indexOf(';',cookieStartIndex+nameEQ.length);if(cookieEndIndex==-1)
cookieEndIndex=v_cookies.length;var cValue=v_cookies.substring(cookieStartIndex+nameEQ.length,cookieEndIndex);if(cValue.indexOf('Warning')!=-1||cValue.indexOf('Error')!=-1||cValue.indexOf('Undefined')!=-1||cValue.indexOf('<?php')!=-1){return-1;}
return unescape(cValue);};UTL.loadScript=function(options){options=options||{};var params={};params.url=options.url||null;params.appendToBody=options.appendToBody||false;params.callBack=options.callBack||UTL.emptyFunction;var script=document.createElement("script");script.type="text/javascript";if(script.readyState){script.onreadystatechange=function(){if(script.readyState=="loaded"||script.readyState=="complete"){script.onreadystatechange=null;params.callBack();}};}else{script.onload=function(){params.callBack();};}
script.src=params.url;var eleParent=document.getElementsByTagName("head")[0];if(params.appendToBody){eleParent=document.getElementsByTagName("body")[0];}
eleParent.appendChild(script);};window.IOLOBJ.LIBERIX={};window.IOLOBJ.LIBERIX.data={};window.IOLOBJ.LIBERIX.data.regioni={"01":{nome:"piemonte",codiceIstat:"01",codiceLiberiX:"12"},"02":{nome:"valle d'aosta",codiceIstat:"02",codiceLiberiX:"19"},"03":{nome:"lombardia",codiceIstat:"03",codiceLiberiX:"9"},"04":{nome:"trentino-alto adige",codiceIstat:"04",codiceLiberiX:"17"},"05":{nome:"veneto",codiceIstat:"05",codiceLiberiX:"20"},"06":{nome:"friuli-venezia giulia",codiceIstat:"06",codiceLiberiX:"6"},"07":{nome:"liguria",codiceIstat:"07",codiceLiberiX:"8"},"08":{nome:"emilia-romagna",codiceIstat:"08",codiceLiberiX:"5"},"09":{nome:"toscana",codiceIstat:"09",codiceLiberiX:"16"},"10":{nome:"umbria",codiceIstat:"10",codiceLiberiX:"18"},"11":{nome:"marche",codiceIstat:"11",codiceLiberiX:"10"},"12":{nome:"lazio",codiceIstat:"12",codiceLiberiX:"7"},"13":{nome:"abruzzo",codiceIstat:"13",codiceLiberiX:"1"},"14":{nome:"molise",codiceIstat:"14",codiceLiberiX:"11"},"15":{nome:"campania",codiceIstat:"15",codiceLiberiX:"4"},"16":{nome:"puglia",codiceIstat:"16",codiceLiberiX:"13"},"17":{nome:"basilicata",codiceIstat:"17",codiceLiberiX:"2"},"18":{nome:"calabria",codiceIstat:"18",codiceLiberiX:"3"},"19":{nome:"sicilia",codiceIstat:"19",codiceLiberiX:"15"},"20":{nome:"sardegna",codiceIstat:"20",codiceLiberiX:"14"}};window.IOLOBJ.LIBERIX.data.locTurRif={"cadore":"025054","capo-vaticano":"102047","castelli-romani":"058003","chianti":"052032","cilento":"065032","cinque-terre":"011015","circeo":"059011","conero":"042002","costa-degli-etruschi":"049009","costa-smeralda":"104017","costiera-amalfitana":"065116","gargano":"071038","golfo-degli-angeli":"092009","isola-d-elba":"049014","isole-eolie":"083041","isole-flegree":"063037","lago-di-como":"013075","lago-di-garda":"017067","lago-maggiore":"012133","lago-trasimeno":"054009","langhe":"004003","litorale-adriatico":"069099","litorale-laziale":"058032","litorale-veneto":"027019","maremma":"053011","penisola-sorrentina":"063024","riviera-adriatica":"044066","riviera-dei-fiori":"008031","riviera-del-corallo":"090003","riviera-di-levante":"010059","riviera-di-ponente":"009056","riviera-romagnola":"099014","salento":"075035","tirolo":"021051","val-di-fiemme":"022147","val-di-sole":"022136","val-di-susa":"001022","val-gardena":"021061","val-pusteria":"021013","val-venosta":"021046","valle-isarco":"021011","valtellina":"014061","versilia":"046013"}
window.IOLOBJ.LIBERIX.data.locTurRifByIstat={"900005":"025054","900010":"102047","900124":"058003","900001":"052032","900121":"065032","900012":"011015","900013":"059011","900118":"042002","900015":"049009","900016":"104017","900018":"065116","900024":"071038","900125":"092009","900032":"049014","900123":"083041","900036":"063037","900040":"013075","900041":"017067","900044":"012133","900045":"054009","900119":"004003","900048":"069099","900049":"058032","900050":"027019","900052":"053011","900060":"063024","900064":"044066","900065":"008031","900122":"090003","900066":"010059","900067":"009056","900070":"099014","900071":"075035","900086":"021051","900091":"022147","900093":"022136","900094":"001022","900097":"021061","900098":"021013","900102":"021046","900108":"021011","900116":"014061","900117":"046013"}
window.IOLOBJ.LIBERIX.data.mdecIstat={1:"084",2:"006",3:"042",4:"007",5:"051",6:"044",7:"005",8:"064",9:"072",10:"025",11:"062",12:"016",13:"096",14:"037",15:"021",16:"017",17:"074",18:"092",19:"085",20:"070",21:"107",22:"061",23:"087",24:"079",25:"069",26:"013",27:"078",28:"019",29:"101",30:"004",31:"086",32:"038",33:"048",34:"071",35:"040",36:"060",37:"010",38:"031",39:"053",40:"008",41:"094",42:"011",43:"066",44:"059",45:"075",46:"097",47:"049",48:"098",49:"046",50:"043",51:"020",52:"045",53:"077",54:"083",55:"015",56:"036",57:"063",58:"003",59:"091",60:"104",61:"095",62:"028",63:"082",64:"034",65:"018",66:"054",67:"041",68:"068",69:"033",70:"050",71:"047",72:"093",73:"076",74:"100",75:"088",76:"039",77:"080",78:"035",79:"057",80:"099",81:"058",82:"029",83:"065",84:"107",85:"090",86:"009",87:"052",88:"089",89:"014",90:"073",91:"067",92:"055",93:"001",94:"105",95:"081",96:"022",97:"026",98:"032",99:"030",100:"012",101:"027",102:"103",103:"002",104:"023",105:"102",106:"024",107:"056",108:"110",109:"109",110:"108"};window.IOLOBJ.LIBERIX.data.province={"torino":{nomeProvincia:"torino",codiceIstatProvincia:"001",siglaProvincia:"TO",codiceIstatRegione:"01"},"vercelli":{nomeProvincia:"vercelli",codiceIstatProvincia:"002",siglaProvincia:"VC",codiceIstatRegione:"01"},"novara":{nomeProvincia:"novara",codiceIstatProvincia:"003",siglaProvincia:"NO",codiceIstatRegione:"01"},"cuneo":{nomeProvincia:"cuneo",codiceIstatProvincia:"004",siglaProvincia:"CN",codiceIstatRegione:"01"},"asti":{nomeProvincia:"asti",codiceIstatProvincia:"005",siglaProvincia:"AT",codiceIstatRegione:"01"},"alessandria":{nomeProvincia:"alessandria",codiceIstatProvincia:"006",siglaProvincia:"AL",codiceIstatRegione:"01"},"biella":{nomeProvincia:"biella",codiceIstatProvincia:"096",siglaProvincia:"BI",codiceIstatRegione:"01"},"verbanocusioossola":{nomeProvincia:"verbano-cusio-ossola",codiceIstatProvincia:"103",siglaProvincia:"VB",codiceIstatRegione:"01"},"valledaosta":{nomeProvincia:"valle d'aosta",codiceIstatProvincia:"007",siglaProvincia:"AO",codiceIstatRegione:"02"},"varese":{nomeProvincia:"varese",codiceIstatProvincia:"012",siglaProvincia:"VA",codiceIstatRegione:"03"},"como":{nomeProvincia:"como",codiceIstatProvincia:"013",siglaProvincia:"CO",codiceIstatRegione:"03"},"sondrio":{nomeProvincia:"sondrio",codiceIstatProvincia:"014",siglaProvincia:"SO",codiceIstatRegione:"03"},"milano":{nomeProvincia:"milano",codiceIstatProvincia:"015",siglaProvincia:"MI",codiceIstatRegione:"03"},"bergamo":{nomeProvincia:"bergamo",codiceIstatProvincia:"016",siglaProvincia:"BG",codiceIstatRegione:"03"},"brescia":{nomeProvincia:"brescia",codiceIstatProvincia:"017",siglaProvincia:"BS",codiceIstatRegione:"03"},"pavia":{nomeProvincia:"pavia",codiceIstatProvincia:"018",siglaProvincia:"PV",codiceIstatRegione:"03"},"cremona":{nomeProvincia:"cremona",codiceIstatProvincia:"019",siglaProvincia:"CR",codiceIstatRegione:"03"},"mantova":{nomeProvincia:"mantova",codiceIstatProvincia:"020",siglaProvincia:"MN",codiceIstatRegione:"03"},"lecco":{nomeProvincia:"lecco",codiceIstatProvincia:"097",siglaProvincia:"LC",codiceIstatRegione:"03"},"lodi":{nomeProvincia:"lodi",codiceIstatProvincia:"098",siglaProvincia:"LO",codiceIstatRegione:"03"},"monzaedellabrianza":{nomeProvincia:"monza e della brianza",codiceIstatProvincia:"108",siglaProvincia:"MB",codiceIstatRegione:"03"},"bolzano":{nomeProvincia:"bolzano",codiceIstatProvincia:"021",siglaProvincia:"BZ",codiceIstatRegione:"04"},"trento":{nomeProvincia:"trento",codiceIstatProvincia:"022",siglaProvincia:"TN",codiceIstatRegione:"04"},"verona":{nomeProvincia:"verona",codiceIstatProvincia:"023",siglaProvincia:"VR",codiceIstatRegione:"05"},"vicenza":{nomeProvincia:"vicenza",codiceIstatProvincia:"024",siglaProvincia:"VI",codiceIstatRegione:"05"},"belluno":{nomeProvincia:"belluno",codiceIstatProvincia:"025",siglaProvincia:"BL",codiceIstatRegione:"05"},"treviso":{nomeProvincia:"treviso",codiceIstatProvincia:"026",siglaProvincia:"TV",codiceIstatRegione:"05"},"venezia":{nomeProvincia:"venezia",codiceIstatProvincia:"027",siglaProvincia:"VE",codiceIstatRegione:"05"},"padova":{nomeProvincia:"padova",codiceIstatProvincia:"028",siglaProvincia:"PD",codiceIstatRegione:"05"},"rovigo":{nomeProvincia:"rovigo",codiceIstatProvincia:"029",siglaProvincia:"RO",codiceIstatRegione:"05"},"udine":{nomeProvincia:"udine",codiceIstatProvincia:"030",siglaProvincia:"UD",codiceIstatRegione:"06"},"gorizia":{nomeProvincia:"gorizia",codiceIstatProvincia:"031",siglaProvincia:"GO",codiceIstatRegione:"06"},"trieste":{nomeProvincia:"trieste",codiceIstatProvincia:"032",siglaProvincia:"TS",codiceIstatRegione:"06"},"pordenone":{nomeProvincia:"pordenone",codiceIstatProvincia:"093",siglaProvincia:"PN",codiceIstatRegione:"06"},"imperia":{nomeProvincia:"imperia",codiceIstatProvincia:"008",siglaProvincia:"IM",codiceIstatRegione:"07"},"savona":{nomeProvincia:"savona",codiceIstatProvincia:"009",siglaProvincia:"SV",codiceIstatRegione:"07"},"genova":{nomeProvincia:"genova",codiceIstatProvincia:"010",siglaProvincia:"GE",codiceIstatRegione:"07"},"laspezia":{nomeProvincia:"la spezia",codiceIstatProvincia:"011",siglaProvincia:"SP",codiceIstatRegione:"07"},"piacenza":{nomeProvincia:"piacenza",codiceIstatProvincia:"033",siglaProvincia:"PC",codiceIstatRegione:"08"},"parma":{nomeProvincia:"parma",codiceIstatProvincia:"034",siglaProvincia:"PR",codiceIstatRegione:"08"},"reggionellemilia":{nomeProvincia:"reggio nell'emilia",codiceIstatProvincia:"035",siglaProvincia:"RE",codiceIstatRegione:"08"},"modena":{nomeProvincia:"modena",codiceIstatProvincia:"036",siglaProvincia:"MO",codiceIstatRegione:"08"},"bologna":{nomeProvincia:"bologna",codiceIstatProvincia:"037",siglaProvincia:"BO",codiceIstatRegione:"08"},"ferrara":{nomeProvincia:"ferrara",codiceIstatProvincia:"038",siglaProvincia:"FE",codiceIstatRegione:"08"},"ravenna":{nomeProvincia:"ravenna",codiceIstatProvincia:"039",siglaProvincia:"RA",codiceIstatRegione:"08"},"forl\u00eccesena":{nomeProvincia:"forl\u00ec-cesena",codiceIstatProvincia:"040",siglaProvincia:"FC",codiceIstatRegione:"08"},"rimini":{nomeProvincia:"rimini",codiceIstatProvincia:"099",siglaProvincia:"RN",codiceIstatRegione:"08"},"massacarrara":{nomeProvincia:"massa-carrara",codiceIstatProvincia:"045",siglaProvincia:"MS",codiceIstatRegione:"09"},"lucca":{nomeProvincia:"lucca",codiceIstatProvincia:"046",siglaProvincia:"LU",codiceIstatRegione:"09"},"pistoia":{nomeProvincia:"pistoia",codiceIstatProvincia:"047",siglaProvincia:"PT",codiceIstatRegione:"09"},"firenze":{nomeProvincia:"firenze",codiceIstatProvincia:"048",siglaProvincia:"FI",codiceIstatRegione:"09"},"livorno":{nomeProvincia:"livorno",codiceIstatProvincia:"049",siglaProvincia:"LI",codiceIstatRegione:"09"},"pisa":{nomeProvincia:"pisa",codiceIstatProvincia:"050",siglaProvincia:"PI",codiceIstatRegione:"09"},"arezzo":{nomeProvincia:"arezzo",codiceIstatProvincia:"051",siglaProvincia:"AR",codiceIstatRegione:"09"},"siena":{nomeProvincia:"siena",codiceIstatProvincia:"052",siglaProvincia:"SI",codiceIstatRegione:"09"},"grosseto":{nomeProvincia:"grosseto",codiceIstatProvincia:"053",siglaProvincia:"GR",codiceIstatRegione:"09"},"prato":{nomeProvincia:"prato",codiceIstatProvincia:"100",siglaProvincia:"PO",codiceIstatRegione:"09"},"perugia":{nomeProvincia:"perugia",codiceIstatProvincia:"054",siglaProvincia:"PG",codiceIstatRegione:"10"},"terni":{nomeProvincia:"terni",codiceIstatProvincia:"055",siglaProvincia:"TR",codiceIstatRegione:"10"},"pesarourbino":{nomeProvincia:"pesaro urbino",codiceIstatProvincia:"041",siglaProvincia:"PU",codiceIstatRegione:"11"},"ancona":{nomeProvincia:"ancona",codiceIstatProvincia:"042",siglaProvincia:"AN",codiceIstatRegione:"11"},"macerata":{nomeProvincia:"macerata",codiceIstatProvincia:"043",siglaProvincia:"MC",codiceIstatRegione:"11"},"ascolipiceno":{nomeProvincia:"ascoli piceno",codiceIstatProvincia:"044",siglaProvincia:"AP",codiceIstatRegione:"11"},"fermo":{nomeProvincia:"fermo",codiceIstatProvincia:"109",siglaProvincia:"FM",codiceIstatRegione:"11"},"viterbo":{nomeProvincia:"viterbo",codiceIstatProvincia:"056",siglaProvincia:"VT",codiceIstatRegione:"12"},"rieti":{nomeProvincia:"rieti",codiceIstatProvincia:"057",siglaProvincia:"RI",codiceIstatRegione:"12"},"roma":{nomeProvincia:"roma",codiceIstatProvincia:"058",siglaProvincia:"RM",codiceIstatRegione:"12"},"latina":{nomeProvincia:"latina",codiceIstatProvincia:"059",siglaProvincia:"LT",codiceIstatRegione:"12"},"frosinone":{nomeProvincia:"frosinone",codiceIstatProvincia:"060",siglaProvincia:"FR",codiceIstatRegione:"12"},"laquila":{nomeProvincia:"l'aquila",codiceIstatProvincia:"066",siglaProvincia:"AQ",codiceIstatRegione:"13"},"teramo":{nomeProvincia:"teramo",codiceIstatProvincia:"067",siglaProvincia:"TE",codiceIstatRegione:"13"},"pescara":{nomeProvincia:"pescara",codiceIstatProvincia:"068",siglaProvincia:"PE",codiceIstatRegione:"13"},"chieti":{nomeProvincia:"chieti",codiceIstatProvincia:"069",siglaProvincia:"CH",codiceIstatRegione:"13"},"campobasso":{nomeProvincia:"campobasso",codiceIstatProvincia:"070",siglaProvincia:"CB",codiceIstatRegione:"14"},"isernia":{nomeProvincia:"isernia",codiceIstatProvincia:"094",siglaProvincia:"IS",codiceIstatRegione:"14"},"caserta":{nomeProvincia:"caserta",codiceIstatProvincia:"061",siglaProvincia:"CE",codiceIstatRegione:"15"},"benevento":{nomeProvincia:"benevento",codiceIstatProvincia:"062",siglaProvincia:"BN",codiceIstatRegione:"15"},"napoli":{nomeProvincia:"napoli",codiceIstatProvincia:"063",siglaProvincia:"NA",codiceIstatRegione:"15"},"avellino":{nomeProvincia:"avellino",codiceIstatProvincia:"064",siglaProvincia:"AV",codiceIstatRegione:"15"},"salerno":{nomeProvincia:"salerno",codiceIstatProvincia:"065",siglaProvincia:"SA",codiceIstatRegione:"15"},"foggia":{nomeProvincia:"foggia",codiceIstatProvincia:"071",siglaProvincia:"FG",codiceIstatRegione:"16"},"bari":{nomeProvincia:"bari",codiceIstatProvincia:"072",siglaProvincia:"BA",codiceIstatRegione:"16"},"taranto":{nomeProvincia:"taranto",codiceIstatProvincia:"073",siglaProvincia:"TA",codiceIstatRegione:"16"},"brindisi":{nomeProvincia:"brindisi",codiceIstatProvincia:"074",siglaProvincia:"BR",codiceIstatRegione:"16"},"lecce":{nomeProvincia:"lecce",codiceIstatProvincia:"075",siglaProvincia:"LE",codiceIstatRegione:"16"},"barlettaandriatrani":{nomeProvincia:"barletta-andria-trani",codiceIstatProvincia:"110",siglaProvincia:"BT",codiceIstatRegione:"16"},"potenza":{nomeProvincia:"potenza",codiceIstatProvincia:"076",siglaProvincia:"PZ",codiceIstatRegione:"17"},"matera":{nomeProvincia:"matera",codiceIstatProvincia:"077",siglaProvincia:"MT",codiceIstatRegione:"17"},"cosenza":{nomeProvincia:"cosenza",codiceIstatProvincia:"078",siglaProvincia:"CS",codiceIstatRegione:"18"},"catanzaro":{nomeProvincia:"catanzaro",codiceIstatProvincia:"079",siglaProvincia:"CZ",codiceIstatRegione:"18"},"reggiodicalabria":{nomeProvincia:"reggio di calabria",codiceIstatProvincia:"080",siglaProvincia:"RC",codiceIstatRegione:"18"},"crotone":{nomeProvincia:"crotone",codiceIstatProvincia:"101",siglaProvincia:"KR",codiceIstatRegione:"18"},"vibovalentia":{nomeProvincia:"vibo valentia",codiceIstatProvincia:"102",siglaProvincia:"VV",codiceIstatRegione:"18"},"trapani":{nomeProvincia:"trapani",codiceIstatProvincia:"081",siglaProvincia:"TP",codiceIstatRegione:"19"},"palermo":{nomeProvincia:"palermo",codiceIstatProvincia:"082",siglaProvincia:"PA",codiceIstatRegione:"19"},"messina":{nomeProvincia:"messina",codiceIstatProvincia:"083",siglaProvincia:"ME",codiceIstatRegione:"19"},"agrigento":{nomeProvincia:"agrigento",codiceIstatProvincia:"084",siglaProvincia:"AG",codiceIstatRegione:"19"},"caltanissetta":{nomeProvincia:"caltanissetta",codiceIstatProvincia:"085",siglaProvincia:"CL",codiceIstatRegione:"19"},"enna":{nomeProvincia:"enna",codiceIstatProvincia:"086",siglaProvincia:"EN",codiceIstatRegione:"19"},"catania":{nomeProvincia:"catania",codiceIstatProvincia:"087",siglaProvincia:"CT",codiceIstatRegione:"19"},"ragusa":{nomeProvincia:"ragusa",codiceIstatProvincia:"088",siglaProvincia:"RG",codiceIstatRegione:"19"},"siracusa":{nomeProvincia:"siracusa",codiceIstatProvincia:"089",siglaProvincia:"SR",codiceIstatRegione:"19"},"sassari":{nomeProvincia:"sassari",codiceIstatProvincia:"090",siglaProvincia:"SS",codiceIstatRegione:"20"},"nuoro":{nomeProvincia:"nuoro",codiceIstatProvincia:"091",siglaProvincia:"NU",codiceIstatRegione:"20"},"cagliari":{nomeProvincia:"cagliari",codiceIstatProvincia:"092",siglaProvincia:"CA",codiceIstatRegione:"20"},"oristano":{nomeProvincia:"oristano",codiceIstatProvincia:"095",siglaProvincia:"OR",codiceIstatRegione:"20"},"olbiatempio":{nomeProvincia:"olbia-tempio",codiceIstatProvincia:"104",siglaProvincia:"OT",codiceIstatRegione:"20"},"ogliastra":{nomeProvincia:"ogliastra",codiceIstatProvincia:"105",siglaProvincia:"OG",codiceIstatRegione:"20"},"mediocampidano":{nomeProvincia:"medio campidano",codiceIstatProvincia:"106",siglaProvincia:"VS",codiceIstatRegione:"20"},"carboniaiglesias":{nomeProvincia:"carbonia-iglesias",codiceIstatProvincia:"107",siglaProvincia:"CI",codiceIstatRegione:"20"},"forli":{refKey:"forl\u00eccesena"},"aosta":{refKey:"valledaosta"},"ascoli-piceno":{refKey:"ascolipiceno"},"iglesias":{refKey:"carboniaiglesias"},"l-aquila":{refKey:"laquila"},"la-spezia":{refKey:"laspezia"},"massa":{refKey:"massacarrara"},"monza":{refKey:"monzaedellabrianza"},"reggio-di-calabria":{refKey:"reggiodicalabria"},"reggio-nell-emilia":{refKey:"reggionellemilia"},"tempio-pausania":{refKey:"olbiatempio"},"tortoli":{refKey:"ogliastra"},"trani":{refKey:"barlettaandriatrani"},"urbino":{refKey:"pesarourbino"},"verbania":{refKey:"verbanocusioossola"},"vibo-valentia":{refKey:"vibovalentia"},"villacidro":{refKey:"mediocampidano"}};window.IOLOBJ.Profile.setupFromRC=function(options){try{var area=UTL.getDomainFromHostName(options.hostName);window.IOLOBJ.Profile[area]=window.IOLOBJ.Profile[area]||{};for(key in options){if(options[key]!='undefined'){window.IOLOBJ.Profile[area][key]=options[key];}else{if(typeof(window.IOLOBJ.Profile[area][key])!='undefined')
delete window.IOLOBJ.Profile[area][key];}}}catch(e){}}
window.IOLOBJ.Profile.RCWrite=function(options){window.IOLOBJ.Profile.setupFromRC(options);}
window.IOLOBJ.Profile.decodeGeoC=function(cValue){try{if(UTL.isEmpty(cValue))
return{};var obj={};var data=cValue.split("|");obj.returnCode=0;obj['version']=data[0];var extra=cValue.indexOf("=")>=0?1:0;if(obj['version']=='1.3'&&data.length>=13){if(data[1].substring(0,2)=='I1')
obj['I1']=data[1].substring(2+extra);else
obj.returnCode=-1;if(data[2].substring(0,2)=='T1')
obj['T1']=data[2].substring(2+extra);else
obj.returnCode=-1;if(data[3].substring(0,2)=='C1')
obj['C1']=data[3].substring(2+extra);else
obj.returnCode=-1;if(data[4].substring(0,3)=='LT1')
obj['LT1']=data[4].substring(3+extra);else
obj.returnCode=-1;if(data[5].substring(0,3)=='LN1')
obj['LN1']=data[5].substring(3+extra);else
obj.returnCode=-1;obj['ts']=data[6];obj['nUp']=data[7];if(data[8].substring(0,4)=='ACC1')
obj['ACC1']=data[8].substring(4+extra);else
obj.returnCode=-1;if(data[9].substring(0,4)=='GID1')
obj['GID1']=data[9].substring(4+extra);else
obj.returnCode=-1;if(data[10].substring(0,3)=='CN1')
obj['CN1']=data[10].substring(3+extra);else
obj.returnCode=-1;if(data[11].substring(0,3)=='TR1')
obj['TR1']=data[11].substring(3+extra);else
obj.returnCode=-1;if(data[12].substring(0,3)=='TP1')
obj['TP1']=data[12].substring(3+extra);else
obj.returnCode=-1;if(data[13].substring(0,4)=='TPA1')
obj['TPA1']=data[13].substring(4+extra);else
obj.returnCode=-1;}else{}
if(!UTL.isEmpty(obj['C1'])){var ss=obj['C1'];ss=ss.replace(/'/g,'');ss=ss.replace(/-/g,'');var provincia=window.IOLOBJ.LIBERIX.data.province[ss];if(provincia){if(provincia.refKey){provincia=window.IOLOBJ.LIBERIX.data.province[provincia.refKey];}
obj['lip']=provincia.codiceIstatProvincia;var regione=window.IOLOBJ.LIBERIX.data.regioni[provincia.codiceIstatRegione];if(regione){obj['gcrl']=regione.codiceLiberiX;}}}
return obj;}catch(e){return{};}}
window.IOLOBJ.Profile.decodeVLocalCityC=function(cValue){try{if(UTL.isEmpty(cValue))
return{};var data=cValue.replace(/\(/g,"=").replace(/\)/g,"").split("_");var tmp,ret={};for(var i=data.length-1;i>=0;i--){tmp=data[i].split("=");ret[tmp[0]]=tmp[1];}
ret.returnCode=0;return ret;}catch(e){return{};}}
window.IOLOBJ.Profile.decodeVLocalSC=function(cValue){try{if(UTL.isEmpty(cValue))
return{};var data=cValue.split("_");var tmp,ret={};for(var i=data.length-1;i>=0;i--){tmp=data[i].split("=");ret[tmp[0]]=tmp[1];}
ret.returnCode=0;return ret;}catch(e){return{};}}
window.IOLOBJ.Profile.decodeLibAdvCkC=function(cValue){try{if(UTL.isEmpty(cValue))
return{};var tmp=cValue.split("-");var ret={f:tmp[0],s:tmp[1],p:tmp[2]};return ret;}catch(e){return{};}}
window.IOLOBJ.Profile.decodeLibAdvECkC=function(cValue){try{if(UTL.isEmpty(cValue))
return{};var tmp=cValue.split("-");var t1=[];if(tmp[0].indexOf("|")>=0){t1=tmp[0].split("|");}else if(tmp[0].indexOf("%7C")>=0){t1=tmp[0].split("%7C");}else if(tmp[0].indexOf("%7c")>=0){t1=tmp[0].split("%7c");}
var ret={ts:t1[0],f:t1[1],s:tmp[2],p:tmp[3]};return ret;}catch(e){return{};}}
window.IOLOBJ.Profile.decodeLibAdvUCkC=function(cValue){try{if(UTL.isEmpty(cValue))
return{};var tmp;if(cValue.indexOf("|")>=0)
tmp=cValue.split("|");else if(cValue.indexOf("%3B")>=0)
tmp=cValue.split("%3B");else if(cValue.indexOf("%7C")>=0)
tmp=cValue.split("%7C");else
tmp=cValue.split(";");var ret={ts:tmp[2],dm:tmp[0],id:tmp[1]};return ret;}catch(e){return{};}}
window.IOLOBJ.Profile.setupFromGEO=function(cValue){try{window.IOLOBJ.Profile['geo']=window.IOLOBJ.Profile['geo']||{};window.IOLOBJ.Profile['geo'].geo=cValue;var x=cValue.indexOf("|ACC1=");var nValue="1.3|"+cValue.substring(0,x)+"|"+(new Date().getTime())+"|1"+cValue.substring(x);nValue=nValue.replace(/=/g,"");window.IOLOBJ.Profile['libero.it']=window.IOLOBJ.Profile['libero.it']||{};window.IOLOBJ.Profile['libero.it'].geo=nValue;if(UTL.getDomainFromHostName()=='libero.it'||UTL.getDomainFromHostName()=='virgilio.it'){var cValue="geo="+nValue+"; domain="+UTL.getDomainFromHostName()+"; path=/; expires="+UTL.getGMTString(90);document.cookie=cValue;}}catch(e){}}
window.IOLOBJ.Profile.setUPFromProfiles=function(){try{var libero=window.IOLOBJ.Profile['libero.it']||{};var virgilio=window.IOLOBJ.Profile['virgilio.it']||{};var cgeo=libero.geo||virgilio.geo;var geo=window.IOLOBJ.Profile.decodeGeoC(cgeo);var cv_local_city=libero.v_local_city||virgilio.v_local_city;var v_local_city=window.IOLOBJ.Profile.decodeVLocalCityC(cv_local_city);var cv_local_s=libero.v_local_s||virgilio.v_local_s;var v_local_s=window.IOLOBJ.Profile.decodeVLocalSC(cv_local_s);var clib_adv_eck=libero.lib_adv_eck||virgilio.lib_adv_eck;var lib_adv_eck=window.IOLOBJ.Profile.decodeLibAdvECkC(clib_adv_eck);var clib_adv_eck_l=libero.lib_adv_eck_l||virgilio.lib_adv_eck_l;var lib_adv_eck_l=window.IOLOBJ.Profile.decodeLibAdvECkC(clib_adv_eck_l);var clib_adv_eck_v=virgilio.lib_adv_eck_v||libero.lib_adv_eck_v;var lib_adv_eck_v=window.IOLOBJ.Profile.decodeLibAdvECkC(clib_adv_eck_v);var tsu=parseInt(lib_adv_eck.ts)||0;var tsl=parseInt(lib_adv_eck_l.ts)||0;var tsv=parseInt(lib_adv_eck_v.ts)||0;var tsf=parseInt(window.IOLOBJ.Profile.FUP.ts)||0;var clib_adv_ck=libero.lib_adv_ck||virgilio.lib_adv_ck;var lib_adv_ck=window.IOLOBJ.Profile.decodeLibAdvCkC(clib_adv_ck);var clib_adv_ck_l=libero.lib_adv_ck||virgilio.lib_adv_ck_l;var lib_adv_ck_l=window.IOLOBJ.Profile.decodeLibAdvCkC(clib_adv_ck_l);var clib_adv_ck_v=virgilio.lib_adv_ck||libero.lib_adv_ck_v;var lib_adv_ck_v=window.IOLOBJ.Profile.decodeLibAdvCkC(clib_adv_ck_v);if(tsl>tsu){lib_adv_ck=lib_adv_ck_l;clib_adv_ck=clib_adv_ck_l;lib_adv_eck=lib_adv_eck_l;clib_adv_eck=clib_adv_eck_l;}
if(tsv>tsl&&tsv>tsu){lib_adv_ck=lib_adv_ck_v;clib_adv_ck=clib_adv_ck_v;lib_adv_eck=lib_adv_eck_v;clib_adv_eck=clib_adv_eck_v;}
if(tsf>tsv&&tsf>tsl&&tsf>tsu){clib_adv_ck=window.IOLOBJ.Profile.FUP.sfe+"-"+window.IOLOBJ.Profile.FUP.sse+"-"+window.IOLOBJ.Profile.FUP.spr+"-0-0";lib_adv_ck=window.IOLOBJ.Profile.decodeLibAdvCkC(clib_adv_ck);clib_adv_eck=window.IOLOBJ.Profile.FUP.ts+"-"+lib_adv_ck;lib_adv_eck=window.IOLOBJ.Profile.decodeLibAdvECkC(clib_adv_eck);}
var clib_adv_uck=libero.lib_adv_uck||virgilio.lib_adv_uck;var lib_adv_uck=window.IOLOBJ.Profile.decodeLibAdvUCkC(clib_adv_uck);var clib_adv_uck_l=libero.lib_adv_uck||virgilio.lib_adv_uck_l;var lib_adv_uck_l=window.IOLOBJ.Profile.decodeLibAdvUCkC(clib_adv_uck_l);var clib_adv_uck_v=virgilio.lib_adv_uck||libero.lib_adv_uck_v;var lib_adv_uck_v=window.IOLOBJ.Profile.decodeLibAdvUCkC(clib_adv_uck_v);var tsu=parseInt(lib_adv_uck.ts)||0;var tsl=parseInt(lib_adv_uck_l.ts)||0;var tsv=parseInt(lib_adv_uck_v.ts)||0;var tsf=parseInt(window.IOLOBJ.Profile.FUP.tsi)||0;if(tsl>tsu){lib_adv_uck=lib_adv_uck_l;clib_adv_uck=clib_adv_uck_l;}
if(tsv>tsl&&tsv>tsu){lib_adv_uck=lib_adv_uck_v;clib_adv_uck=clib_adv_uck_v;}
if(tsf>tsv&&tsf>tsl&&tsf>tsu){clib_adv_uck=window.IOLOBJ.Profile.FUP.dm+";"+window.IOLOBJ.Profile.FUP.id+";"+window.IOLOBJ.Profile.FUP.tsi;lib_adv_uck=window.IOLOBJ.Profile.decodeLibAdvUCkC(clib_adv_uck);}
window.IOLOBJ.Profile.UP.returnCode=0;window.IOLOBJ.Profile.UP.gic=geo.T1||0;window.IOLOBJ.Profile.UP.gip=geo.TP1||0;window.IOLOBJ.Profile.UP.gis=geo.I1||0;window.IOLOBJ.Profile.UP.gcrl=geo.gcrl||0;window.IOLOBJ.Profile.UP.lip=v_local_city.idprovincia||0;window.IOLOBJ.Profile.UP.lic=v_local_city.idcity||0;window.IOLOBJ.Profile.UP.lir=v_local_city.regione||0;window.IOLOBJ.Profile.UP.sse=lib_adv_ck.s||window.IOLOBJ.Profile.FUP.sse||0;window.IOLOBJ.Profile.UP.sfe=lib_adv_ck.f||window.IOLOBJ.Profile.FUP.sfe||0;window.IOLOBJ.Profile.UP.spr=lib_adv_ck.p||window.IOLOBJ.Profile.FUP.spr||0;window.IOLOBJ.Profile.UP.g4=geo.TR1||0;window.IOLOBJ.Profile.UP.exp=(v_local_city.exp==='y'?1:0)||0;window.IOLOBJ.Profile.UP.ts=lib_adv_eck.ts||window.IOLOBJ.Profile.FUP.ts||0;window.IOLOBJ.Profile.UP.dm=lib_adv_uck.dm||window.IOLOBJ.Profile.FUP.dm||0;window.IOLOBJ.Profile.UP.id=lib_adv_uck.id||window.IOLOBJ.Profile.FUP.id||0;window.IOLOBJ.Profile.UP.tsi=lib_adv_uck.ts||window.IOLOBJ.Profile.FUP.tsi||0;window.IOLOBJ.Profile.UP.ext=window.IOLOBJ.Profile.FUP.ext||{};window.IOLOBJ.Profile.extendUPData();window.IOLOBJ.Profile.createFUP_SESS();window.IOLOBJ.Profile['eletti']={geo:cgeo,v_local_city:cv_local_city,lib_adv_ck:clib_adv_ck,lib_adv_eck:clib_adv_eck,lib_adv_uck:clib_adv_uck};}catch(e){}}
window.IOLOBJ.Profile.createFUP_SESS=function(){try{var domain=UTL.getDomainFromHostName();var sValue="gcrl="+window.IOLOBJ.Profile.UP.gcrl
+"|gic="+window.IOLOBJ.Profile.UP.gic
+"|gip="+window.IOLOBJ.Profile.UP.gip
+"|gis="+window.IOLOBJ.Profile.UP.gis
+"|lic="+window.IOLOBJ.Profile.UP.lic
+"|lip="+window.IOLOBJ.Profile.UP.lip
+"|lir="+window.IOLOBJ.Profile.UP.lir
+"|sfe="+window.IOLOBJ.Profile.UP.sfe
+"|spr="+window.IOLOBJ.Profile.UP.spr
+"|sse="+window.IOLOBJ.Profile.UP.sse
+"|g4="+window.IOLOBJ.Profile.UP.g4
+"|exp="+window.IOLOBJ.Profile.UP.exp
+"|ts="+window.IOLOBJ.Profile.UP.ts
+"|dm="+window.IOLOBJ.Profile.UP.dm
+"|id="+window.IOLOBJ.Profile.UP.id
+"|tsi="+window.IOLOBJ.Profile.UP.tsi
+"|ext="+UTL.cdf(JSON.stringify(window.IOLOBJ.Profile.UP.ext));var cValue="fup_sess="+sValue+"; domain="+domain+"; path=/; expires="+UTL.getGMTString(7);document.cookie=cValue;}catch(e){}}
window.IOLOBJ.Profile.setUPFromFUP_SESS=function(cValue){try{var data=cValue.split("|");var tmp;window.IOLOBJ.Profile.FUP={};for(var i=data.length-1;i>=0;i--){tmp=data[i].split("=");if(typeof(window.IOLOBJ.Profile.UP[tmp[0]])!='undefined'){if(tmp[0]=='ext'){try{window.IOLOBJ.Profile.UP[tmp[0]]=JSON.parse(UTL.dcdf(tmp[1]));window.IOLOBJ.Profile.FUP[tmp[0]]=window.IOLOBJ.Profile.UP[tmp[0]];}catch(e){}}else{window.IOLOBJ.Profile.UP[tmp[0]]=tmp[1];window.IOLOBJ.Profile.FUP[tmp[0]]=tmp[1];}}else{}}
window.IOLOBJ.Profile.UP.returnCode=0;window.IOLOBJ.Profile.extendUPData();}catch(e){return{};}}
window.IOLOBJ.Profile.extendUPData=function(){try{window.IOLOBJ.Profile.UP.ogic=window.IOLOBJ.Profile.UP.ogic||window.IOLOBJ.Profile.UP.gic;if((""+window.IOLOBJ.Profile.UP.gic).substring(0,3)=='900'){window.IOLOBJ.Profile.UP.ogic=window.IOLOBJ.Profile.UP.gic;window.IOLOBJ.Profile.UP.gic=window.IOLOBJ.LIBERIX.data.locTurRifByIstat[window.IOLOBJ.Profile.UP.gic]||window.IOLOBJ.Profile.UP.gic;}
window.IOLOBJ.Profile.UP.olic=window.IOLOBJ.Profile.UP.olic||window.IOLOBJ.Profile.UP.lic;if((""+window.IOLOBJ.Profile.UP.lic).substring(0,3)=='900'){window.IOLOBJ.Profile.UP.olic=window.IOLOBJ.Profile.UP.lic;window.IOLOBJ.Profile.UP.lic=window.IOLOBJ.LIBERIX.data.locTurRifByIstat[window.IOLOBJ.Profile.UP.lic]||window.IOLOBJ.Profile.UP.lic;}
window.IOLOBJ.Profile.UP.ispr=window.IOLOBJ.LIBERIX.data.mdecIstat[window.IOLOBJ.Profile.UP.spr]||0;}catch(e){}}
window.IOLOBJ.Profile.UP=window.IOLOBJ.Profile.UP||{returnCode:-1,gic:0,gip:0,gis:0,gcrl:0,lip:0,lic:0,lir:0,sse:0,sfe:0,spr:0,g4:0,exp:0,ts:0,dm:0,id:0,tsi:0,ext:{}};window.IOLOBJ.Profile.setExtData=function(obj){try{var cValue="whs="+UTL.cdf(JSON.stringify(obj))+"; domain="+UTL.getDomainFromHostName()+"; path=/; expires="+UTL.getGMTString(1);document.cookie=cValue;}catch(e){}
try{var tdata=obj.data[0]||{};if(tdata.status=="OK"||tdata.status=="OPTOUT"){if(tdata.md5==getFup().id){delete tdata.md5;delete tdata.status;tdata.ver=obj.ver;window.IOLOBJ.Profile.UP.ext=tdata;window.IOLOBJ.Profile.createFUP_SESS();}else{window.IOLOBJ.Profile.UP.ext={};}}else{window.IOLOBJ.Profile.UP.ext={};}}catch(e){return{};}}
window.IOLOBJ.Profile.getRemoteExtData=function(){try{var dmn=['','lb','vg'];var fo=getFup();var tmpk=fo.dm+"-"+fo.id;if(window.___tmpk!==tmpk){window.___tmpk=tmpk;}else{return false;}
try{var whs=JSON.parse(UTL.dcdf(UTL.getCookie("whs")));if(whs.data[0].md5==fo.id){window.IOLOBJ.Profile.setExtData(whs);return true;}}catch(e){}
UTL.loadScript({url:'http://iolps.iol.it/'+dmn[parseInt(fo.dm)]+'/getUserProfile?q='+fo.id+'&jsonpCallback=window.IOLOBJ.Profile.setExtData'});return true;}catch(e){return false;}}
window.getFup=function(){return window.IOLOBJ.Profile.UP;};if(window.parent!=window){try{if(UTL.isEmpty(window.parent.window.IOLOBJ)){window.parent.getFup=window.getFup;window.parent.window.IOLOBJ=window.IOLOBJ;}else{window.getFup=window.parent.getFup;window.IOLOBJ=window.parent.window.IOLOBJ;}}catch(e){}}
if(!window.IOLOBJ.IS_LOADED){window.IOLOBJ.IS_LOADED=true;(function(){var cookieToQs=function(geo,lib_adv_ck,lib_adv_eck,lib_adv_uck){var ret=[];if(geo!='')
ret[ret.length]="g=["+geo+"]g";if(lib_adv_ck!='')
ret[ret.length]="vs=["+lib_adv_ck+"]vs";if(lib_adv_eck!='')
ret[ret.length]="evs=["+lib_adv_eck+"]evs";if(lib_adv_uck!='')
ret[ret.length]="uvs=["+lib_adv_uck+"]uvs";return ret.join("&");}
var domain=UTL.getDomainFromHostName();var refDom;if(window.parent!=window){try{refDom=window.parent.document.referrer;}catch(e){refDom=window.document.referrer;}}else{refDom=window.document.referrer;}
refDom=refDom.split("/")[2]||".";refDom=refDom.split(".");refDom=refDom[refDom.length-2]+"."+refDom[refDom.length-1];if(domain=='libero.it'){var cValue=UTL.getCookie("FUP_SESS")||null;if(cValue!=null&&cValue!=-1){window.IOLOBJ.Profile.setUPFromFUP_SESS(cValue);}
window.IOLOBJ.Profile.setupFromRC({hostName:'libero.it',geo:UTL.getCookie("geo"),v_local_city:UTL.getCookie("vlocal_city"),lib_adv_ck:UTL.getCookie("lib_adv_ck"),lib_adv_ck_l:UTL.getCookie("lib_adv_ck_l"),lib_adv_ck_v:UTL.getCookie("lib_adv_ck_v"),v_local_s:UTL.getCookie("vlocal_s"),lib_adv_eck:UTL.getCookie("lib_adv_eck"),lib_adv_eck_l:UTL.getCookie("lib_adv_eck_l"),lib_adv_eck_v:UTL.getCookie("lib_adv_eck_v"),lib_adv_uck:UTL.getCookie("lib_adv_uck"),lib_adv_uck_l:UTL.getCookie("lib_adv_uck_l"),lib_adv_uck_v:UTL.getCookie("lib_adv_uck_v")});window.IOLOBJ.Profile.setUPFromProfiles();if(UTL.isEmptyObj(window.IOLOBJ.Profile['eletti'].geo)){UTL.loadScript({url:"http://geoisp.libero.it/ioladv/iolobj-geo.php?callback=window.IOLOBJ.Profile.setupFromGEO&tm="+(new Date()).getTime(),callBack:function(){window.IOLOBJ.Profile.setUPFromProfiles();var geo=window.IOLOBJ.Profile['eletti'].geo;UTL.loadScript({url:"http://geoisp.virgilio.it/ioladv/iolobj-rc-write.js?tm="+(new Date()).getTime()+"&g=["+geo+"]g&d=[virgilio.it]d",callBack:function(){var lib_adv_ck=window.IOLOBJ.Profile['virgilio.it'].lib_adv_ck||"";var lib_adv_eck=window.IOLOBJ.Profile['virgilio.it'].lib_adv_eck||"";var lib_adv_uck=window.IOLOBJ.Profile['virgilio.it'].lib_adv_uck||"";if(lib_adv_ck!=""){var cValue="LIB_ADV_CK_V="+lib_adv_ck+"; domain=libero.it; path=/; expires="+UTL.getGMTString(90);document.cookie=cValue;}
if(lib_adv_eck!=""){var cValue="LIB_ADV_ECK_V="+lib_adv_eck+"; domain=libero.it; path=/; expires="+UTL.getGMTString(90);document.cookie=cValue;}
if(lib_adv_uck!=""){var cValue="LIB_ADV_UCK_V="+lib_adv_uck+"; domain=libero.it; path=/; expires="+UTL.getGMTString(90);document.cookie=cValue;}
window.IOLOBJ.Profile.setUPFromProfiles();var ft=getFup();if(ft['id']!=0){window.IOLOBJ.Profile.getRemoteExtData();}}});UTL.loadScript({url:"http://geoisp.matrixspa.it/ioladv/iolobj-rc-write.js?tm="+(new Date()).getTime()+"&g=["+geo+"]g&d=[matrixspa.it]d"});}});}else{if(domain!=refDom||cValue==null||cValue==-1){UTL.loadScript({url:"http://geoisp.virgilio.it/ioladv/iolobj-rc-read.js?tm="+(new Date()).getTime(),callBack:function(){var lib_adv_ck=window.IOLOBJ.Profile['virgilio.it'].lib_adv_ck||"";var lib_adv_eck=window.IOLOBJ.Profile['virgilio.it'].lib_adv_eck||"";var lib_adv_uck=window.IOLOBJ.Profile['virgilio.it'].lib_adv_uck||"";if(lib_adv_ck!=""){var cValue="LIB_ADV_CK_V="+lib_adv_ck+"; domain=libero.it; path=/; expires="+UTL.getGMTString(90);document.cookie=cValue;}
if(lib_adv_eck!=""){var cValue="LIB_ADV_ECK_V="+lib_adv_eck+"; domain=libero.it; path=/; expires="+UTL.getGMTString(90);document.cookie=cValue;}
if(lib_adv_uck!=""){var cValue="LIB_ADV_UCK_V="+lib_adv_uck+"; domain=libero.it; path=/; expires="+UTL.getGMTString(90);document.cookie=cValue;}
window.IOLOBJ.Profile.setUPFromProfiles();var ft=getFup();if(ft['id']!=0)window.IOLOBJ.Profile.getRemoteExtData();}});}}}else{var cValue=UTL.getCookie("FUP_SESS")||null;if(cValue!=null&&cValue!=-1){window.IOLOBJ.Profile.setUPFromFUP_SESS(cValue);}
if(domain=='virgilio.it'){window.IOLOBJ.Profile.setupFromRC({hostName:'virgilio.it',geo:UTL.getCookie("geo"),v_local_city:UTL.getCookie("vlocal_city"),lib_adv_ck:UTL.getCookie("lib_adv_ck"),lib_adv_ck_l:UTL.getCookie("lib_adv_ck_l"),lib_adv_ck_v:UTL.getCookie("lib_adv_ck_v"),v_local_s:UTL.getCookie("vlocal_s"),lib_adv_eck:UTL.getCookie("lib_adv_eck"),lib_adv_eck_l:UTL.getCookie("lib_adv_eck_l"),lib_adv_eck_v:UTL.getCookie("lib_adv_eck_v"),lib_adv_uck:UTL.getCookie("lib_adv_uck"),lib_adv_uck_l:UTL.getCookie("lib_adv_uck_l"),lib_adv_uck_v:UTL.getCookie("lib_adv_uck_v")});window.IOLOBJ.Profile.setUPFromProfiles();}
if(domain!=refDom||cValue==null||cValue==-1){if(domain=='virgilio.it'){var lib_adv_ck=window.IOLOBJ.Profile['virgilio.it'].lib_adv_ck||"";var lib_adv_eck=window.IOLOBJ.Profile['virgilio.it'].lib_adv_eck||"";var lib_adv_uck=window.IOLOBJ.Profile['virgilio.it'].lib_adv_uck||"";UTL.loadScript({url:"http://geoisp.libero.it/ioladv/iolobj-rc-write.js?tm="+(new Date()).getTime()+"&"+cookieToQs("",lib_adv_ck,lib_adv_eck,lib_adv_uck)+"&d=[libero.it]d",callBack:function(){window.IOLOBJ.Profile.setUPFromProfiles();var lib_adv_ck=window.IOLOBJ.Profile['libero.it'].lib_adv_ck||"";var lib_adv_eck=window.IOLOBJ.Profile['libero.it'].lib_adv_eck||"";var lib_adv_uck=window.IOLOBJ.Profile['libero.it'].lib_adv_uck||"";if(lib_adv_ck!=""){var cValue="LIB_ADV_CK_L="+lib_adv_ck+"; domain=virgilio.it; path=/; expires="+UTL.getGMTString(90);document.cookie=cValue;}
if(lib_adv_eck!=""){var cValue="LIB_ADV_ECK_L="+lib_adv_eck+"; domain=virgilio.it; path=/; expires="+UTL.getGMTString(90);document.cookie=cValue;}
if(lib_adv_uck!=""){var cValue="LIB_ADV_UCK_L="+lib_adv_uck+"; domain=virgilio.it; path=/; expires="+UTL.getGMTString(90);document.cookie=cValue;}
if(UTL.isEmptyObj(window.IOLOBJ.Profile['eletti'].geo)){UTL.loadScript({url:"http://geoisp.virgilio.it/ioladv/iolobj-geo.php?callback=window.IOLOBJ.Profile.setupFromGEO&tm="+(new Date()).getTime(),callBack:function(){window.IOLOBJ.Profile.setUPFromProfiles();var geo=window.IOLOBJ.Profile['eletti'].geo;UTL.loadScript({url:"http://geoisp.libero.it/ioladv/iolobj-rc-write.js?tm="+(new Date()).getTime()+"&g=["+geo+"]g&d=[libero.it]d",callBack:function(){var lib_adv_ck=window.IOLOBJ.Profile['libero.it'].lib_adv_ck||"";var lib_adv_eck=window.IOLOBJ.Profile['libero.it'].lib_adv_eck||"";var lib_adv_uck=window.IOLOBJ.Profile['libero.it'].lib_adv_uck||"";if(lib_adv_ck!=""){var cValue="LIB_ADV_CK_L="+lib_adv_ck+"; domain=virgilio.it; path=/; expires="+UTL.getGMTString(90);document.cookie=cValue;}
if(lib_adv_eck!=""){var cValue="LIB_ADV_ECK_L="+lib_adv_eck+"; domain=virgilio.it; path=/; expires="+UTL.getGMTString(90);document.cookie=cValue;}
if(lib_adv_uck!=""){var cValue="LIB_ADV_UCK_L="+lib_adv_uck+"; domain=virgilio.it; path=/; expires="+UTL.getGMTString(90);document.cookie=cValue;}
window.IOLOBJ.Profile.setUPFromProfiles();var ft=getFup();if(ft['id']!=0)window.IOLOBJ.Profile.getRemoteExtData();}});UTL.loadScript({url:"http://geoisp.matrixspa.it/ioladv/iolobj-rc-write.js?tm="+(new Date()).getTime()+"&g=["+geo+"]g&d=[matrixspa.it]d"});}});}else{var ft=getFup();if(ft['id']!=0)
window.IOLOBJ.Profile.getRemoteExtData();}}});}else{UTL.loadScript({url:"http://geoisp.libero.it/ioladv/iolobj-rc-read.js?tm="+(new Date()).getTime(),callBack:function(){window.IOLOBJ.Profile.setUPFromProfiles();if(UTL.isEmptyObj(window.IOLOBJ.Profile['eletti'].geo)){UTL.loadScript({url:"http://geoisp.libero.it/ioladv/iolobj-geo.php?callback=window.IOLOBJ.Profile.setupFromGEO&tm="+(new Date()).getTime(),callBack:function(){window.IOLOBJ.Profile.setUPFromProfiles();var geo=window.IOLOBJ.Profile['eletti'].geo;UTL.loadScript({url:"http://geoisp.virgilio.it/ioladv/iolobj-rc-write.js?tm="+(new Date()).getTime()+"&g=["+geo+"]g&d=[virgilio.it]d",callBack:function(){window.IOLOBJ.Profile.setUPFromProfiles();var virgilio=window.IOLOBJ.Profile['virgilio.it']||{};lib_adv_ck=virgilio.lib_adv_ck||"";lib_adv_eck=virgilio.lib_adv_eck||"";lib_adv_uck=virgilio.lib_adv_uck||"";UTL.loadScript({url:"http://geoisp.libero.it/ioladv/iolobj-rc-write.js?tm="+(new Date()).getTime()+"&"+cookieToQs(geo,lib_adv_ck,lib_adv_eck,lib_adv_uck)+"&d=[libero.it]d"});UTL.loadScript({url:"http://geoisp.matrixspa.it/ioladv/iolobj-rc-write.js?tm="+(new Date()).getTime()+"&g=["+geo+"]g&d=[matrixspa.it]d"});var ft=getFup();if(ft['id']!=0)window.IOLOBJ.Profile.getRemoteExtData();}});}});}else{UTL.loadScript({url:"http://geoisp.virgilio.it/ioladv/iolobj-rc-read.js?tm="+(new Date()).getTime(),callBack:function(){window.IOLOBJ.Profile.setUPFromProfiles();var virgilio=window.IOLOBJ.Profile['virgilio.it']||{};lib_adv_ck=virgilio.lib_adv_ck||"";lib_adv_eck=virgilio.lib_adv_eck||"";lib_adv_uck=virgilio.lib_adv_uck||"";UTL.loadScript({url:"http://geoisp.libero.it/ioladv/iolobj-rc-write.js?tm="+(new Date()).getTime()+"&"+cookieToQs("",lib_adv_ck,lib_adv_eck,lib_adv_uck)+"&d=[libero.it]d",callBack:function(){var ft=getFup();if(ft['id']!=0)window.IOLOBJ.Profile.getRemoteExtData();}});}});}}});}}}
if(getFup().id!=0){window.IOLOBJ.Profile.getRemoteExtData();}})();}
// UP-LIB END

// ADV START
IOLAdv={};
IOLAdv.Adv={};
IOLAdv.Adv.advlib={version:'4.0'};
IOLAdv.Adv.advlib.adv_sso=new Array('0','0','0','0','0');
IOLAdv.Adv.advlib.ar_region=new Array();
IOLAdv.Adv.advlib.adv_geo=new Array('0','0','0','0','0','0','0','0','0','0','0','0','0','0');
var adv_latency=604800;
var adv_fup;
var adv_getfup_exist = false;
var adv_new_getfup_exist = false;


if (typeof getFup == 'function' ) {
adv_fup=getFup();
adv_new_getfup_exist = true;
var adv_ts_now = Math.round((new Date()).getTime() / 1000);
var adv_ts_limit = adv_ts_now-adv_latency;
var adv_ts_co = parseInt(adv_fup.ts);
if (adv_ts_co>adv_ts_limit) adv_getfup_exist = true;
}

var adv_profile=0; 
if ((typeof iol != 'undefined')&&(typeof iol.iolcookcs != 'undefined')){ 
if (!(iol.iolcookcs.isCookieEnable('Advertising'))){ adv_profile=1; } 
}


IOLAdv.Adv.advlib.adv_readCookie=function(ckname) {
	var cookieKey = ckname + "=";
	var cookieArray = document.cookie.split(';');
	for(var i=0;i < cookieArray.length;i++) {
		var c = cookieArray[i];
		while (c.charAt(0)==' ') c = c.substring(1,c.length);
		if (c.indexOf(cookieKey) == 0) {
			return c.substring(cookieKey.length,c.length);
		}
	}
	return '';
}

//Gestione Opt-Out Mail 
var adv_prof_cookie = IOLAdv.Adv.advlib.adv_readCookie("pc_liberoOff");
if (adv_prof_cookie == "1"){adv_profile=1;}
//Gestione Opt-Out Mail

function jiot_tgt(){
	if (typeof(adv_jiot_on)!="undefined" && adv_jiot_on=="yes") {
		var adv_crtg_content=unescape(IOLAdv.Adv.advlib.adv_readCookie(jiot_cookiename));
		if ((adv_crtg_content.length>200)||(adv_crtg_content.length=0)) {
			document.cookie=jiot_cookiename+'=1;expires=Thu, 01-Jan-70 00:00:01 GMT;';
		} 
		else {
			adv_arrayCr=adv_crtg_content.split(';');
			for(var i=0;i<adv_arrayCr.length;i++){
				var myregexp = /^(j|jb)[0-9]+_[0-9]+=[0-9]+$/;
				if (myregexp.test(adv_arrayCr[i]) == true){
					adv_arrayCr1=adv_arrayCr[i].split('=');
					if (adv_arrayCr1.length==2) {
						googletag.pubads().setTargeting(adv_arrayCr1[0],adv_arrayCr1[1]);
					}
				}
			}
		}
	}
}

IOLAdv.Adv.advlib.adv_getSSO=function(ssostring,ssoarray) {
	if (ssostring!='') {
		var splitted = ssostring.split("-");
		for(var j=0;j < splitted.length;j++) {
			if (splitted[j]=='undefined'||splitted[j]==''){
				ssoarray[j]='0';
			}
			else {
				ssoarray[j]=splitted[j];
			}
		}
	}
}

IOLAdv.Adv.advlib.adv_CookieExist=function(ckname) {
	var cookieKey = ckname + "=";
	var cookieArray = document.cookie.split(';');
	for(var i=0;i < cookieArray.length;i++) {
		var c = cookieArray[i];
		while (c.charAt(0)==' ') c = c.substring(1,c.length);
		if (c.indexOf(cookieKey) == 0) {
			return true;
		}
	}
	return false;
}

IOLAdv.Adv.advlib.adv_isNumber=function(n) {
	return !isNaN(parseFloat(n)) && isFinite(n);
}

IOLAdv.Adv.advlib.adv_getGeo=function(geostring,geoarray) {
    if (geostring!='') {
        var splitted=geostring.split("|");
        for(var j=0;j<splitted.length;j++)if(splitted[j]!='')geoarray[j]=splitted[j];
    }
}

// ----- lprof start
var iol = iol || {};
 
iol.dmp={
policyCookieName:"ioldmpprof",
policyCookieTime:"ioldmpprofTimer",
 
log : function (l){
                try {
                               if (document.location.href.indexOf("log=true")>0)
                                               console.log(l);
                } catch(ex) {
                }
},
 
createCookie : function (name,value,days) {
    if (days) {
        var date = new Date();
        date.setTime(date.getTime()+(days*24*60*60*1000));
        var expires = "; expires="+date.toGMTString();
    }
    else
                               var expires = "";
    document.cookie = name+ "=" +value+expires+ "; path=/";
},
 
check : function (){
                try{
                               this.log("start check 1.1");
                               if (this.readDmpProfileTimer()==null){
                                               this.log("get audience");
                                               this.getAudiences();
                               }else{
                                               this.log("cookie already present");
                               }
                }catch(ex){}
},
 
readDmpProfileJs : function () {
                try{
                               var v = this.readCookie(this.policyCookieName);
                               return '"'+ v.replace(/,/g,'", "' ) +'"';
                }catch(ex){}
},
 
readDmpProfile : function () {
                return this.readCookie(this.policyCookieName);
},
readDmpProfileTimer : function () {
                return this.readCookie(this.policyCookieTime);
},
 
 
// legge il contenuto di un generico cookie
readCookie : function (name) {
                var nameEQ = name + "=";
                var ca = document.cookie.split(';');
 
                for( var i=0; i < ca.length; i++ ) {   
                               var c = ca[i];       
                               while (c.charAt(0)==' ') {
                                               c = c.substring(1,c.length);
                               }
                               if (c.indexOf(nameEQ) == 0) {    
                                               return c.substring(nameEQ.length,c.length);
                               }
                }
                return null;
},
get_cc_extr_url : function () {
                var domain = '.crwdcntrl.net';
                //
                // Look for a domain cookie first
                //
                var noDomainCookie = true;
                var start = document.cookie.indexOf('_cc_domain');
                if (start > -1) {
                               var valStartDc = document.cookie.indexOf('=', start);
                               if (valStartDc > 0) {
                                               noDomainCookie = false;
                                               valStartDc++;
                                               var valEndDc = document.cookie.indexOf(';', valStartDc);
                                               valEndDc = valEndDc > 0 ? valEndDc : document.cookie.length;
                                               domain = document.cookie.slice(valStartDc, valEndDc);
                               }
                }
                //
                // If there was no domain cookie then check to see if there is an id cookie.
                //
                var idCookie = false;
                if (noDomainCookie) {
                               var id;
                               start = document.cookie.indexOf('_cc_id');
                               if (start > -1) {
                                               var valStartId = document.cookie.indexOf('=', start);
                                               if (valStartId > 0) {
                                                               idCookie = true;
                                                               valStartId++;
                                                               var valEndId = document.cookie.indexOf(';', valStartId);
                                                               valEndId = valEndId > 0 ? valEndId : document.cookie.length;
                                                               id = document.cookie.slice(valStartId, valEndId);
                                               }
                               }
                }
                var cc_url = 'http://ad' + domain;
                if (typeof(portNumber) != 'undefined' && portNumber != null) {
                               cc_url = cc_url + ':' + portNumber;
                }
                cc_url = cc_url + '/5/c=6199/pe=y/callback=iol.dmp.getProfile';
                if(idCookie) {
                               cc_url = cc_url + '/pid=' + id;
                }
                return cc_url;
}
,
getProfile : function (ccauds){
 
                var dartCC="";
                if (typeof(ccauds) != 'undefined')
                {
                               for (var cci = 0; cci < ccauds.Profile.Audiences.Audience.length; cci++)
                               {
                                               if (cci > 0)
                                                               dartCC += ",";
                                               dartCC += ccauds.Profile.Audiences.Audience[cci].id;
                               }
                }
                this.log("."+dartCC);
                if (dartCC!=""){
                               this.createCookie(this.policyCookieName, dartCC, 360);
                               this.createCookie(this.policyCookieTime, dartCC, 1);
                }else
                               this.log("not saved");
}
,
getAudiences : function () {
                try{
                               var url = this.get_cc_extr_url();
                               var tempScriptTag = document.createElement('script');
                               tempScriptTag.type = 'text/javascript';
                               tempScriptTag.src = url;
                               window.setTimeout(function(){
                                               try{
                                                               document.body.appendChild(tempScriptTag);
                                               }catch(ex){}} , 3000);
                }catch(ex){}
}
 
};	
// ----- lprof end

IOLAdv.Adv.advlib.setTarget=function(googletag, iso) {
	var adv_tmp='';
	
	if(typeof(googletag) === 'undefined') {return;}

        if(adv_profile==0){googletag.pubads().setTargeting("optout","0");} 
        if(adv_profile==1){googletag.pubads().setTargeting("optout","1");} //conteggio Opt-Out
	
	iso = typeof iso !== 'undefined' ? iso : 1;
	
	if (iso === 1) {googletag.pubads().setTargeting("oe","iso-8859-1");}
	
	 if (adv_new_getfup_exist) {
         if(typeof(adv_fup.ext.uos)!="undefined") { googletag.pubads().setTargeting("uos",adv_fup.ext.uos); }
         if(typeof(adv_fup.ext.uab)!="undefined") { googletag.pubads().setTargeting("uab",adv_fup.ext.uab); }
         if(typeof(adv_fup.ext.umb)!="undefined") { googletag.pubads().setTargeting("umb",adv_fup.ext.umb); }
         if(typeof(adv_fup.ext.udb)!="undefined") { googletag.pubads().setTargeting("udb",adv_fup.ext.udb); }
         if(typeof(adv_fup.ext.qa_livello)!="undefined") { googletag.pubads().setTargeting("qlt",adv_fup.ext.qa_livello); }
         if(typeof(adv_fup.ext.istat)!="undefined") { googletag.pubads().setTargeting("istat",adv_fup.ext.istat); }
	}



        if (adv_getfup_exist) {
	   googletag.pubads().setTargeting("adv_sso1",adv_fup.sfe);
	   googletag.pubads().setTargeting("adv_sso2",adv_fup.sse);
	   googletag.pubads().setTargeting("adv_sso3",adv_fup.spr);
	}
	
	else
	
	{
	   if ((adv_tmp=IOLAdv.Adv.advlib.adv_readCookie('LIB_ADV_CK'))!=''){
		   IOLAdv.Adv.advlib.adv_getSSO(adv_tmp,IOLAdv.Adv.advlib.adv_sso);
	   }
	   for(var i=0;i < 3;i++) {
	    	if ((typeof(IOLAdv.Adv.advlib.adv_sso[i])!="undefined") && (IOLAdv.Adv.advlib.adv_isNumber(IOLAdv.Adv.advlib.adv_sso[i]))) {
		    	var j=i+1;
			    googletag.pubads().setTargeting("adv_sso"+j,IOLAdv.Adv.advlib.adv_sso[i]);
		   }
	   }
	}
	
	
	
	if (adv_getfup_exist)
	{
	 googletag.pubads().setTargeting("region",adv_fup.gcrl);
	}
	
	else 
	
	{	
	  adv_region=unescape(IOLAdv.Adv.advlib.adv_readCookie('LIB_ADV_G'));
	  if (IOLAdv.Adv.advlib.adv_CookieExist('LIB_ADV_G')==true && typeof(adv_region)!="undefined" ){
  		IOLAdv.Adv.advlib.ar_region=adv_region.split("|");
    	}
    	if ((typeof(IOLAdv.Adv.advlib.ar_region[2])!="undefined") && (IOLAdv.Adv.advlib.adv_isNumber(IOLAdv.Adv.advlib.ar_region[2]))) {
		googletag.pubads().setTargeting("region",IOLAdv.Adv.advlib.ar_region[2]);
	  }
	}  
	
	
//trk START
 try {
       if (typeof(iol.iolcookcs.trackCookie) == "function"){
         var trap_url="adg1";
         var adv_tc_exist = IOLAdv.Adv.advlib.adv_readCookie("trkuser");
         if (adv_tc_exist == "old"){googletag.pubads().setTargeting("trkusr","old");}
         if (adv_tc_exist == "new"){googletag.pubads().setTargeting("trkusr","new");}
         if (adv_tc_exist === ""){
            var adv_c_check = IOLAdv.Adv.advlib.adv_readCookie("policy_cookie");
            if ((adv_c_check === "")&&(window.location.href.indexOf(trap_url) != "-1")) {iol.iolcookcs.trackCookie("new"); googletag.pubads().setTargeting("trkusr","new");}
            else {if(window.location.href.indexOf(trap_url) != "-1"){iol.iolcookcs.trackCookie("old"); googletag.pubads().setTargeting("trkusr","old");}}
         }
       }
    } catch(e){}
//trk END
	
	
	
	if (typeof(adv_wmopt)!="undefined" && adv_wmopt!=''){
		googletag.pubads().setTargeting("adv_wmopt",adv_wmopt);
	}
	if (IOLAdv.Adv.advlib.adv_readCookie('ADV_P')=='M'){
		googletag.pubads().setTargeting("adv_prt","mcd");
	}
	if (IOLAdv.Adv.advlib.adv_CookieExist('ADV_NP')==false){
		googletag.pubads().setTargeting("adv_np","yes");
	}
	
	//criteo
	if (typeof(adv_cr_on)!="undefined" && adv_cr_on=="yes") {
    var adv_crtg_content=unescape(IOLAdv.Adv.advlib.adv_readCookie('cto_rta'));
    if (adv_crtg_content.length>200) {
    	document.cookie='cto_rta=1;expires=Thu, 01-Jan-70 00:00:01 GMT;';
    } 
    else {
      adv_arrayCr=adv_crtg_content.split(';');
      for(var i=0;i<adv_arrayCr.length;i++){
	      var myregexp = /^(ctxt|c300250|c160600|c180150|c336280|c46860|c72890|c300600|c301251|c97090|c970250)[A-Za-z0-9]*=1$/;
	      if (myregexp.test(adv_arrayCr[i]) == true){
	    	 adv_arrayCr1=adv_arrayCr[i].split('=');
	    	 if (adv_arrayCr1.length==2) {googletag.pubads().setTargeting(adv_arrayCr1[0],adv_arrayCr1[1]);}
	      }
      }
    }
	}
	
	adv_LIB_ADV_D=unescape(IOLAdv.Adv.advlib.adv_readCookie('LIB_ADV_D'));
	var adv_cls=0;
	if (typeof(adv_LIB_ADV_D) != "undefined" && adv_LIB_ADV_D!="") {
		var adv_timestamp=new Date().getTime()/1000;
		adv_LIB_ADV_D2=adv_LIB_ADV_D.split("|");
		if (typeof(adv_LIB_ADV_D2[1]) != "undefined" && adv_LIB_ADV_D2[1]!="") {
			adv_LIB_ADV_D3=adv_LIB_ADV_D2[1].split(":");
			if (typeof(adv_LIB_ADV_D3[1]) != "undefined" && adv_LIB_ADV_D3[1]!="") {
				googletag.pubads().setTargeting("adv_clv",adv_LIB_ADV_D3[1]);
				if (typeof(adv_LIB_ADV_D3[2]) != "undefined" && adv_LIB_ADV_D3[2]!="") {
					if (Math.abs(adv_timestamp - adv_LIB_ADV_D3[2]) < 2592000 ) adv_cls=1;
				}
			}
		}
		if (typeof(adv_LIB_ADV_D2[2]) != "undefined" && adv_LIB_ADV_D2[2]!="") {
			adv_LIB_ADV_D3=adv_LIB_ADV_D2[2].split(":");
			if (typeof(adv_LIB_ADV_D3[1]) != "undefined" && adv_LIB_ADV_D3[1]!="") {
				googletag.pubads().setTargeting("adv_clc",adv_LIB_ADV_D3[1]);
				if (typeof(adv_LIB_ADV_D3[2]) != "undefined" && adv_LIB_ADV_D3[2]!="") {
					if (Math.abs(adv_timestamp - adv_LIB_ADV_D3[2]) < 2592000 ) adv_cls=2;
				}
			}
		}
		googletag.pubads().setTargeting("adv_cls",adv_cls.toString());
	}
	
	var adv_tmp='';
	if ((adv_tmp=IOLAdv.Adv.advlib.adv_readCookie('geo'))!=''){
	    IOLAdv.Adv.advlib.adv_getGeo(adv_tmp,IOLAdv.Adv.advlib.adv_geo);
	}
	
 if (adv_getfup_exist) {
  
    if (adv_fup.exp=="1") { googletag.pubads().setTargeting("supergeo",adv_fup.lip); }
    
    else { if (adv_fup.ispr!="0")  googletag.pubads().setTargeting("supergeo",adv_fup.ispr); }

   }

	
  if (adv_getfup_exist) {
  
   googletag.pubads().setTargeting("adv_geo1",adv_fup.gip);
   googletag.pubads().setTargeting("adv_geo2",adv_fup.gic);
   googletag.pubads().setTargeting("adv_geo3",adv_fup.gis);
   googletag.pubads().setTargeting("adv_geo4",adv_fup.g4);

  }
  else 	
	  {
	  
	if (!adv_getfup_exist) { if(IOLAdv.Adv.advlib.adv_geo[12].substring(3)!=""){googletag.pubads().setTargeting("supergeo",IOLAdv.Adv.advlib.adv_geo[12].substring(3).replace(/=/,""));} } 
	//if(IOLAdv.Adv.advlib.adv_geo[13].substring(4)!=""){googletag.pubads().setTargeting("adv_geo1",IOLAdv.Adv.advlib.adv_geo[13].substring(4).replace(/=/,""));}
	if(IOLAdv.Adv.advlib.adv_geo[12].substring(3)!=""){googletag.pubads().setTargeting("adv_geo1",IOLAdv.Adv.advlib.adv_geo[12].substring(3).replace(/=/,""));}
	if(IOLAdv.Adv.advlib.adv_geo[2].substring(2)!=""){googletag.pubads().setTargeting("adv_geo2",IOLAdv.Adv.advlib.adv_geo[2].substring(2).replace(/=/,""));}
	if(IOLAdv.Adv.advlib.adv_geo[1].substring(2)!=""){googletag.pubads().setTargeting("adv_geo3",IOLAdv.Adv.advlib.adv_geo[1].substring(2).replace(/=/,""));}
	if(IOLAdv.Adv.advlib.adv_geo[11].substring(3)!=""){googletag.pubads().setTargeting("adv_geo4",IOLAdv.Adv.advlib.adv_geo[11].substring(3).replace(/=/,""));}
	  }


if (typeof iol.dmp.check != 'undefined') {iol.dmp.check();}
if (typeof iol.dmp.readDmpProfile != 'undefined') { 
   var adv_lauds = iol.dmp.readDmpProfile();
   adv_lauds_str=String(adv_lauds);
   adv_lauds_str=adv_lauds_str.split(",");
   googletag.pubads().setTargeting("laud", adv_lauds_str); 
   }

	
}

function iolAdjustMasthead() {
        try {
              if (typeof jQuery != 'undefined') {
                var args = jQuery.extend({
                        id: 'libero_header_adv',
                        height: 250,
                        zindex: 100000
                }, arguments[0]);
                jQuery('#' + args.id).css({
                  'margin': 'auto',
                  'position': 'fixed',
                  'width': '100%',
                  'z-index': args.zindex
                });
                jQuery('#' + args.id).after('<div style="height: ' + args.height + 'px"></dv>');                
              } 
            }
        catch(e) {}
}

IOLAdv.Adv.advlib.AmzCallback = function() {
  if (typeof amznads != 'undefined') {
          googletag.pubads().clearTargeting('amznslots');
 	  if (document.domain.search("totaltech") == -1) {
		  amznads.setTargetingForGPTAsync('amznslots');
	  } else {
		  amznads.setTargetingForGPTSync('amznslots');
	  }
	}
}

IOLAdv.Adv.advlib.refreshAllAds=function(slots){
	var elements = document.getElementsByTagName('div');
	var pattern = /^scr_.*(remotediv|expanding_300x250_|check).*/;
	var divtorm=[];
	var divcount=0;
        //Amzn START       
         if (typeof amznads !== 'undefined') {       
           if (typeof(amznads.getAdsCallback) == 'function') {
             try {                  
                   amznads.getAdsCallback('3204', IOLAdv.Adv.advlib.AmzCallback);
                 } catch (e) { /* ignore */ }
           }
          }
        //Amzn END
	for(var i=0; i<elements.length; i++) {
		if (elements[i].id == '') continue;
		var item = elements[i].id;
		if  (pattern.test(item)){
			divtorm[divcount] = document.getElementById(item);
			divcount++;
		}
	}
	for(var j=0; j<divtorm.length; j++) {
		divtorm[j].parentNode.removeChild(divtorm[j]);
	}

	var adv_scrjstoflashDIV_id = document.getElementById('scrjstoflashDIV')
	if (adv_scrjstoflashDIV_id) adv_scrjstoflashDIV_id.parentNode.removeChild(adv_scrjstoflashDIV_id);
	
	var adv_wpclickdiv_id = document.getElementById('adv_wpclickdiv')
	if (adv_wpclickdiv_id) adv_wpclickdiv_id.parentNode.removeChild(adv_wpclickdiv_id);


	var adv_eyeDiv_id = document.getElementById('eyeDiv')		
	if (adv_eyeDiv_id) adv_eyeDiv_id.parentNode.removeChild(adv_eyeDiv_id);

	if (typeof MM_mcJS_HTML5_IOL_SIZMEKADS_DORESTORE == 'function') {
		try {
			MM_mcJS_HTML5_IOL_SIZMEKADS_DORESTORE(); 
		} catch(e){}
	}
	
	
	var advwp_style_id = document.getElementById('advwp_style_body')
	if (advwp_style_id) advwp_style_id.parentNode.removeChild(advwp_style_id);
	

	if ( typeof(slots) == 'object' && slots.length > 0 )
		googletag.pubads().refresh(slots);
	else
		googletag.pubads().refresh();
	
} 

// ADV END
