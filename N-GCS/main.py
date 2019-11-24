from header import *

form_class = uic.loadUiType("GCS.ui")[0]

class SerialReadThread(QThread):

    thread_received_data = pyqtSignal(QByteArray, name="receivedData") #사용자 지정 시그널 생성, 받은 데이터 그대로 전달 해주기 위해 QByteArray로 전달

    def __init__(self, Qserial):
        QThread.__init__(self) #QThread모듈의 초기화 함수 호출
        self.cond = QWaitCondition()
        self._status = False
        self.mutex = QMutex() # 한 번에 하나의 스레드만 엑세스 할 수 있도록 하기 위함
        self.Qserial_Thread = Qserial # QSerialport 객체 상속

    def __del__(self): # 소멸자
        self.wait()

    def run(self):
        while True:
            self.mutex.lock()
            if not self._status: #open을 통해 시리얼을 열지 못하면 false임
                self.cond.wait(self.mutex) #열때까지 기다림
            
            buf = self.Qserial_Thread.readAll()
            if buf: #들어온 데이터가 있으면
                self.thread_received_data.emit(buf) #이벤트 발생
            self.usleep(1)
            self.mutex.unlock()
            
    @pyqtSlot(bool)
    def set_status(self, status):
        self._status = status 
        if self._status: #open을 통해 시리얼을 열면 True임, 잠겨있던 스레드를 품
            self.cond.wakeAll() 

