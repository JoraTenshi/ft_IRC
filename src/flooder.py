import socket
import sys
import threading


def flood(sock: socket, thread_id: int = 0):
	for i in range(0, 100000):
		try:
			sock.send(f"PRIVMSG #general :Flood message {i} from thread {thread_id}. Get rekt n00bs\r\n".encode('utf-8'))
		except BrokenPipeError:
			print("Connection closed")
			break
		print(f"Message {i} sent from thread {thread_id}")
		i += 1


if __name__ == "__main__":
	if len(sys.argv) < 4:
		print("Usage: python flooder.py <ip> <port> <password>")
		exit(1)
	ip = sys.argv[1]
	port = sys.argv[2]
	passwd = sys.argv[3]
	print(f"Connecting to {ip}:{port} with password {passwd}")
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

	try:
		sock.connect((ip, int(port)))
		sock.send(f"PASS {passwd}\r\n".encode('utf-8'))
		sock.send(f"NICK flooder\r\n".encode('utf-8'))
		data = sock.recv(1024)
		print(data.decode())
		sock.send(f"USER flooder 0 * :flooder\r\n".encode('utf-8'))
		data = sock.recv(1024)
		print(data.decode())
		sock.send(f"JOIN #general\r\n".encode('utf-8'))
		data = sock.recv(1024)
		print(data.decode())
	except ConnectionRefusedError:
		print("Connection refused")
		exit(1)

	print("Connected")

	# Generate three threads to flood the server
	threads = []
	for i in range(0, 3):
		threads.append(threading.Thread(target=flood, args=(sock, i,)))
		i += 1

	for thread in threads:
		thread.start()

	for thread in threads:
		thread.join()

	sock.close()