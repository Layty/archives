X3C http交互格式

1、http://192.168.7.1/get_guide_info.json

curl "http://192.168.7.1/get_guide_info.json" -H "Cookie: sn=230921016; 230921016guide=pwdSetting; guide_info=^%^7B^%^22wan_proto^%^22^%^3A^%^22^%^22^%^2C^%^22wan_ip^%^22^%^3A^%^22^%^22^%^2C^%^22wan_netmask^%^22^%^3A^%^22^%^22^%^2C^%^22wan_gateway^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_name^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22dns1^%^22^%^3A^%^22^%^22^%^2C^%^22dns2^%^22^%^3A^%^22^%^22^%^2C^%^22radio^%^22^%^3A^%^22^%^22^%^2C^%^22ssid^%^22^%^3A^%^22^%^22^%^2C^%^22is_broadcast^%^22^%^3A^%^22^%^22^%^2C^%^22akm^%^22^%^3A^%^22^%^22^%^2C^%^22ssid_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22ssid_5g^%^22^%^3A^%^22^%^22^%^2C^%^22is_broadcast_5g^%^22^%^3A^%^22^%^22^%^2C^%^22akm_5g^%^22^%^3A^%^22^%^22^%^2C^%^22ssid_pwd_5g^%^22^%^3A^%^22^%^22^%^2C^%^22wifi_mode_type^%^22^%^3A^%^22^%^22^%^7D" -H "Accept-Encoding: gzip, deflate, sdch" -H "Accept-Language: zh-CN,zh;q=0.8" -H "User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36" -H "Accept: application/json, text/javascript, */*; q=0.01" -H "Referer: http://192.168.7.1/html/guide/pwdSetting.html" -H "X-Requested-With: XMLHttpRequest" -H "Connection: keep-alive" --compressed

GET /get_guide_info.json HTTP/1.1
Host: 192.168.7.1
Connection: keep-alive
Accept: application/json, text/javascript, */*; q=0.01
X-Requested-With: XMLHttpRequest
User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36
Referer: http://192.168.7.1/html/guide/pwdSetting.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8
Cookie: sn=230921016; 230921016guide=pwdSetting; guide_info=%7B%22wan_proto%22%3A%22%22%2C%22wan_ip%22%3A%22%22%2C%22wan_netmask%22%3A%22%22%2C%22wan_gateway%22%3A%22%22%2C%22ppp_name%22%3A%22%22%2C%22ppp_pwd%22%3A%22%22%2C%22dns1%22%3A%22%22%2C%22dns2%22%3A%22%22%2C%22radio%22%3A%22%22%2C%22ssid%22%3A%22%22%2C%22is_broadcast%22%3A%22%22%2C%22akm%22%3A%22%22%2C%22ssid_pwd%22%3A%22%22%2C%22ssid_5g%22%3A%22%22%2C%22is_broadcast_5g%22%3A%22%22%2C%22akm_5g%22%3A%22%22%2C%22ssid_pwd_5g%22%3A%22%22%2C%22wifi_mode_type%22%3A%22%22%7D

{
	"result":	{
		"wan":	{
			"proto":	"dhcp",
			"ip":	"0.0.0.0",
			"netmask":	"0.0.0.0",
			"gateway":	"0.0.0.0",
			"ppp_name":	"",
			"ppp_pwd":	"",
			"dns1":	"",
			"dns2":	""
		},
		"wifi":	{
			"radio":	true,
			"ssid":	"ezviz_03da00",
			"is_broadcast":	true,
			"akm":	"open",
			"ssid_pwd":	"",
			"ssid_5g":	"ezviz_03da00_5G",
			"is_broadcast_5g":	true,
			"akm_5g":	"open",
			"ssid_pwd_5g":	"",
			"wifi_mode_type":	"class_3",
			"guest_ssid":	"ezviz_guest_03da00",
			"guest_is_broadcast":	true,
			"guest_akm":	"open",
			"guest_ssid_pwd":	"",
			"hotspot_enable":	0
		}
	},
	"status":	{
		"ret_code":	0,
		"err_reason":	""
	}
}

2、http://192.168.7.1/set_guide_info.cgi
curl "http://192.168.7.1/set_guide_info.cgi" -H "Authorization: lepgVFnzerbqnNKlYaACsFinIvMW8S" -H "Origin: http://192.168.7.1" -H "Accept-Encoding: gzip, deflate" -H "Accept-Language: zh-CN,zh;q=0.8" -H "User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36" -H "Content-Type: application/x-www-form-urlencoded; charset=UTF-8" -H "Accept: application/json, text/javascript, */*; q=0.01" -H "Referer: http://192.168.7.1/html/guide/wifiSetting.html" -H "X-Requested-With: XMLHttpRequest" -H "Cookie: sn=230921016; Authorization=lepgVFnzerbqnNKlYaACsFinIvMW8S; masterNetIp=; 230921016guide=wifiSetting; guide_info=^%^7B^%^22wan_proto^%^22^%^3A^%^22dhcp^%^22^%^2C^%^22wan_ip^%^22^%^3A^%^22^%^22^%^2C^%^22wan_netmask^%^22^%^3A^%^22^%^22^%^2C^%^22wan_gateway^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_name^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22dns1^%^22^%^3A^%^22^%^22^%^2C^%^22dns2^%^22^%^3A^%^22^%^22^%^2C^%^22radio^%^22^%^3Atrue^%^2C^%^22ssid^%^22^%^3A^%^22ezviz_03da00^%^22^%^2C^%^22is_broadcast^%^22^%^3Atrue^%^2C^%^22akm^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22ssid_5g^%^22^%^3A^%^22ezviz_03da00_5G^%^22^%^2C^%^22is_broadcast_5g^%^22^%^3Atrue^%^2C^%^22akm_5g^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd_5g^%^22^%^3A^%^22^%^22^%^2C^%^22wifi_mode_type^%^22^%^3A^%^22class_3^%^22^%^7D" -H "Connection: keep-alive" --data "wan_proto=dhcp^&wan_ip=^&wan_netmask=^&wan_gateway=^&ppp_name=^&ppp_pwd=^&dns1=^&dns2=^&radio=true^&ssid=ezviz_03da00^&is_broadcast=true^&akm=wpa-wpa2-hybrid^&ssid_pwd=^&ssid_5g=ezviz_03da00_5G^&is_broadcast_5g=true^&akm_5g=open^&ssid_pwd_5g=^&wifi_mode_type=class_1^&master_ssid=ezviz_03da00^&master_ssid_5g=ezviz_03da00_5G^&master_akm=wpa-wpa2-hybrid^&master_akm_5g=wpa-wpa2-hybrid^&master_ssid_pwd=a12345678^&master_ssid_pwd_5g=a12345678" --compressed

