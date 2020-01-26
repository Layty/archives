
if (typeof(advinj1_init) != "undefined") {
	advinj1_init=1;
} else {
	advinj1_init=0;
}

var advinj1_init2;
var advinj1_sup=0;
var advinj1_ie=((document.all)&&(!window.opera));
var advinj1_srv="sito";

if (document.getElementById) advinj1_sup=1;

var advinj1_nk = window.location.toString();
var advinj1_nk2 = advinj1_nk.indexOf(".libero.it");

if (advinj1_nk2 != -1) {
	advinj1_nk3 = advinj1_nk.indexOf("/",advinj1_nk2);
	advinj1_nk = advinj1_nk.substring(advinj1_nk3+1);
	advinj1_nk2 = advinj1_nk.indexOf("/");
	if (advinj1_nk2 != -1) {
		advinj1_nk = advinj1_nk.substring(0,advinj1_nk2);
	}

} else {
	advinj1_nk="";
}

function advinj1_remote(ur) { 
	if (document.getElementById) {
		var iddo = document.getElementById('l_header_topbar');
	  	var element = document.createElement('script');
		element.setAttribute('type','text/javascript'); 
		element.setAttribute('src',ur); 
		iddo.appendChild(element);
	}
}

function advinj1_boot() {
	if (typeof(advinj1_init2) == "undefined") {
		if (advinj1_sup==1) {
		  	var t = new Date();
			advinj1_remote('http://barra-spazio.libero.it/adv.php?n='+advinj1_nk+'&u='+escape(this.location)+'&t='+t.getTime());
		}
		advinj1_init2 = 1;
	}
}

function adv_ok() { }

// google
var googletag = googletag || {};
googletag.cmd = googletag.cmd || [];
var adv_slots = {};
var theHead = document.getElementsByTagName('head')[0];

function getScript(url, callback){
	var theScript = document.createElement('script');
	theScript.type = 'text/javascript';
	theScript.src = url;
	theHead.appendChild(theScript);
	if (typeof callback!=='undefined'){
		theScript.onreadystatechange = function () {
			if (theScript.readyState == 'complete' || theScript.readyState == 'loaded' ) {
				callback();
			}
		};
		theScript.onload = callback;
	}		
}	



function advrender(br) {
	var cond = ((/*Avoid frames*/ window.self.location.href===window.top.location.href));
	if (cond && br=='1') {
			m=document.getElementById('ad72890top');
			b=document.getElementById('ad72890bottom');
			if(m && b){
				getScript('http://www.googletagservices.com/tag/js/gpt.js',
						function(){
							googletag.cmd.push(function() {
								if (typeof(adv_profile)!="undefined"){
									googletag.pubads().setCookieOptions(adv_profile);
							    }								
								if(location.search.indexOf('dctestAdv=1')>0){
									adv_slots['728x90top'] = googletag.defineSlot('/5180/testiol/libero/community/test1/top', [[728, 90]], 'ad72890top').addService(googletag.pubads());
									adv_slots['728x90bottom'] = googletag.defineSlot('/5180/testiol/libero/community/test1/bottom', [[728, 90]], 'ad72890bottom').addService(googletag.pubads());
								}else{
									adv_slots['728x90top'] = googletag.defineSlot('/5180/libero/community/digiland/top', [[728, 90]], 'ad72890top').addService(googletag.pubads());
									adv_slots['728x90bottom'] = googletag.defineSlot('/5180/libero/community/digiland/bottom', [[728, 90]], 'ad72890bottom').addService(googletag.pubads());
								}
								if(typeof(adv_liberotest)=="undefined"){var adv_liberotest=false;}
								IOLAdv.Adv.advlib.setTarget(googletag, 1);
								googletag.pubads().enableSingleRequest();
								googletag.enableServices();
							});
							googletag.cmd.push(function() { 
								if (typeof(adv_profile)!="undefined"){
									googletag.pubads().setCookieOptions(adv_profile);
							    }							
								googletag.display('ad72890top'); 
							});
						}
				);
			}
	} 
}