class SerialController(QMainWindow, form_class, QWidget):
    BAUDRATES = (
        QSerialPort.Baud1200,
        QSerialPort.Baud2400,
        QSerialPort.Baud4800,
        QSerialPort.Baud9600,
        QSerialPort.Baud19200,
        QSerialPort.Baud38400,
        QSerialPort.Baud57600,
        QSerialPort.Baud115200,
    )

    DATABITS = (
        QSerialPort.Data5,
        QSerialPort.Data6,
        QSerialPort.Data7,
        QSerialPort.Data8,
    )

    FLOWCONTROL = (
        QSerialPort.NoFlowControl,
        QSerialPort.HardwareControl,
        QSerialPort.SoftwareControl,
    )

    PARITY = (
        QSerialPort.NoParity,
        QSerialPort.EvenParity,
        QSerialPort.OddParity,
        QSerialPort.SpaceParity,
        QSerialPort.MarkParity,
    )

    STOPBITS = (
        QSerialPort.OneStop,
        QSerialPort.OneAndHalfStop,
        QSerialPort.TwoStop,

    )
    received_data_control = pyqtSignal(QByteArray)
    sent_data = pyqtSignal(str)

    def __init__(self, my_window):
        super().__init__()
        QWidget.__init__(self, flags=Qt.Widget)
        self.setupUi(self)
        #시리얼 인스턴스 생성
        #시리얼 스레드 설정 및 시작
        self.Qserial = QSerialPort()
        self.serial_info = QSerialPortInfo()
        
        self.PortComboBox = my_window.PortComboBox
        self.BaudComboBox = my_window.BaudComboBox
        self.DataComboBox = my_window.DataComboBox
        self.FlowComboBox = my_window.FlowComboBox
        self.ParityComboBox = my_window.ParityComboBox
        self.StopComboBox = my_window.StopComboBox
        self.RpyTableWidget = my_window.RpyTableWidget

       # self.serial_read_thread = SerialReadThread(self.Qserial)
       # self.serial_read_thread.start()
       # self.serial_read_thread.thread_received_data.connect(lambda v: self.received_data_control.emit(v))
        self.fill_serial_info()


    def fill_serial_info(self):
        self.PortComboBox.insertItems(0, self.get_available_port())
        self.BaudComboBox.insertItems(0, [str(x) for x in self.BAUDRATES])
        self.DataComboBox.insertItems(0, [str(x) for x in self.DATABITS])
        flow_name = {0: "None", 1: "Hardware", 2: "Software"} #값이 0, 1, 2로 저장되있기 때문에 딕셔너리로 값을 만들어준다.
        self.FlowComboBox.insertItems(0, [flow_name[x] for x in self.FLOWCONTROL])
        parity_name = {0: "None", 2: "Even", 3: "Odd", 4: "Space", 5: "Mark"}
        self.ParityComboBox.insertItems(0, [parity_name[x] for x in self.PARITY])
        stop_bits_name = {1: "1", 3: "1.5", 2: "2"}
        self.StopComboBox.insertItems(0, [stop_bits_name[x] for x in self.STOPBITS])

    def get_available_port(self):
        available_port = list()
        port_path = 'COM'

        for number in range(255):
            port_name = port_path + str(number)
            if not self.open(port_name):
                continue
            available_port.append(port_name)
            self.Qserial.close()

        # GUI에 연결 가능한 포트를 리스트로 반환 
        return available_port

    def open(self, port_name, baudrate = QSerialPort.Baud9600, data_bits = QSerialPort.Data8, flow_control = QSerialPort.NoFlowControl, parity = QSerialPort.NoParity, stop_bits = QSerialPort.OneStop):
        # 인자값으로 받은 시리얼 접속 정보를 이용하여 해당 포트를 연결
        info = QSerialPortInfo(port_name)
        self.Qserial.setPort(info)
        self.Qserial.setBaudRate(baudrate)
        self.Qserial.setDataBits(data_bits)
        self.Qserial.setFlowControl(flow_control)
        self.Qserial.setParity(parity)
        self.Qserial.setStopBits(stop_bits)

        return self.Qserial.open(QIODevice.ReadWrite)

    def connect_serial(self):
        self.serial_read_thread = SerialReadThread(self.Qserial)
        self.serial_read_thread.thread_received_data.connect(lambda v: self.received_data_control.emit(v))
        self.serial_read_thread.start()
        serial_info = {
            "port_name": self.PortComboBox.currentText(),
            "baudrate": self.BAUDRATES[self.BaudComboBox.currentIndex()],
            "data_bits": self.DATABITS[self.DataComboBox.currentIndex()],
            "flow_control": self.FLOWCONTROL[self.FlowComboBox.currentIndex()],
            "parity": self.PARITY[self.ParityComboBox.currentIndex()],
            "stop_bits": self.STOPBITS[self.StopComboBox.currentIndex()],
        }
        status = self.open(**serial_info)
        self.serial_read_thread.set_status(status)
        
        return status

    def disconnet_serial(self):
        self.serial_read_thread.__del__
        return self.Qserial.close()

    @pyqtSlot(bytes, name = "writeData")
    def write_data(self, data):
        
        if data == "R":
            requestData = '\x02' + '53' + '52' + '0' + '0' + '5B' + '\x03' 
            requestData = requestData.encode("utf-8")
            self.Qserial.writeData(requestData)
            self.Qserial.flush()
        else:  
            Rp = self.RpyTableWidget.item(0,0)
            Ri = self.RpyTableWidget.item(0,1)
            Rd = self.RpyTableWidget.item(0,2)
            
            Pp = self.RpyTableWidget.item(1,0)
            Pi = self.RpyTableWidget.item(1,1)
            Pd = self.RpyTableWidget.item(1,2)

            Yp = self.RpyTableWidget.item(2,0)
            Yi = self.RpyTableWidget.item(2,1)
            Yd = self.RpyTableWidget.item(2,2)

            arr = [[Rp, 65], [Ri, 66], [Rd, 67], [Pp, 68], [Pi, 69], [Pd, 70], [Yp, 71], [Yi, 72], [Yd, 73]]
            X = [ i[0] for i in arr ]
            Y = [ j[1] for j in arr ]
        #    X = [Rp, Ri, Rd, Pp, Pi, Pd, Yp, Yi, Yd]
        #    Y = [65, 66, 67, 68, 69, 70, 71, 72, 73]
            
            for i in range(9): 
                integer = math.trunc(float(X[i].text()))
                decimal = round((float(X[i].text()) - integer) * 10)

                integer = format(integer, '#02d')
                decimal = format(decimal, '#02d')

                _sum = 0x00
                _sum = 0x53 + Y[i] + int(integer) + int(decimal) 

                _sum = _sum & 0b11111111 # 캐리니블 버림
                checkSum = _sum ^ 0xFF # 1의보수
                checkSum = hex(checkSum+0b01) # 2의 보수
                
                data = '\x02' + '53' + hex(Y[i]).split('x')[1] + str(integer) + str(decimal) + checkSum.split('x')[1] + '\x03'
                data = data.encode("utf-8")

                self.Qserial.writeData(data) 
                self.Qserial.waitForBytesWritten()
                self.Qserial.flush()
                time.sleep(0.1)

    #'S' + 'M' + 'A' + 1 + 2 + checkSum 
    # Request Button누를시 Z문자 보냄 SerialRequestButton
    # Check Sum이 맞지 않을 경우 Y문자 받음 그리고 데이터 다시 보냄
        

   