POST /set_guide_info.cgi HTTP/1.1
Host: 192.168.7.1
Connection: keep-alive
Content-Length: 417
Authorization: lepgVFnzerbqnNKlYaACsFinIvMW8S
Origin: http://192.168.7.1
User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36
Content-Type: application/x-www-form-urlencoded; charset=UTF-8
Accept: application/json, text/javascript, */*; q=0.01
X-Requested-With: XMLHttpRequest
Referer: http://192.168.7.1/html/guide/wifiSetting.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8
Cookie: sn=230921016; Authorization=lepgVFnzerbqnNKlYaACsFinIvMW8S; masterNetIp=; 230921016guide=wifiSetting; guide_info=%7B%22wan_proto%22%3A%22dhcp%22%2C%22wan_ip%22%3A%22%22%2C%22wan_netmask%22%3A%22%22%2C%22wan_gateway%22%3A%22%22%2C%22ppp_name%22%3A%22%22%2C%22ppp_pwd%22%3A%22%22%2C%22dns1%22%3A%22%22%2C%22dns2%22%3A%22%22%2C%22radio%22%3Atrue%2C%22ssid%22%3A%22ezviz_03da00%22%2C%22is_broadcast%22%3Atrue%2C%22akm%22%3A%22open%22%2C%22ssid_pwd%22%3A%22%22%2C%22ssid_5g%22%3A%22ezviz_03da00_5G%22%2C%22is_broadcast_5g%22%3Atrue%2C%22akm_5g%22%3A%22open%22%2C%22ssid_pwd_5g%22%3A%22%22%2C%22wifi_mode_type%22%3A%22class_3%22%7D; guide=assocIpc

{
	"status":	{
		"ret_code":	0,
		"err_reason":	""
	}
}

3、http://192.168.7.1/get_wireless_2g.json
curl "http://192.168.7.1/get_wireless_2g.json" -H "Cookie: sn=230921016; Authorization=lepgVFnzerbqnNKlYaACsFinIvMW8S; masterNetIp=; guide_info=^%^7B^%^22wan_proto^%^22^%^3A^%^22dhcp^%^22^%^2C^%^22wan_ip^%^22^%^3A^%^22^%^22^%^2C^%^22wan_netmask^%^22^%^3A^%^22^%^22^%^2C^%^22wan_gateway^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_name^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22dns1^%^22^%^3A^%^22^%^22^%^2C^%^22dns2^%^22^%^3A^%^22^%^22^%^2C^%^22radio^%^22^%^3Atrue^%^2C^%^22ssid^%^22^%^3A^%^22ezviz_03da00^%^22^%^2C^%^22is_broadcast^%^22^%^3Atrue^%^2C^%^22akm^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22ssid_5g^%^22^%^3A^%^22ezviz_03da00_5G^%^22^%^2C^%^22is_broadcast_5g^%^22^%^3Atrue^%^2C^%^22akm_5g^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd_5g^%^22^%^3A^%^22^%^22^%^2C^%^22wifi_mode_type^%^22^%^3A^%^22class_3^%^22^%^7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null" -H "Accept-Encoding: gzip, deflate, sdch" -H "Accept-Language: zh-CN,zh;q=0.8" -H "Authorization: lepgVFnzerbqnNKlYaACsFinIvMW8S" -H "Accept: application/json, text/javascript, */*; q=0.01" -H "Referer: http://192.168.7.1/html/status/status.html" -H "X-Requested-With: XMLHttpRequest" -H "Connection: keep-alive" -H "User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36" --compressed

GET /get_wireless_2g.json HTTP/1.1
Host: 192.168.7.1
Connection: keep-alive
Accept: application/json, text/javascript, */*; q=0.01
X-Requested-With: XMLHttpRequest
User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36
Authorization: lepgVFnzerbqnNKlYaACsFinIvMW8S
Referer: http://192.168.7.1/html/status/status.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8
Cookie: sn=230921016; Authorization=lepgVFnzerbqnNKlYaACsFinIvMW8S; masterNetIp=; guide_info=%7B%22wan_proto%22%3A%22dhcp%22%2C%22wan_ip%22%3A%22%22%2C%22wan_netmask%22%3A%22%22%2C%22wan_gateway%22%3A%22%22%2C%22ppp_name%22%3A%22%22%2C%22ppp_pwd%22%3A%22%22%2C%22dns1%22%3A%22%22%2C%22dns2%22%3A%22%22%2C%22radio%22%3Atrue%2C%22ssid%22%3A%22ezviz_03da00%22%2C%22is_broadcast%22%3Atrue%2C%22akm%22%3A%22open%22%2C%22ssid_pwd%22%3A%22%22%2C%22ssid_5g%22%3A%22ezviz_03da00_5G%22%2C%22is_broadcast_5g%22%3Atrue%2C%22akm_5g%22%3A%22open%22%2C%22ssid_pwd_5g%22%3A%22%22%2C%22wifi_mode_type%22%3A%22class_3%22%7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null

{
	"result":	{
		"basic":	{
			"radio":	true,
			"bw_cap":	"ht20",
			"chanspec":	"auto",
			"current_chan":	"HT20-7",
			"current_radio":	true
		},
		"master":	{
			"bss_enabled":	true,
			"mode":	"ap",
			"ssid":	"ezviz_03da00",
			"is_broadcast":	true,
			"ap_isolate":	false,
			"akm":	"wpa-wpa2-hybrid",
			"ssid_pwd":	"a12345678"
		},
		"support_channels":	{
			"HT20":	["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13"],
			"HT40":	["5u", "6u", "7u", "8u", "9u", "10u", "11u", "12u", "13u", "1l", "2l", "3l", "4l", "5l", "6l", "7l", "8l", "9l"]
		}
	},
	"status":	{
		"ret_code":	0,
		"err_reason":	""
	}
}

4、http://192.168.7.1/set_wireless_2g.cgi
curl "http://192.168.7.1/set_wireless_2g.cgi" -H "Authorization: lepgVFnzerbqnNKlYaACsFinIvMW8S" -H "Origin: http://192.168.7.1" -H "Accept-Encoding: gzip, deflate" -H "Accept-Language: zh-CN,zh;q=0.8" -H "User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36" -H "Content-Type: application/x-www-form-urlencoded; charset=UTF-8" -H "Accept: application/json, text/javascript, */*; q=0.01" -H "Referer: http://192.168.7.1/html/setting/wifi/wifi.html" -H "X-Requested-With: XMLHttpRequest" -H "Cookie: sn=230921016; Authorization=lepgVFnzerbqnNKlYaACsFinIvMW8S; masterNetIp=; guide_info=^%^7B^%^22wan_proto^%^22^%^3A^%^22dhcp^%^22^%^2C^%^22wan_ip^%^22^%^3A^%^22^%^22^%^2C^%^22wan_netmask^%^22^%^3A^%^22^%^22^%^2C^%^22wan_gateway^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_name^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22dns1^%^22^%^3A^%^22^%^22^%^2C^%^22dns2^%^22^%^3A^%^22^%^22^%^2C^%^22radio^%^22^%^3Atrue^%^2C^%^22ssid^%^22^%^3A^%^22ezviz_03da00^%^22^%^2C^%^22is_broadcast^%^22^%^3Atrue^%^2C^%^22akm^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22ssid_5g^%^22^%^3A^%^22ezviz_03da00_5G^%^22^%^2C^%^22is_broadcast_5g^%^22^%^3Atrue^%^2C^%^22akm_5g^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd_5g^%^22^%^3A^%^22^%^22^%^2C^%^22wifi_mode_type^%^22^%^3A^%^22class_3^%^22^%^7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null" -H "Connection: keep-alive" --data "master_ssid=ezviz_03da00^&master_is_broadcast=true^&master_akm=wpa2-psk(aes)^&master_ssid_pwd=a12345678^&master_ap_isolate=false^&bw_cap=ht20^&chanspec=auto" --compressed

