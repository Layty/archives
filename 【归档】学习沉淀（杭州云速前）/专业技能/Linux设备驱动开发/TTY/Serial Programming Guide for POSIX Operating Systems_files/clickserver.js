// V4.2.3-141210
var ckSrvUrl="";
var ckTarget="";
var ckTo=null;
var ckFrm=null;

function frmCkSrv(frmobj,a,b,c,continueRequest) {
	if(continueRequest==false) return false;
	if(ckFrm==null) {
	  ckFrm=frmobj;
		ckSrvUrl=goCkSrv(frmobj.action,a,b,c,undefined);
		x=new Image();
		x.onload=ckGoFrm;
		x.onerror=ckGoFrm;
		x.src=ckSrvUrl;
		ckTo=window.setTimeout("ckFrm.submit();",2000);
		return false;
	} else {
	  ckFrm=null;
	  return true;
	}
}
function ckGoFrm() { if(ckFrm!=null) ckFrm.submit(); window.clearTimeout(ckTo); }

function clickServer(url,a,b,c) {
	if(url.indexOf("://")<0) return goCkSrv('http://'+url,a,b,c,null);
	return goCkSrv(url,a,b,c,null);
}
function ckSrv(lnk,a,b,c,target) {
	if(lnk.href !== undefined) {
		urlHref=lnk.href;
		if(target == null && lnk.target !== undefined && lnk.target !== "") {
			target=lnk.target;
		}
	} else urlHref=lnk;
	return goCkSrv(urlHref,a,b,c,target);
}
function goCkSrv(url,a,b,c,target) {
	ckSrvUrl=url;
	ckTarget=self;
	bse=document.getElementsByTagName("BASE");
	if(url.indexOf("://")<0) {
		if(bse[0]) {
			url=bse[0].href+url;
		} else {
			pathname = window.location.pathname;
			pathname = pathname.substring(0, pathname.lastIndexOf('/')+1 );
			if(pathname.charAt(0)=='/') url=window.location.protocol+'//'+window.location.host+'/'+url;
			else url=window.location.protocol+"//"+window.location.host+pathname+url;
		}
	}
	if(bse[0]) {
		if(target==null) {
			if(bse[0].target) {
				target=bse[0].target;
			}
		}
	}
	
	if (url.substring(0,7) == "http://") { url = url.substring(7); }
	else if (url.substring(0,8) == "https://") { url = url.substring(8); }
	dt=new Date();
  var frmUrl="";
  try {
    frmUrl=window.location.host+window.location.pathname;
  } catch(e) {
    frmUrl="_REFER_";
  }
	nloc="http://clickserver.libero.it/ck/"+dt.getTime()+"/"+encodeURI(a)+"/"+encodeURI(b)+"/"+encodeURI(c)+"/_f_/"+encodeURI(frmUrl)+"/_t_/"+url;
	if(target===undefined) return nloc;
	if(target!==null && target.toLowerCase()=='_ajax') {
		x=new Image();
		x.src=nloc;
	} else if(target!==null && target.toLowerCase()!='_top' && target.toLowerCase()!='_parent') {
		x=new Image();
		x.src=nloc;
		w=window.open(ckSrvUrl, target);
	} else {
		w=self;
		cnt=0;
		if(target!==null) {
			if(target.toLowerCase()=='_parent') {
				w=w.parent;
			} else {
				while(w.parent !== w && ++cnt<10) w=w.parent;
			}
		}
		ckTarget=w;
		x=new Image();
		x.onload=ckGo;
		x.onerror=ckGo;
		x.src=nloc;
		ckTo=window.setTimeout("ckTarget.location.href='"+ckSrvUrl+"';",2000);
	}
	return false;
}
function ckGo() { ckTarget.location.href=ckSrvUrl; window.clearTimeout(ckTo); }
