import serial
import struct

def readSettings():
	serialSettings = open("../Saved/Serial/settings.txt", "r")
	serialSettingsCode = serialSettings.read()
	print(f"{serialSettingsCode}\n")
	return serialSettingsCode
	

def serialRead(_port, _baudrate, _timeout):
	ser = serial.Serial(_port, baudrate = _baudrate, timeout = _timeout)
	while 1: 
		buffer = ser.readline()
		components = buffer.decode().split("\n")
		components.remove("")
		flat_components = []

		f = open("../Saved/Serial/output.fgg", "wb")


		for i in range(len(components)):
			components[i] = components[i].split("\t")
			for j in range(len(components[i])):
				components[i][j] = components[i][j].replace("\r", "")
				if (components[i][j] == ""):
					del components[i][j]
				else:
					fl = float(components[i][j])
					f.write(bytearray(struct.pack("f", fl)) )		

		f.flush()
		f.close()


if __name__ == "__main__":
	code = readSettings()
	settings = code.split()
	serialRead(settings[0], int(settings[1]), 1)