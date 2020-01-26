module("luci.controller.welcome.index", package.seeall)

require "luci.http"
require "luci.template"
require 'luci.sys'

logfile = "/tmp/plugin_welcome"


function index()
	entry({"welcome"}, call("welcome"), nil)
	entry({"welcome", "auth"}, call("auth"), nil)
end

function welcome()
	luci.http.header("Cache-Control", "no-cache")
	luci.http.header("Pragma", "no-cache")
	luci.http.header("Expires", 0)
	luci.template.render("welcome/index")
end

function auth()
	require "io"
	clientip = luci.http.getenv("REMOTE_ADDR")
	local result = luci.sys.exec("/usr/sbin/iptables -t nat -I PREROUTING -i br-lan -p tcp -s " .. clientip .. "/32 -j ACCEPT")
	local fh = io.open(logfile, 'w+')
	fh:write(clientip)
	fh:close()
	local response = {}
	response['result'] = "ok"
	luci.http.prepare_content("application/json")
	luci.http.write_json(response)
end