POST /set_wireless_2g.cgi HTTP/1.1
Host: 192.168.7.1
Connection: keep-alive
Content-Length: 150
Authorization: lepgVFnzerbqnNKlYaACsFinIvMW8S
Origin: http://192.168.7.1
User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36
Content-Type: application/x-www-form-urlencoded; charset=UTF-8
Accept: application/json, text/javascript, */*; q=0.01
X-Requested-With: XMLHttpRequest
Referer: http://192.168.7.1/html/setting/wifi/wifi.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8
Cookie: sn=230921016; Authorization=lepgVFnzerbqnNKlYaACsFinIvMW8S; masterNetIp=; guide_info=%7B%22wan_proto%22%3A%22dhcp%22%2C%22wan_ip%22%3A%22%22%2C%22wan_netmask%22%3A%22%22%2C%22wan_gateway%22%3A%22%22%2C%22ppp_name%22%3A%22%22%2C%22ppp_pwd%22%3A%22%22%2C%22dns1%22%3A%22%22%2C%22dns2%22%3A%22%22%2C%22radio%22%3Atrue%2C%22ssid%22%3A%22ezviz_03da00%22%2C%22is_broadcast%22%3Atrue%2C%22akm%22%3A%22open%22%2C%22ssid_pwd%22%3A%22%22%2C%22ssid_5g%22%3A%22ezviz_03da00_5G%22%2C%22is_broadcast_5g%22%3Atrue%2C%22akm_5g%22%3A%22open%22%2C%22ssid_pwd_5g%22%3A%22%22%2C%22wifi_mode_type%22%3A%22class_3%22%7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null


{
	"result":	{
		"current_chan":	"7"
	},
	"status":	{
		"ret_code":	0,
		"err_reason":	""
	}
}


5、http://192.168.7.1/get_wireless_5g.json
curl "http://192.168.7.1/get_wireless_5g.json" -H "Cookie: sn=230921016; Authorization=lepgVFnzerbqnNKlYaACsFinIvMW8S; masterNetIp=; guide_info=^%^7B^%^22wan_proto^%^22^%^3A^%^22dhcp^%^22^%^2C^%^22wan_ip^%^22^%^3A^%^22^%^22^%^2C^%^22wan_netmask^%^22^%^3A^%^22^%^22^%^2C^%^22wan_gateway^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_name^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22dns1^%^22^%^3A^%^22^%^22^%^2C^%^22dns2^%^22^%^3A^%^22^%^22^%^2C^%^22radio^%^22^%^3Atrue^%^2C^%^22ssid^%^22^%^3A^%^22ezviz_03da00^%^22^%^2C^%^22is_broadcast^%^22^%^3Atrue^%^2C^%^22akm^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22ssid_5g^%^22^%^3A^%^22ezviz_03da00_5G^%^22^%^2C^%^22is_broadcast_5g^%^22^%^3Atrue^%^2C^%^22akm_5g^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd_5g^%^22^%^3A^%^22^%^22^%^2C^%^22wifi_mode_type^%^22^%^3A^%^22class_3^%^22^%^7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null" -H "Accept-Encoding: gzip, deflate, sdch" -H "Accept-Language: zh-CN,zh;q=0.8" -H "Authorization: lepgVFnzerbqnNKlYaACsFinIvMW8S" -H "Accept: application/json, text/javascript, */*; q=0.01" -H "Referer: http://192.168.7.1/html/status/status.html" -H "X-Requested-With: XMLHttpRequest" -H "Connection: keep-alive" -H "User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36" --compressed

GET /get_wireless_5g.json HTTP/1.1
Host: 192.168.7.1
Connection: keep-alive
Accept: application/json, text/javascript, */*; q=0.01
X-Requested-With: XMLHttpRequest
User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36
Authorization: lepgVFnzerbqnNKlYaACsFinIvMW8S
Referer: http://192.168.7.1/html/status/status.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8
Cookie: sn=230921016; Authorization=lepgVFnzerbqnNKlYaACsFinIvMW8S; masterNetIp=; guide_info=%7B%22wan_proto%22%3A%22dhcp%22%2C%22wan_ip%22%3A%22%22%2C%22wan_netmask%22%3A%22%22%2C%22wan_gateway%22%3A%22%22%2C%22ppp_name%22%3A%22%22%2C%22ppp_pwd%22%3A%22%22%2C%22dns1%22%3A%22%22%2C%22dns2%22%3A%22%22%2C%22radio%22%3Atrue%2C%22ssid%22%3A%22ezviz_03da00%22%2C%22is_broadcast%22%3Atrue%2C%22akm%22%3A%22open%22%2C%22ssid_pwd%22%3A%22%22%2C%22ssid_5g%22%3A%22ezviz_03da00_5G%22%2C%22is_broadcast_5g%22%3Atrue%2C%22akm_5g%22%3A%22open%22%2C%22ssid_pwd_5g%22%3A%22%22%2C%22wifi_mode_type%22%3A%22class_3%22%7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null

{
	"result":	{
		"basic":	{
			"radio":	true,
			"bw_cap":	"ht40",
			"chanspec":	"auto",
			"current_chan":	"VHT40-40u",
			"current_radio":	true
		},
		"master":	{
			"bss_enabled":	true,
			"mode":	"ap",
			"ssid":	"ezviz_03da00_5G",
			"is_broadcast":	true,
			"ap_isolate":	false,
			"akm":	"wpa-wpa2-hybrid",
			"ssid_pwd":	"a12345678"
		},
		"support_channels":	{
			"HT20":	["36", "40", "44", "48", "52", "56", "60", "64", "149", "153", "157", "161", "165"],
			"HT40":	["40u", "48u", "56u", "64u", "153u", "161u", "36l", "44l", "52l", "60l", "149l", "157l"],
			"HT80":	["36/80", "52/80", "149/80", "40/80", "56/80", "153/80", "44/80", "60/80", "157/80", "48/80", "64/80", "161/80"]
		}
	},
	"status":	{
		"ret_code":	0,
		"err_reason":	""
	}
}

6、http://192.168.7.1/set_wireless_5g.cgi
curl "http://192.168.7.1/set_wireless_5g.cgi" -H "Authorization: lepgVFnzerbqnNKlYaACsFinIvMW8S" -H "Origin: http://192.168.7.1" -H "Accept-Encoding: gzip, deflate" -H "Accept-Language: zh-CN,zh;q=0.8" -H "User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36" -H "Content-Type: application/x-www-form-urlencoded; charset=UTF-8" -H "Accept: application/json, text/javascript, */*; q=0.01" -H "Referer: http://192.168.7.1/html/setting/wifi/wifi.html" -H "X-Requested-With: XMLHttpRequest" -H "Cookie: sn=230921016; Authorization=lepgVFnzerbqnNKlYaACsFinIvMW8S; masterNetIp=; guide_info=^%^7B^%^22wan_proto^%^22^%^3A^%^22dhcp^%^22^%^2C^%^22wan_ip^%^22^%^3A^%^22^%^22^%^2C^%^22wan_netmask^%^22^%^3A^%^22^%^22^%^2C^%^22wan_gateway^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_name^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22dns1^%^22^%^3A^%^22^%^22^%^2C^%^22dns2^%^22^%^3A^%^22^%^22^%^2C^%^22radio^%^22^%^3Atrue^%^2C^%^22ssid^%^22^%^3A^%^22ezviz_03da00^%^22^%^2C^%^22is_broadcast^%^22^%^3Atrue^%^2C^%^22akm^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22ssid_5g^%^22^%^3A^%^22ezviz_03da00_5G^%^22^%^2C^%^22is_broadcast_5g^%^22^%^3Atrue^%^2C^%^22akm_5g^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd_5g^%^22^%^3A^%^22^%^22^%^2C^%^22wifi_mode_type^%^22^%^3A^%^22class_3^%^22^%^7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null" -H "Connection: keep-alive" --data "master_ssid=ezviz_03da00_5G^&master_is_broadcast=true^&master_akm=wpa2-psk(aes)^&master_ssid_pwd=a12345678^&master_ap_isolate=false^&bw_cap=ht40^&chanspec=auto" --compressed

