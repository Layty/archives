/* Libero COMSCORE engine Version 1.53 */

if ( typeof(Libero) == 'undefined' ) {
	Libero = {};
}

if ( typeof(Libero.Stash) == 'undefined' ) {
	Libero.Stash = {};
}

if ( typeof(Prototype) == 'undefined' ) {
	var Class = {
		create: function() {
			return function() {
				this.initialize.apply(this, arguments);
			};
		}
	};
}

Libero.objectExtend = function(destination, source) {
	for (var property in source) {
		if ( typeof(source[property]) != 'undefined' ) {
			destination[property] = source[property]
		}
	}
	return destination;
};

try { Libero.Stash.csDebug = libero_comscore_debug } catch (e) {}
try { Libero.Stash.csTest = libero_comscore_test } catch (e) {}
try { Libero.Stash.csCNAutoComplete = libero_comscore_cnautocomplete } catch (e) {}
try { Libero.Stash.csCheckMultipleCalls = libero_comscore_check_multiple_calls } catch (e) {}
try { Libero.Stash.csObj = libero_comscore_csobj } catch (e) {}
try { Libero.Stash.csLabels = libero_comscore_labels } catch (e) {}

if ( /\.(st|pp)\.libero\.it(:\d+)?$/.test(document.location.host) ) {
	Libero.Stash.csTest = 1;
	Libero.Stash.csDebug = 1;
}

if ( /Libero(Tracking|Comscore)Debug=(\d+)/.test(document.cookie) ) {
	Libero.Stash.csDebug = parseInt(RegExp.$2);
}

if ( typeof console == 'undefined' ) {
	Libero.Stash.csDebug = 0;
}

if ( Libero.Stash.csDebug )
	console.log('[LT]', 'Loading tracking engine');

if ( typeof Libero.Stash.csImages != 'number' ) 
	Libero.Stash.csImages = 0;

