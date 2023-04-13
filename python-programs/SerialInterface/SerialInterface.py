import serial
import time

# ser = serial.Serial('COM4', 9600)


# message = ser.readline()
# message = message.decode().strip()
# header, parsed_data = process_message(message)
# print(header, parsed_data)


class SerialInterface:
    
    def __init__(self, port, baud_rate) -> None:
        '''
        Initializes the IMU class.
        args:
        - ser: An initalized serial object which can communicate with the IMU
        '''
        self.serial = serial.Serial(port, baud_rate)
        self.printSerial = False
        
        return
    

    def process_message(self, message : str):
        '''
        Sorts and parses data.
        args:
        - message: A string that stores header and parsed data.
        '''
        if(len(message) == 0): 
            return "", []
        if(message[0] != '!'):
            return "", []
        if(message[len(message)-1] != ';'):
            return "", []
        
        parsed_data = []
        token = ""
        for letter in message[1:]:
            if letter == ';':
                parsed_data.append(token)
                break
            if letter == ',':
                parsed_data.append(token)
                token = ""
                continue
            token = token+letter
        header = parsed_data[0]
        parsed_data = parsed_data[1:]
        for i,num in enumerate(parsed_data):
            try:
                parsed_data[i] = float(num)
            except:
                pass
        
        return header, parsed_data

    def getPose(self):
        message = ""
        self.serial.write(b'!1;\n')
        # keep reading until a ';' is found or the timeout is reached
        timer = time.time()
        while message.find("!MTR,") == -1 and time.time()-timer < 1:
            message = self.serial.readline().decode('UTF-8').strip()
        # parse the serial into a header and list
        header, data = self.process_message(message)
        if self.printSerial:
            print(message)
        
        if(header != "MTR"):
            return []
        
        return data
    
    def setTargetPose(self, turn_vel, forward_vel):
        # message = "!2,"+str(x)+","+str(y)+","+str(theta * 180 / 3.14159265359)+";\n"
        message = "!2," +str(turn_vel)+","+str(forward_vel)+";\n"
        # keep writing until the response MTR_WRT is recieved or the timeout is reached
        timer = time.time()
        return_msg = ""
        while return_msg.find(";") == -1 and time.time()-timer < 1:
            self.serial.write(message.encode('UTF-8'))
            return_msg = self.serial.readline().decode('UTF-8').strip()
        
        if self.printSerial:
            print(message)
        
    
    def setGains(self, k_rho, k_alpha, k_beta):
        message = "!3,"+str(k_rho*1000)+","+str(k_alpha*1000)+","+str(k_beta*1000)+";\n"
        # keep writing until the response CONSTWRT is recieved or the timeout is reached
        timer = time.time()
        return_msg = ""
        while return_msg.find("CONSTWRT") != -1 and time.time()-timer < 1:
            self.serial.write(message.encode('UTF-8'))
            return_msg = self.serial.readline().decode('UTF-8').strip()
        
        if self.printSerial:
            print(message)
    


if __name__ == "__main__":
    ser = SerialInterface('/dev/ttyUSB0', 115200)
    
    print(ser.getPose())
    ser.setTargetPose(100, 0, 90)
    print(ser.getPose())

        