POST /set_wireless_5g.cgi HTTP/1.1
Host: 192.168.7.1
Connection: keep-alive
Content-Length: 153
Authorization: lepgVFnzerbqnNKlYaACsFinIvMW8S
Origin: http://192.168.7.1
User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36
Content-Type: application/x-www-form-urlencoded; charset=UTF-8
Accept: application/json, text/javascript, */*; q=0.01
X-Requested-With: XMLHttpRequest
Referer: http://192.168.7.1/html/setting/wifi/wifi.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8
Cookie: sn=230921016; Authorization=lepgVFnzerbqnNKlYaACsFinIvMW8S; masterNetIp=; guide_info=%7B%22wan_proto%22%3A%22dhcp%22%2C%22wan_ip%22%3A%22%22%2C%22wan_netmask%22%3A%22%22%2C%22wan_gateway%22%3A%22%22%2C%22ppp_name%22%3A%22%22%2C%22ppp_pwd%22%3A%22%22%2C%22dns1%22%3A%22%22%2C%22dns2%22%3A%22%22%2C%22radio%22%3Atrue%2C%22ssid%22%3A%22ezviz_03da00%22%2C%22is_broadcast%22%3Atrue%2C%22akm%22%3A%22open%22%2C%22ssid_pwd%22%3A%22%22%2C%22ssid_5g%22%3A%22ezviz_03da00_5G%22%2C%22is_broadcast_5g%22%3Atrue%2C%22akm_5g%22%3A%22open%22%2C%22ssid_pwd_5g%22%3A%22%22%2C%22wifi_mode_type%22%3A%22class_3%22%7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null

{
	"result":	{
		"current_chan":	"40u"
	},
	"status":	{
		"ret_code":	0,
		"err_reason":	""
	}
}


7、http://192.168.7.1/get_wireless_acl.json
curl "http://192.168.7.1/get_wireless_acl.json" -H "Cookie: sn=230921016; Authorization=lepgVFnzerbqnNKlYaACsFinIvMW8S; masterNetIp=; guide_info=^%^7B^%^22wan_proto^%^22^%^3A^%^22dhcp^%^22^%^2C^%^22wan_ip^%^22^%^3A^%^22^%^22^%^2C^%^22wan_netmask^%^22^%^3A^%^22^%^22^%^2C^%^22wan_gateway^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_name^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22dns1^%^22^%^3A^%^22^%^22^%^2C^%^22dns2^%^22^%^3A^%^22^%^22^%^2C^%^22radio^%^22^%^3Atrue^%^2C^%^22ssid^%^22^%^3A^%^22ezviz_03da00^%^22^%^2C^%^22is_broadcast^%^22^%^3Atrue^%^2C^%^22akm^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22ssid_5g^%^22^%^3A^%^22ezviz_03da00_5G^%^22^%^2C^%^22is_broadcast_5g^%^22^%^3Atrue^%^2C^%^22akm_5g^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd_5g^%^22^%^3A^%^22^%^22^%^2C^%^22wifi_mode_type^%^22^%^3A^%^22class_3^%^22^%^7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null" -H "Accept-Encoding: gzip, deflate, sdch" -H "Accept-Language: zh-CN,zh;q=0.8" -H "Authorization: lepgVFnzerbqnNKlYaACsFinIvMW8S" -H "Accept: application/json, text/javascript, */*; q=0.01" -H "Referer: http://192.168.7.1/html/status/status.html" -H "X-Requested-With: XMLHttpRequest" -H "Connection: keep-alive" -H "User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36" --compressed

