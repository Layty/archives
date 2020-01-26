#!/bin/sh

case "$1" in
	config-*|request-*mount|request-samba)
		exec haetool "$@"
		;;
	*)
		echo "*** Operation not permitted for '$1'."
		exit 1
		;;
esac

