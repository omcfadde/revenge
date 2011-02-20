#!/bin/bash
# $Id$
# Copyright (C) 2007 2011  Oliver McFadden <omcfadde@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

echo -n "checking whether X is running... "
if [ -n "$DISPLAY" ]; then
	echo "yes"
else
	echo "no"
	exit 1
fi

echo -n "checking whether beryl is disabled... "
if [ -z "$(ps -e | grep '\<beryl\>')" ]; then
	echo "yes"
else
	echo "no"
	exit 1
fi

echo -n "checking whether compiz is disabled... "
if [ -z "$(ps -e | grep '\<compiz\>')" ]; then
	echo "yes"
else
	echo "no"
	exit 1
fi

echo -n "checking for fglrx... "
if [ -n "$(cat /proc/modules | grep '\<fglrx\>')" ]; then
	echo "yes"
else
	echo "no"
	exit 1
fi

echo -n "checking for fglrxinfo... "
if [ -n "$(which fglrxinfo 2> /dev/null)" ]; then
	echo "yes"
else
	echo "no"
	exit 1
fi

echo -n "checking for lspci... "
if [ -n "$(which lspci 2> /dev/null)" ]; then
	echo "yes"
else
	echo "no"
	exit 1
fi

echo -n "checking for tar... "
if [ -n "$(which tar 2> /dev/null)" ]; then
	echo "yes"
else
	echo "no"
	exit 1
fi

./configure && make && ./src/revenge $* 2>&1 | tee revenge.log
x=$?

echo
echo "Please send your report via email with the following details, making sure to"
echo "attach the relivant files; in the case of a successful dump, a tarball will be"
echo "generated. However, if there is a failure, \"revenge.log\" will be generated."
echo
echo "Both successful and failed dumps are useful! Please ensure to send them."
echo
echo "================================================================================"
echo
echo "To: Oliver McFadden <omcfadde@gmail.com>"
if [ "$x" -eq 0 ]; then
	echo "Subject: [REVENGE] $(ls revenge-*.tar.bz2)"
else
	echo "Subject: [REVENGE] revenge.log"
fi
echo
echo "You may include further details about your hardware or software configuration"
echo "in the body of this email."
echo
echo "================================================================================"
echo
echo "Thank You for helping with Free Software driver development!"