GET /get_wireless_acl.json HTTP/1.1
Host: 192.168.7.1
Connection: keep-alive
Accept: application/json, text/javascript, */*; q=0.01
X-Requested-With: XMLHttpRequest
User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36
Authorization: lepgVFnzerbqnNKlYaACsFinIvMW8S
Referer: http://192.168.7.1/html/status/status.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8
Cookie: sn=230921016; Authorization=lepgVFnzerbqnNKlYaACsFinIvMW8S; masterNetIp=; guide_info=%7B%22wan_proto%22%3A%22dhcp%22%2C%22wan_ip%22%3A%22%22%2C%22wan_netmask%22%3A%22%22%2C%22wan_gateway%22%3A%22%22%2C%22ppp_name%22%3A%22%22%2C%22ppp_pwd%22%3A%22%22%2C%22dns1%22%3A%22%22%2C%22dns2%22%3A%22%22%2C%22radio%22%3Atrue%2C%22ssid%22%3A%22ezviz_03da00%22%2C%22is_broadcast%22%3Atrue%2C%22akm%22%3A%22open%22%2C%22ssid_pwd%22%3A%22%22%2C%22ssid_5g%22%3A%22ezviz_03da00_5G%22%2C%22is_broadcast_5g%22%3Atrue%2C%22akm_5g%22%3A%22open%22%2C%22ssid_pwd_5g%22%3A%22%22%2C%22wifi_mode_type%22%3A%22class_3%22%7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null

{
	"result":	{
		"hik_restrict_mode":	"deny",
		"hik_restrict_macs":	"",
		"hik_restrict_macs_ext":	[]
	},
	"status":	{
		"ret_code":	0,
		"err_reason":	""
	}
}
或
{
	"result":	{
		"hik_restrict_mode":	"deny",
		"hik_restrict_macs":	"C4:6A:B7:48:84:FD",
		"hik_restrict_macs_ext":	["MI2-xiaomishouji"]
	},
	"status":	{
		"ret_code":	0,
		"err_reason":	""
	}
}

8、http://192.168.7.1/set_wireless_acl.cgi
curl "http://192.168.7.1/set_wireless_acl.cgi" -H "Authorization: 53RzmL4hNLU0AuXoShf9yDi440vfAL" -H "Origin: http://192.168.7.1" -H "Accept-Encoding: gzip, deflate" -H "Accept-Language: zh-CN,zh;q=0.8" -H "User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36" -H "Content-Type: application/x-www-form-urlencoded; charset=UTF-8" -H "Accept: application/json, text/javascript, */*; q=0.01" -H "Referer: http://192.168.7.1/html/status/status.html" -H "X-Requested-With: XMLHttpRequest" -H "Cookie: masterNetIp=; guide_info=^%^7B^%^22wan_proto^%^22^%^3A^%^22dhcp^%^22^%^2C^%^22wan_ip^%^22^%^3A^%^22^%^22^%^2C^%^22wan_netmask^%^22^%^3A^%^22^%^22^%^2C^%^22wan_gateway^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_name^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22dns1^%^22^%^3A^%^22^%^22^%^2C^%^22dns2^%^22^%^3A^%^22^%^22^%^2C^%^22radio^%^22^%^3Atrue^%^2C^%^22ssid^%^22^%^3A^%^22ezviz_03da00^%^22^%^2C^%^22is_broadcast^%^22^%^3Atrue^%^2C^%^22akm^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22ssid_5g^%^22^%^3A^%^22ezviz_03da00_5G^%^22^%^2C^%^22is_broadcast_5g^%^22^%^3Atrue^%^2C^%^22akm_5g^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd_5g^%^22^%^3A^%^22^%^22^%^2C^%^22wifi_mode_type^%^22^%^3A^%^22class_3^%^22^%^7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null; sn=230921016; Authorization=53RzmL4hNLU0AuXoShf9yDi440vfAL" -H "Connection: keep-alive" --data "hik_restrict_mode=deny^&hik_restrict_macs=C4^%^3A6A^%^3AB7^%^3A48^%^3A84^%^3AFD^&hik_restrict_macs_ext=^%^5B^%^22MI2-xiaomishouji^%^22^%^5D" --compressed
或
curl "http://192.168.7.1/set_wireless_acl.cgi" -H "Authorization: 53RzmL4hNLU0AuXoShf9yDi440vfAL" -H "Origin: http://192.168.7.1" -H "Accept-Encoding: gzip, deflate" -H "Accept-Language: zh-CN,zh;q=0.8" -H "User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36" -H "Content-Type: application/x-www-form-urlencoded; charset=UTF-8" -H "Accept: application/json, text/javascript, */*; q=0.01" -H "Referer: http://192.168.7.1/html/status/status.html" -H "X-Requested-With: XMLHttpRequest" -H "Cookie: masterNetIp=; guide_info=^%^7B^%^22wan_proto^%^22^%^3A^%^22dhcp^%^22^%^2C^%^22wan_ip^%^22^%^3A^%^22^%^22^%^2C^%^22wan_netmask^%^22^%^3A^%^22^%^22^%^2C^%^22wan_gateway^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_name^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22dns1^%^22^%^3A^%^22^%^22^%^2C^%^22dns2^%^22^%^3A^%^22^%^22^%^2C^%^22radio^%^22^%^3Atrue^%^2C^%^22ssid^%^22^%^3A^%^22ezviz_03da00^%^22^%^2C^%^22is_broadcast^%^22^%^3Atrue^%^2C^%^22akm^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22ssid_5g^%^22^%^3A^%^22ezviz_03da00_5G^%^22^%^2C^%^22is_broadcast_5g^%^22^%^3Atrue^%^2C^%^22akm_5g^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd_5g^%^22^%^3A^%^22^%^22^%^2C^%^22wifi_mode_type^%^22^%^3A^%^22class_3^%^22^%^7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null; sn=230921016; Authorization=53RzmL4hNLU0AuXoShf9yDi440vfAL" -H "Connection: keep-alive" --data "hik_restrict_mode=deny&hik_restrict_macs=&hik_restrict_macs_ext=" --compressed

POST /set_wireless_acl.cgi HTTP/1.1
Host: 192.168.7.1
Connection: keep-alive
Content-Length: 119
Authorization: 53RzmL4hNLU0AuXoShf9yDi440vfAL
Origin: http://192.168.7.1
User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36
Content-Type: application/x-www-form-urlencoded; charset=UTF-8
Accept: application/json, text/javascript, */*; q=0.01
X-Requested-With: XMLHttpRequest
Referer: http://192.168.7.1/html/status/status.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8
Cookie: masterNetIp=; guide_info=%7B%22wan_proto%22%3A%22dhcp%22%2C%22wan_ip%22%3A%22%22%2C%22wan_netmask%22%3A%22%22%2C%22wan_gateway%22%3A%22%22%2C%22ppp_name%22%3A%22%22%2C%22ppp_pwd%22%3A%22%22%2C%22dns1%22%3A%22%22%2C%22dns2%22%3A%22%22%2C%22radio%22%3Atrue%2C%22ssid%22%3A%22ezviz_03da00%22%2C%22is_broadcast%22%3Atrue%2C%22akm%22%3A%22open%22%2C%22ssid_pwd%22%3A%22%22%2C%22ssid_5g%22%3A%22ezviz_03da00_5G%22%2C%22is_broadcast_5g%22%3Atrue%2C%22akm_5g%22%3A%22open%22%2C%22ssid_pwd_5g%22%3A%22%22%2C%22wifi_mode_type%22%3A%22class_3%22%7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null; sn=230921016; Authorization=53RzmL4hNLU0AuXoShf9yDi440vfAL

{
	"result":	{
},
	"status":	{
		"ret_code":	0,
		"err_reason":	""
	}
}



9、http://192.168.7.1/get_light_stat.json
curl "http://192.168.7.1/get_light_stat.json" -H "Cookie: masterNetIp=; guide_info=^%^7B^%^22wan_proto^%^22^%^3A^%^22dhcp^%^22^%^2C^%^22wan_ip^%^22^%^3A^%^22^%^22^%^2C^%^22wan_netmask^%^22^%^3A^%^22^%^22^%^2C^%^22wan_gateway^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_name^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22dns1^%^22^%^3A^%^22^%^22^%^2C^%^22dns2^%^22^%^3A^%^22^%^22^%^2C^%^22radio^%^22^%^3Atrue^%^2C^%^22ssid^%^22^%^3A^%^22ezviz_03da00^%^22^%^2C^%^22is_broadcast^%^22^%^3Atrue^%^2C^%^22akm^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22ssid_5g^%^22^%^3A^%^22ezviz_03da00_5G^%^22^%^2C^%^22is_broadcast_5g^%^22^%^3Atrue^%^2C^%^22akm_5g^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd_5g^%^22^%^3A^%^22^%^22^%^2C^%^22wifi_mode_type^%^22^%^3A^%^22class_3^%^22^%^7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null; sn=230921016; Authorization=lepgVFnzerbqnNKlYaACsFinIvMW8S" -H "Accept-Encoding: gzip, deflate, sdch" -H "Accept-Language: zh-CN,zh;q=0.8" -H "Authorization: lepgVFnzerbqnNKlYaACsFinIvMW8S" -H "Accept: application/json, text/javascript, */*; q=0.01" -H "Referer: http://192.168.7.1/html/setting/system/light.html" -H "X-Requested-With: XMLHttpRequest" -H "Connection: keep-alive" -H "User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36" --compressed

