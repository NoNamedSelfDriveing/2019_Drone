from header import *
from PyQt5.QtCore import QTimer, Qt
from PyQt5 import QtGui, QtWidgets, QtCore
from PyQt5.QtGui import *
import sys
from PyQt5.QtWidgets import *
from PyQt5 import uic

form_class = uic.loadUiType("GCS.ui")[0]

class MyWindow(QMainWindow, form_class):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.setWindowIcon(QIcon("image\drone_icon.png"))
        
        self.RpyTableWidget.resizeColumnsToContents()        
        self.RpyTableWidget.horizontalHeader().setStretchLastSection(True)

        self.SettingComboBox.setEditable(True)
        self.SettingComboBox.lineEdit().setAlignment(Qt.AlignCenter)
        
    def comboBoxEvent(self):
        self.SettingComboBox.currentIndexChanged.connect(self.changeIndex)

    def changeIndex(self):
        a = self.SettingComboBox.currentIndex()
        self.SettingStacked.setCurrentIndex(a)

    def startEvent(self):
        self.timer = QTimer()
        self.timer.timeout.connect(self.goToMain)
        self.timer.start(2000)

    def goToMain(self):
        self.WindowStackedWidget.setCurrentIndex(1)

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    
    N_GCS = MyWindow()

    N_GCS.startEvent()
    N_GCS.comboBoxEvent()
    
    N_GCS.show()

    sys.exit(app.exec_())