Libero.getTracking = function () {
	var args = Libero.objectExtend({
		csObj: Libero.Stash.csObj,
		ajax: undefined,
		url: document.location.href,
		path: document.location.pathname,
		cookie: document.cookie,
		search: document.location.search,
		referrer: document.referrer,
		debug: Libero.Stash.csDebug,
		test: Libero.Stash.csTest,
		callDirectly: 0,
		callJS: 0,
		https: document.location.protocol == 'https:', 
		log_prefix: '[LT]',
		check_multiple_calls: Libero.Stash.csCheckMultipleCalls,
		add_host_to_cn: Libero.Stash.csCNAutoComplete,
		onLoadHandler: undefined,
		labels: Libero.Stash.csLabels || {},
		timeout: 2,
		trackingUrl: undefined,
		cacheCountername: false,
		cs_ns_site: 'libero',
		cs_c2: '13259779',
		profile: true,
		title: document.title,
		charset: document.characterSet || document.defaultCharset,
		vportal: 0
	}, arguments[0]);

	
	if ( typeof console == 'undefined' ) {
		args.debug = 0;
	}

	args.version = '1.53';

	if ( args.debug )
		console.log(args.log_prefix, 'getTracking (' + args.version + ')', args);

	var countername;

	if ( args.cacheCountername && typeof(Libero.Stash.csName) != 'undefined' ) {
		countername = Libero.Stash.csName;
		if ( args.debug )
			console.log(args.log_prefix, 'Getting cn from cache: ', countername);
	}	
	else {
		
		if ( typeof(args.csObj) == 'undefined' || typeof(args.csObj.cn) != 'object' ) {
			if ( args.debug )
				console.log(args.log_prefix, 'Undefined \'csObj\'');
			return undefined;
		}

		countername = args.csObj.defaultCn;
	
		for ( var i = 0, l = args.csObj.cn.length; i < l; i++ ) {
	
			var found = 0;
	
			if ( args.debug )
				console.log(args.log_prefix, args.csObj.cn[i].n, args.csObj.cn[i]);
	
			var re;
			try {
				re = new RegExp(args.csObj.cn[i].v);
			}
			catch (e) {
				if (args.debug)
					console.log(args.log_prefix, e);
				continue;		
			}
			
			if ( args.debug )
				console.log(args.log_prefix, 'Matching ' + args.csObj.cn[i].t + ' ' + args.csObj.cn[i].v);
	
			var value; 
	
			if ( args.csObj.cn[i].t == 'js' ) {
				try {
					eval('value = ' + args.csObj.cn[i].id);
				}
				catch (e) {
					if (args.debug)
						console.log(args.log_prefix, e);
					continue;
				}
			}
			else if ( args.csObj.cn[i].t == 'dom' ) {
				try {
					value = document.getElementById(args.csObj.cn[i].id).innerHTML;
				}
				catch (e) {
					if (args.debug)
						console.log(args.log_prefix, e);
					continue;
				}
			}
			else {	
				value = args[args.csObj.cn[i].t];
			}
			
			if ( value == null || /^\s*$/.test(value) ) {
				if (args.debug)
					console.log(args.log_prefix, 'Skipped null value');
				continue;
			}
	
			if ( re.test(value) ) {
				if ( args.debug ) 
					console.log(args.log_prefix, 'FOUND (' + args[args.csObj.cn[i].t] + ' =~ ' + re + ')');
				found = 1;
				countername = args.csObj.cn[i].n;
				break;
			}
		}
	
		if ( args.add_host_to_cn ) {
			countername += '.' + document.location.hostname.replace(/\./g,'_');
		}
	} // End getting countername

	if ( typeof(args.csObj.site) === 'string' ) {
		args.cs_ns_site = args.csObj.site;
	}

	if ( ( args.callJS && typeof COMSCORE != 'undefined' ) || args.callDirectly ) {

		Libero.Stash.csImages++;

		if ( typeof args.check_multiple_calls == 'number' && args.check_multiple_calls > 0 && Libero.Stash.csImages > args.check_multiple_calls ) {
			if ( args.debug )
				console.log(args.log_prefix, 'Tracking skipped, the calls (' + Libero.Stash.csImages + ') reached max limit (' + args.check_multiple_calls + ')');
		}
		else {
		
			if ( typeof(args.labels.wa_header) === 'undefined' && /^vportals/.test(args.cs_ns_site) ) {
				if ( document.getElementById('vh_header_top') ) {
					args.labels.wa_header = 'virgilio';
				}
				else if ( document.getElementById('l_header_topbar') ) {
					args.labels.wa_header = 'libero';
				}
			}

			if ( args.vportal ) {
				args.referrer = document.referrer ? args.referrer : document.referrer
			}

			if ( args.profile ) {
				if ( typeof(iol_profile_data) != 'object' || typeof(iol_profile_data.geo) != 'object' ) {

					if ( args.debug )
						console.log(args.log_prefix,'iol_profile_data ko');

					try {
						iol_profile_data = (new IOL.Profile()).getProfile();
						if ( args.debug )
							console.log(args.log_prefix,'IOL.Profile ok');
					}
					catch(e) {
						if ( args.debug )
							console.log(args.log_prefix,'IOL.Profile ko');
					}
				}
				else {
					if ( args.debug )
						console.log(args.log_prefix,'iol_profile_data ok');
				}

				if ( typeof(iol_profile_data) === 'object' ) {
					if ( typeof(iol_profile_data.geo) === 'object' ) {
						args.labels.wa_geo_1_id = iol_profile_data.geo.istatComune;
						args.labels.wa_geo_2_id = iol_profile_data.geo.istatProvincia;
						args.labels.wa_geo_3_id = iol_profile_data.geo.isp;
						args.labels.wa_geo_4_id = iol_profile_data.geo.codiceRegione;
					}
					if ( typeof(iol_profile_data.local) === 'object' ) {
						args.labels.wa_local_1_id = iol_profile_data.local.idprovincia;
						args.labels.wa_local_2_id = iol_profile_data.local.idcity;
						args.labels.wa_local_3_id = iol_profile_data.local.regione;
					}
					if ( typeof(iol_profile_data.adv) === 'object' ) {
						args.labels.wa_adv_1_id = iol_profile_data.adv.sesso;
						args.labels.wa_adv_2_id = iol_profile_data.adv.fasciaEta;
						args.labels.wa_adv_3_id = iol_profile_data.adv.provincia;
					}
				 }
			}
		
			args.labels.wa_engine_v = args.version;
			
			if ( typeof(args.csObj.version) != 'undefined' ) {
				args.labels.wa_data_v = args.csObj.version;
			}
			
			if ( typeof(args.referrer) != 'undefined' && ! /^\s*$/.test(args.referrer) && ! /^https?:\/\//.test(args.referrer) ) {
				args.referrer = 'http://' + args.referrer;
			}

			if ( args.callDirectly ) {
				var i = new Image(), 
					esc = typeof(encodeURIComponent) == 'function' ? encodeURIComponent : escape, 
					label_qs = '';
				if ( typeof(args.labels) == 'object' ) {
					for ( var prop in args.labels ) {
						if ( typeof(args.labels[prop]) == 'undefined' )
							continue;
						label_qs += '&' + prop + '=' + esc(args.labels[prop]);
					}
				}
	
				if ( typeof(args.onLoadHandler) == 'function' ) {
					if ( args.debug )
						console.log(args.log_prefix, 'Adding onload handler: ', args.onLoadHandler);
					
					var tracking_timeout = window.setTimeout("Libero.Stash.csOnLoad()", args.timeout*1000);
					
					Libero.Stash.csOnLoad = function () {
						try { window.clearTimeout(tracking_timeout) } catch (e) {}
	
						i.onload = i.onerror = function () {return};
						
						args.onLoadHandler();
					};
					i.onload = Libero.Stash.csOnLoad;
					i.onerror = Libero.Stash.csOnLoad;
				}
				
				if ( typeof(args.trackingUrl) != 'undefined' && typeof(args.trackingUrl) == 'string' )
					i.src = args.trackingUrl + '&_t=' + +(new Date); 
				else
					i.src = (args.https ? 'https://sb' : 'http://b') + '.scorecardresearch.com/b?c1=2&c2=' + args.cs_c2 + '&name=' + esc(countername) + '&ns_site=' + ( args.test ? 'test' : args.cs_ns_site ) + '&c7=' + esc(args.url) + '&c8=' + esc(args.title) + '&c9=' + esc(args.ajax || args.referrer) + '&ns__t=' + +(new Date) + '&ns_c=' + esc(args.charset) + label_qs;
				if ( args.debug )
					console.log(args.log_prefix, 'Making direct call to ', i);
			}
			else {
	
				if ( args.debug )
					console.log(args.log_prefix, 'Making standard call...');
	
				COMSCORE.beacon(
					Libero.objectExtend(
						{
							c1: 2,
							c2: args.cs_c2,
							name: countername,
							ns_site: args.test ? 'test' : args.cs_ns_site
						},
						typeof(args.labels) == 'object' ? args.labels : {}
					) 
				);
			}
		} // End tracking call
	}

	if ( args.debug )
		console.log(args.log_prefix, 'The winner is: ' + countername);
	return countername;
}
Libero.getComscore = Libero.getTracking;