GET /get_light_stat.json HTTP/1.1
Host: 192.168.7.1
Connection: keep-alive
Accept: application/json, text/javascript, */*; q=0.01
X-Requested-With: XMLHttpRequest
User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36
Authorization: lepgVFnzerbqnNKlYaACsFinIvMW8S
Referer: http://192.168.7.1/html/setting/system/light.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8
Cookie: masterNetIp=; guide_info=%7B%22wan_proto%22%3A%22dhcp%22%2C%22wan_ip%22%3A%22%22%2C%22wan_netmask%22%3A%22%22%2C%22wan_gateway%22%3A%22%22%2C%22ppp_name%22%3A%22%22%2C%22ppp_pwd%22%3A%22%22%2C%22dns1%22%3A%22%22%2C%22dns2%22%3A%22%22%2C%22radio%22%3Atrue%2C%22ssid%22%3A%22ezviz_03da00%22%2C%22is_broadcast%22%3Atrue%2C%22akm%22%3A%22open%22%2C%22ssid_pwd%22%3A%22%22%2C%22ssid_5g%22%3A%22ezviz_03da00_5G%22%2C%22is_broadcast_5g%22%3Atrue%2C%22akm_5g%22%3A%22open%22%2C%22ssid_pwd_5g%22%3A%22%22%2C%22wifi_mode_type%22%3A%22class_3%22%7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null; sn=230921016; Authorization=lepgVFnzerbqnNKlYaACsFinIvMW8S

{
	"result":	{
		"light_on":	true
	},
	"status":	{
		"ret_code":	0,
		"err_reason":	""
	}
}



10、http://192.168.7.1/trigger_light_switch.cgi
curl "http://192.168.7.1/trigger_light_switch.cgi" -H "Authorization: lepgVFnzerbqnNKlYaACsFinIvMW8S" -H "Origin: http://192.168.7.1" -H "Accept-Encoding: gzip, deflate" -H "Accept-Language: zh-CN,zh;q=0.8" -H "User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36" -H "Content-Type: application/x-www-form-urlencoded; charset=UTF-8" -H "Accept: application/json, text/javascript, */*; q=0.01" -H "Referer: http://192.168.7.1/html/setting/system/light.html" -H "X-Requested-With: XMLHttpRequest" -H "Cookie: masterNetIp=; guide_info=^%^7B^%^22wan_proto^%^22^%^3A^%^22dhcp^%^22^%^2C^%^22wan_ip^%^22^%^3A^%^22^%^22^%^2C^%^22wan_netmask^%^22^%^3A^%^22^%^22^%^2C^%^22wan_gateway^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_name^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22dns1^%^22^%^3A^%^22^%^22^%^2C^%^22dns2^%^22^%^3A^%^22^%^22^%^2C^%^22radio^%^22^%^3Atrue^%^2C^%^22ssid^%^22^%^3A^%^22ezviz_03da00^%^22^%^2C^%^22is_broadcast^%^22^%^3Atrue^%^2C^%^22akm^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22ssid_5g^%^22^%^3A^%^22ezviz_03da00_5G^%^22^%^2C^%^22is_broadcast_5g^%^22^%^3Atrue^%^2C^%^22akm_5g^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd_5g^%^22^%^3A^%^22^%^22^%^2C^%^22wifi_mode_type^%^22^%^3A^%^22class_3^%^22^%^7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null; sn=230921016; Authorization=lepgVFnzerbqnNKlYaACsFinIvMW8S" -H "Connection: keep-alive" --data "is_on=false" --compressed

POST /trigger_light_switch.cgi HTTP/1.1
Host: 192.168.7.1
Connection: keep-alive
Content-Length: 11
Authorization: lepgVFnzerbqnNKlYaACsFinIvMW8S
Origin: http://192.168.7.1
User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36
Content-Type: application/x-www-form-urlencoded; charset=UTF-8
Accept: application/json, text/javascript, */*; q=0.01
X-Requested-With: XMLHttpRequest
Referer: http://192.168.7.1/html/setting/system/light.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8
Cookie: masterNetIp=; guide_info=%7B%22wan_proto%22%3A%22dhcp%22%2C%22wan_ip%22%3A%22%22%2C%22wan_netmask%22%3A%22%22%2C%22wan_gateway%22%3A%22%22%2C%22ppp_name%22%3A%22%22%2C%22ppp_pwd%22%3A%22%22%2C%22dns1%22%3A%22%22%2C%22dns2%22%3A%22%22%2C%22radio%22%3Atrue%2C%22ssid%22%3A%22ezviz_03da00%22%2C%22is_broadcast%22%3Atrue%2C%22akm%22%3A%22open%22%2C%22ssid_pwd%22%3A%22%22%2C%22ssid_5g%22%3A%22ezviz_03da00_5G%22%2C%22is_broadcast_5g%22%3Atrue%2C%22akm_5g%22%3A%22open%22%2C%22ssid_pwd_5g%22%3A%22%22%2C%22wifi_mode_type%22%3A%22class_3%22%7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null; sn=230921016; Authorization=lepgVFnzerbqnNKlYaACsFinIvMW8S

{
	"result":	{
},
	"status":	{
		"ret_code":	0,
		"err_reason":	""
	}
}

11、http://192.168.7.1/trigger_quick_access_2g.cgi
curl "http://192.168.7.1/trigger_quick_access_2g.cgi" -X POST -H "Authorization: 53RzmL4hNLU0AuXoShf9yDi440vfAL" -H "Origin: http://192.168.7.1" -H "Accept-Encoding: gzip, deflate" -H "Accept-Language: zh-CN,zh;q=0.8" -H "User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36" -H "Accept: application/json, text/javascript, */*; q=0.01" -H "Referer: http://192.168.7.1/html/ipcManage/default.html" -H "X-Requested-With: XMLHttpRequest" -H "Cookie: masterNetIp=; guide_info=^%^7B^%^22wan_proto^%^22^%^3A^%^22dhcp^%^22^%^2C^%^22wan_ip^%^22^%^3A^%^22^%^22^%^2C^%^22wan_netmask^%^22^%^3A^%^22^%^22^%^2C^%^22wan_gateway^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_name^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22dns1^%^22^%^3A^%^22^%^22^%^2C^%^22dns2^%^22^%^3A^%^22^%^22^%^2C^%^22radio^%^22^%^3Atrue^%^2C^%^22ssid^%^22^%^3A^%^22ezviz_03da00^%^22^%^2C^%^22is_broadcast^%^22^%^3Atrue^%^2C^%^22akm^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22ssid_5g^%^22^%^3A^%^22ezviz_03da00_5G^%^22^%^2C^%^22is_broadcast_5g^%^22^%^3Atrue^%^2C^%^22akm_5g^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd_5g^%^22^%^3A^%^22^%^22^%^2C^%^22wifi_mode_type^%^22^%^3A^%^22class_3^%^22^%^7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null; sn=230921016; Authorization=53RzmL4hNLU0AuXoShf9yDi440vfAL" -H "Connection: keep-alive" -H "Content-Length: 0" --compressed