class MyWindow(QMainWindow, form_class, QWidget):
    _time = 0

    def __init__(self):
        super().__init__()
        QWidget.__init__(self, flags=Qt.Widget)
        self.setupUi(self)
        self.setWindowIcon(QIcon("image\drone_icon.png"))
        
        self.RpyTableWidget.resizeColumnsToContents()        
        self.RpyTableWidget.horizontalHeader().setStretchLastSection(True)

        self.SettingComboBox.setEditable(True)
        self.SettingComboBox.lineEdit().setAlignment(Qt.AlignCenter)

        self.SerialConnectButton.setText("Connect")
        self.SerialSendButton.setText("Send")
        self.SerialRequestButton.setText("Request")

        self.serial_control = SerialController(self) # SerialController 객체 생성
        self.init_widget()
        self.startTimer()

    def init_widget(self):
        self.SerialConnectButton.clicked.connect(self.slot_clicked_connect_button) #연결 버튼이 클릭되면 호출
        self.serial_control.received_data_control.connect(self.read_data)
        #data = bytes([0x00]) + bytes([0x02]) + bytes("TEST DATA", "utf-8") + bytes([0x03])

        self.SerialSendButton.clicked.connect(lambda: self.serial_control.write_data("N")) # send버튼 눌를시 데이터 전송
        self.SerialRequestButton.clicked.connect(lambda: self.serial_control.write_data("R")) #Request 버튼 누를 시 Z 프로토콜 전송

        self.BaudComboBox.setCurrentIndex(7) # 9600baud
        self.DataComboBox.setCurrentIndex(3) # 8

    def comboBoxEvent(self):
        self.SettingComboBox.currentIndexChanged.connect(self.changeIndex)

    def changeIndex(self):
        a = self.SettingComboBox.currentIndex()
        self.SettingStacked.setCurrentIndex(a)

    def startTimer(self):
        self.timer = QTimer()
        self.timer.timeout.connect(self.goToMain)
        self.timer.start(2000)

    def flight_Timer(self):
        self.flightTimer = QTimer()
        self.flightTimer.timeout.connect(self.showFlightTime)
        self.flightTimer.setInterval(1000)
        self.flightTimer.start()

    def goToMain(self):
        self.WindowStackedWidget.setCurrentIndex(1)
        self.timer.stop()

    def showFlightTime(self):
        self._time += 1
        _time = self._time

        _time = time.strftime("%H:%M:%S", time.gmtime(_time))
        self.Time_Label.setText(_time)

    @pyqtSlot(QByteArray)
    def read_data(self, rd):
        print(type(rd))
        
    @pyqtSlot(name="clickedConnetButton") #시리얼 연결 버튼을 클릭하는 이벤트 발생시
    def slot_clicked_connect_button(self):
        if self.serial_control.Qserial.isOpen(): # 열려있다면 
            self.serial_control.disconnet_serial() # 닫고

            print('disconnect')
        else: #닫혀있다면 
            print("connet")           
            self.serial_control.connect_serial() #연다
        self.SerialConnectButton.setText({False: 'Connect', True: 'Disconnect'}[self.serial_control.Qserial.isOpen()])
        
        if self.serial_control.Qserial.isOpen():
            self.flight_Timer()
        else:
            print('close')
            self._time = -1
            self.showFlightTime()
            self.flightTimer.stop()
            

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)

    excepthook = sys.excepthook
    sys.excepthook = lambda t, val, tb: excepthook(t, val, tb)

    N_GCS = MyWindow()

    N_GCS.comboBoxEvent()
 

    N_GCS.showMaximized()
    
    sys.exit(app.exec_())