Libero.click = function () {

	var on_load_handler;
	var argument_idx = 0;
	var href;

	if ( typeof(arguments[0]) == 'object' && arguments[0].tagName == 'A' ) { 
		href = arguments[0].href;
		if ( typeof(href) == 'string' ) {
			on_load_handler = function () {
				document.location.href = href;
			};
			argument_idx = 1;
		}
	}
	else {
		argument_idx = 0;
	}

	var args = Libero.objectExtend({
		callDirectly: 1,
		cacheCountername: 1,
		onLoadHandler: on_load_handler
	}, arguments[argument_idx]); 

	args.labels = Libero.objectExtend(args.labels || {}, {
		ns_type: 'hidden',
		lib_name: Libero.Stash.csName,
		lib_ev_type: 'click',
		lib_dest: href
	});

	return Libero.getTracking(args);
}

Libero.CGI = function (url) {
	var query_string;
	if ( typeof(url) == 'undefined' || typeof(url) != 'string' )
		return {};
	query_string = url.split(/\?/)[1];
	if ( query_string ) {
		this.search = query_string;
		var pairs = this.search.split(/\&+/);
		for ( var i = 0; i < pairs.length; i++ ) {
			var pair = pairs[i].split('=');
			for ( var y = 0; y < pair.length; y++ ) {
				pair[y] = pair[y].replace(/\+/g, ' ');
				pair[y] = pair[y].replace(/%([a-fA-F0-9][a-fA-F0-9])/g,
					function ($0,$1) {
						$1 = $1.toUpperCase();
						var h = '0123456789ABCDEF';
						var dec = 0;
						var a = 0;
						for ( n = $1.length; n > 0; n-- ) {
							dec += h.indexOf($1.charAt(n-1)) * Math.pow(16,a);
							a++;
						}
						return String.fromCharCode(dec);
					} );
			}
			this[pair[0]] = pair[1];
		}
	}
	else {
		return {}
	}
}

Libero.Stash.q = new Libero.CGI(document.location.href);
Libero.Stash.csCall = 0; 
for (var type = ['url','cookie','path','ajax','directly','referrer'], i = 0; i < type.length; i++ ) {
	if ( typeof Libero.Stash.q['cs_' + type[i]] != 'undefined' ) {
		if ( Libero.Stash.csDebug )
			console.log('[LT]', 'CGI query string: ' + type[i], Libero.Stash.q['cs_' + type[i]]);
		Libero.Stash.csCall++;
	}
}

if ( Libero.Stash.csCall > 0 ) {
	Libero.Stash.csName = Libero.getTracking({ 
		url: Libero.Stash.q['cs_url'], 
		path: Libero.Stash.q['cs_path'], 
		cookie: Libero.Stash.q['cs_cookie'], 
		ajax: Libero.Stash.q['cs_ajax'], 
		callDirectly: 1, 
		debug: parseInt(Libero.Stash.csDebug || Libero.Stash.q.cs_debug), 
		test: parseInt(Libero.Stash.csTest || Libero.Stash.q.cs_test), 
		https: parseInt(Libero.Stash.q.cs_https),
		referrer: Libero.Stash.q.cs_referrer,
		title: Libero.Stash.q.cs_title,
		vportal: parseInt(Libero.Stash.q.cs_vp),
		charset: Libero.Stash.q.cs_charset
	});
}
else {
	Libero.Stash.csName = Libero.getTracking({ 
		callJS: 1, 
		debug: parseInt(Libero.Stash.csDebug || Libero.Stash.q.cs_debug), 
		test: parseInt(Libero.Stash.csTest || Libero.Stash.q.cs_test), 
		referrer: Libero.Stash.q.cs_referrer,
		vportal: parseInt(Libero.Stash.q.cs_vp)
	});
}