POST /trigger_quick_access_2g.cgi HTTP/1.1
Host: 192.168.7.1
Connection: keep-alive
Content-Length: 0
Authorization: 53RzmL4hNLU0AuXoShf9yDi440vfAL
Origin: http://192.168.7.1
User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36
Accept: application/json, text/javascript, */*; q=0.01
X-Requested-With: XMLHttpRequest
Referer: http://192.168.7.1/html/ipcManage/default.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8
Cookie: masterNetIp=; guide_info=%7B%22wan_proto%22%3A%22dhcp%22%2C%22wan_ip%22%3A%22%22%2C%22wan_netmask%22%3A%22%22%2C%22wan_gateway%22%3A%22%22%2C%22ppp_name%22%3A%22%22%2C%22ppp_pwd%22%3A%22%22%2C%22dns1%22%3A%22%22%2C%22dns2%22%3A%22%22%2C%22radio%22%3Atrue%2C%22ssid%22%3A%22ezviz_03da00%22%2C%22is_broadcast%22%3Atrue%2C%22akm%22%3A%22open%22%2C%22ssid_pwd%22%3A%22%22%2C%22ssid_5g%22%3A%22ezviz_03da00_5G%22%2C%22is_broadcast_5g%22%3Atrue%2C%22akm_5g%22%3A%22open%22%2C%22ssid_pwd_5g%22%3A%22%22%2C%22wifi_mode_type%22%3A%22class_3%22%7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null; sn=230921016; Authorization=53RzmL4hNLU0AuXoShf9yDi440vfAL

{
	"result":	{
		"trigger_result":	0
	},
	"status":	{
		"ret_code":	0,
		"err_reason":	""
	}
}

12、http://192.168.7.1/stop_quick_access_2g.cgi
curl "http://192.168.7.1/stop_quick_access_2g.cgi" -X POST -H "Authorization: 53RzmL4hNLU0AuXoShf9yDi440vfAL" -H "Origin: http://192.168.7.1" -H "Accept-Encoding: gzip, deflate" -H "Accept-Language: zh-CN,zh;q=0.8" -H "User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36" -H "Accept: application/json, text/javascript, */*; q=0.01" -H "Referer: http://192.168.7.1/html/ipcManage/default.html" -H "X-Requested-With: XMLHttpRequest" -H "Cookie: masterNetIp=; guide_info=^%^7B^%^22wan_proto^%^22^%^3A^%^22dhcp^%^22^%^2C^%^22wan_ip^%^22^%^3A^%^22^%^22^%^2C^%^22wan_netmask^%^22^%^3A^%^22^%^22^%^2C^%^22wan_gateway^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_name^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22dns1^%^22^%^3A^%^22^%^22^%^2C^%^22dns2^%^22^%^3A^%^22^%^22^%^2C^%^22radio^%^22^%^3Atrue^%^2C^%^22ssid^%^22^%^3A^%^22ezviz_03da00^%^22^%^2C^%^22is_broadcast^%^22^%^3Atrue^%^2C^%^22akm^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22ssid_5g^%^22^%^3A^%^22ezviz_03da00_5G^%^22^%^2C^%^22is_broadcast_5g^%^22^%^3Atrue^%^2C^%^22akm_5g^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd_5g^%^22^%^3A^%^22^%^22^%^2C^%^22wifi_mode_type^%^22^%^3A^%^22class_3^%^22^%^7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null; sn=230921016; Authorization=53RzmL4hNLU0AuXoShf9yDi440vfAL" -H "Connection: keep-alive" -H "Content-Length: 0" --compressed

POST /stop_quick_access_2g.cgi HTTP/1.1
Host: 192.168.7.1
Connection: keep-alive
Content-Length: 0
Authorization: 53RzmL4hNLU0AuXoShf9yDi440vfAL
Origin: http://192.168.7.1
User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36
Accept: application/json, text/javascript, */*; q=0.01
X-Requested-With: XMLHttpRequest
Referer: http://192.168.7.1/html/ipcManage/default.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8
Cookie: masterNetIp=; guide_info=%7B%22wan_proto%22%3A%22dhcp%22%2C%22wan_ip%22%3A%22%22%2C%22wan_netmask%22%3A%22%22%2C%22wan_gateway%22%3A%22%22%2C%22ppp_name%22%3A%22%22%2C%22ppp_pwd%22%3A%22%22%2C%22dns1%22%3A%22%22%2C%22dns2%22%3A%22%22%2C%22radio%22%3Atrue%2C%22ssid%22%3A%22ezviz_03da00%22%2C%22is_broadcast%22%3Atrue%2C%22akm%22%3A%22open%22%2C%22ssid_pwd%22%3A%22%22%2C%22ssid_5g%22%3A%22ezviz_03da00_5G%22%2C%22is_broadcast_5g%22%3Atrue%2C%22akm_5g%22%3A%22open%22%2C%22ssid_pwd_5g%22%3A%22%22%2C%22wifi_mode_type%22%3A%22class_3%22%7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null; sn=230921016; Authorization=53RzmL4hNLU0AuXoShf9yDi440vfAL

{"status":{"ret_code":0,"err_reason":"ok"}}

13、http://192.168.7.1/get_remain_scan_time.json
curl "http://192.168.7.1/get_remain_scan_time.json" -H "Cookie: masterNetIp=; guide_info=^%^7B^%^22wan_proto^%^22^%^3A^%^22dhcp^%^22^%^2C^%^22wan_ip^%^22^%^3A^%^22^%^22^%^2C^%^22wan_netmask^%^22^%^3A^%^22^%^22^%^2C^%^22wan_gateway^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_name^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22dns1^%^22^%^3A^%^22^%^22^%^2C^%^22dns2^%^22^%^3A^%^22^%^22^%^2C^%^22radio^%^22^%^3Atrue^%^2C^%^22ssid^%^22^%^3A^%^22ezviz_03da00^%^22^%^2C^%^22is_broadcast^%^22^%^3Atrue^%^2C^%^22akm^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22ssid_5g^%^22^%^3A^%^22ezviz_03da00_5G^%^22^%^2C^%^22is_broadcast_5g^%^22^%^3Atrue^%^2C^%^22akm_5g^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd_5g^%^22^%^3A^%^22^%^22^%^2C^%^22wifi_mode_type^%^22^%^3A^%^22class_3^%^22^%^7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null; sn=230921016; Authorization=53RzmL4hNLU0AuXoShf9yDi440vfAL" -H "Accept-Encoding: gzip, deflate, sdch" -H "Accept-Language: zh-CN,zh;q=0.8" -H "Authorization: 53RzmL4hNLU0AuXoShf9yDi440vfAL" -H "Accept: application/json, text/javascript, */*; q=0.01" -H "Referer: http://192.168.7.1/html/ipcManage/default.html" -H "X-Requested-With: XMLHttpRequest" -H "Connection: keep-alive" -H "User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36" --compressed

