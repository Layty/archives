clouda.define("mbaas/consult",function(){var o=(clouda.lightapp,clouda.mbaas.consult={}),u=(clouda.mbaas||{},clouda.DelegateClass,clouda.kuangForReady,clouda.runtimeError,clouda.installPlugin,clouda.cloudaBLight),n=(clouda.ErrCode,clouda.lib.utils.stat);o.openConsult=function(o,a){if(o){if(clouda.PLATFORM===clouda.PLATFORMS.IOS&&clouda.RUNTIME===clouda.RUNTIMES.KUANG&&clouda.RUNTIME_VERSION>="6.8"){var l=function(o){a.onsuccess(o)},c=function(o){a.onfail(o)};return a.appid=o,n({api:"consult.openConsult",runtime:"kuang"}),void u("consult",JSON.stringify(a),l,c)}if(clouda.PLATFORM===clouda.PLATFORMS.ANDROID&&clouda.RUNTIME===clouda.RUNTIMES.KUANG&&clouda.RUNTIME_VERSION>="6.8"){var l=function(o){a.onsuccess(o)},c=function(o){a.onfail(o)};return a.appid=o,n({api:"consult.openConsult",runtime:"kuang"}),void u("consult",JSON.stringify(a),l,c)}n({api:"consult.openConsult",runtime:"web"}),window.location.href="https://pim.baidu.com/web/?appid="+o+"&formid=10000&u="+encodeURIComponent(window.location.href)}}});