GET /get_remain_scan_time.json HTTP/1.1
Host: 192.168.7.1
Connection: keep-alive
Accept: application/json, text/javascript, */*; q=0.01
X-Requested-With: XMLHttpRequest
User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36
Authorization: 53RzmL4hNLU0AuXoShf9yDi440vfAL
Referer: http://192.168.7.1/html/ipcManage/default.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8
Cookie: masterNetIp=; guide_info=%7B%22wan_proto%22%3A%22dhcp%22%2C%22wan_ip%22%3A%22%22%2C%22wan_netmask%22%3A%22%22%2C%22wan_gateway%22%3A%22%22%2C%22ppp_name%22%3A%22%22%2C%22ppp_pwd%22%3A%22%22%2C%22dns1%22%3A%22%22%2C%22dns2%22%3A%22%22%2C%22radio%22%3Atrue%2C%22ssid%22%3A%22ezviz_03da00%22%2C%22is_broadcast%22%3Atrue%2C%22akm%22%3A%22open%22%2C%22ssid_pwd%22%3A%22%22%2C%22ssid_5g%22%3A%22ezviz_03da00_5G%22%2C%22is_broadcast_5g%22%3Atrue%2C%22akm_5g%22%3A%22open%22%2C%22ssid_pwd_5g%22%3A%22%22%2C%22wifi_mode_type%22%3A%22class_3%22%7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null; sn=230921016; Authorization=53RzmL4hNLU0AuXoShf9yDi440vfAL

{
	"result":	{
		"remain_scan_time":	0
	},
	"status":	{
		"ret_code":	0,
		"err_reason":	""
	}
}

14、http://192.168.7.1/get_ipc_info.json
curl "http://192.168.7.1/get_ipc_info.json" -H "Cookie: masterNetIp=; guide_info=^%^7B^%^22wan_proto^%^22^%^3A^%^22dhcp^%^22^%^2C^%^22wan_ip^%^22^%^3A^%^22^%^22^%^2C^%^22wan_netmask^%^22^%^3A^%^22^%^22^%^2C^%^22wan_gateway^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_name^%^22^%^3A^%^22^%^22^%^2C^%^22ppp_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22dns1^%^22^%^3A^%^22^%^22^%^2C^%^22dns2^%^22^%^3A^%^22^%^22^%^2C^%^22radio^%^22^%^3Atrue^%^2C^%^22ssid^%^22^%^3A^%^22ezviz_03da00^%^22^%^2C^%^22is_broadcast^%^22^%^3Atrue^%^2C^%^22akm^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd^%^22^%^3A^%^22^%^22^%^2C^%^22ssid_5g^%^22^%^3A^%^22ezviz_03da00_5G^%^22^%^2C^%^22is_broadcast_5g^%^22^%^3Atrue^%^2C^%^22akm_5g^%^22^%^3A^%^22open^%^22^%^2C^%^22ssid_pwd_5g^%^22^%^3A^%^22^%^22^%^2C^%^22wifi_mode_type^%^22^%^3A^%^22class_3^%^22^%^7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null; sn=230921016; Authorization=53RzmL4hNLU0AuXoShf9yDi440vfAL" -H "Accept-Encoding: gzip, deflate, sdch" -H "Accept-Language: zh-CN,zh;q=0.8" -H "Authorization: 53RzmL4hNLU0AuXoShf9yDi440vfAL" -H "Accept: application/json, text/javascript, */*; q=0.01" -H "Referer: http://192.168.7.1/html/ipcManage/default.html" -H "X-Requested-With: XMLHttpRequest" -H "Connection: keep-alive" -H "User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36" --compressed

GET /get_ipc_info.json HTTP/1.1
Host: 192.168.7.1
Connection: keep-alive
Accept: application/json, text/javascript, */*; q=0.01
X-Requested-With: XMLHttpRequest
User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36
Authorization: 53RzmL4hNLU0AuXoShf9yDi440vfAL
Referer: http://192.168.7.1/html/ipcManage/default.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8
Cookie: masterNetIp=; guide_info=%7B%22wan_proto%22%3A%22dhcp%22%2C%22wan_ip%22%3A%22%22%2C%22wan_netmask%22%3A%22%22%2C%22wan_gateway%22%3A%22%22%2C%22ppp_name%22%3A%22%22%2C%22ppp_pwd%22%3A%22%22%2C%22dns1%22%3A%22%22%2C%22dns2%22%3A%22%22%2C%22radio%22%3Atrue%2C%22ssid%22%3A%22ezviz_03da00%22%2C%22is_broadcast%22%3Atrue%2C%22akm%22%3A%22open%22%2C%22ssid_pwd%22%3A%22%22%2C%22ssid_5g%22%3A%22ezviz_03da00_5G%22%2C%22is_broadcast_5g%22%3Atrue%2C%22akm_5g%22%3A%22open%22%2C%22ssid_pwd_5g%22%3A%22%22%2C%22wifi_mode_type%22%3A%22class_3%22%7D; guide=assocIpc; 230921016guide=null; 230921016guide_info=null; sn=230921016; Authorization=53RzmL4hNLU0AuXoShf9yDi440vfAL

{"result":{"ipc_info":[]},"status":{"ret_code":0,"err_reason":"ok"}}

15、




