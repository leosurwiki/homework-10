# -*- coding: utf-8 -*-

"""
Module implementing main_GUI.
"""
#error enumeration
ERROR_INVALID_INPUT = 1#输入矩阵时输入了非数字元素
ERROR_OUT_OF_RANGE = 2#输入矩阵时数字大小超过了合法范围
SOLVE_CHOSEN = -10001#表示数组上的这一位被选取了
MAXM = 32#矩阵最大长度
MAXN = 32#矩阵最大宽度
from PyQt4.QtGui import * 
from PyQt4.QtCore import *
import sys
import time
import thread
from Ui_main import Ui_main_GUI


class main_GUI(QDialog, Ui_main_GUI):
    """
    main_GUI是以Ui_main_GUI为GUI样式文件的主程序
    """
    situation = []
    now_frame = 0
    frame_num = 0
    tmp = ()
    t = {}
    l = {}
    m = 0
    n = 0
    now = 0#此时的在第几个tab上
    #t,l是为了增加程序的拓展性用的，即支持几个tab格的操作由于本次程序只使用了一个tab因而只使用了t[0]，那么如果要支持几个tab呢？聪明的你能搞定吗？
    def timer(self,no,interval):  
        while self.now_frame<self.frame_num-1:  
            self.on_pushButton_3_clicked()
            time.sleep(interval) 
        thread.exit_thread()
    def matrixinput(self,main_GUI):#从UI上读入一个矩阵
        m=0
        n=0 #整张表为空时表示矩阵[0]
        result = []
        for i in range(32):
            tmp = []
            for j in range(32): 
                try:
                    tmp.append(self.t[self.now].item(i,j).text())
                except:
                    tmp.append('0')
            result.append(tmp)
        for i in range(32):
            for j in range(32): 
                try:
                    if result[i][j]=="":result[i][j]=0
                    else:result[i][j] = int(result[i][j])#int转化不成功意味着输入了非数字字符
                except:
                    return ERROR_INVALID_INPUT
                if result[i][j] < -100 or result[i][j]  > 100:
                    return ERROR_OUT_OF_RANGE
                if result[i][j]!=0 and (i>n):
                    n=i
                if result[i][j]!=0 and (j>m):
                    m=j
        matrix = []
        for i in range(n+1):
            for j in range(m+1):
                matrix.append(result[i][j])
        return [n+1,m+1,matrix]
    def repaint(self):
        for i in range(len(self.situation[0])):
            for j in range(len(self.situation[0][0])):
                if int(self.tmp[i*len(self.situation[0][0])+j]) == -10001:
                    nitem = self.t[self.now].item(i,j)
                    if nitem==None:
                        nitem = QTableWidgetItem ('0')
                    nitem.setBackgroundColor(QColor(0,215,0))
                    self.t[self.now].setItem(i,j,nitem)
        for i in range(len(self.situation[0])):
            for j in range(len(self.situation[0][0])):
                try:
                    nitem = self.t[self.now].item(i,j)
                    if nitem==None:
                        nitem = QTableWidgetItem ('0')
                        self.t[self.now].setItem(i,j,nitem)
                    if self.situation[self.now_frame][i][j]=='1':
                        nitem.setBackgroundColor(QColor(255,215,0))#黄色
                        self.t[self.now].setItem(i,j,nitem)
                    else:
                        nitem.setBackgroundColor(QColor(255,255,255))#白色
                        self.t[self.now].setItem(i,j,nitem)
                except:
                    print "error1"
                    print i,j,self.now_frame
        for i in range(len(self.situation[0])):
            for j in range(len(self.situation[0][0])):
                if int(self.tmp[i*len(self.situation[0][0])+j]) == -10001:
                    nitem = self.t[self.now].item(i,j)
                    if self.situation[self.now_frame][i][j]!='1':
                        nitem.setBackgroundColor(QColor(0,0,255))
                    else:
                        nitem.setBackgroundColor(QColor(0,255,0))
                    self.t[self.now].setItem(i,j,nitem)
    def check(self):
        self.pushButton_3.setDisabled(False)
        self.pushButton_4.setDisabled(False)
        self.pushButton_5.setDisabled(False)
        self.pushButton_6.setDisabled(False) 
        print self.now_frame,self.frame_num
        if self.now_frame==0:
            self.pushButton_4.setDisabled(True)
            self.pushButton_5.setDisabled(True)
        if self.now_frame==self.frame_num-1:
            self.pushButton_3.setDisabled(True)
            self.pushButton_6.setDisabled(True)      
        
    def history(self):
        fin = open("internal_result.dat")
        i=0
        while True:
            now_situation = []
            s=fin.readline()
            if s=="":break
            n = int(s.split(' ')[1])
            m = int(s.split(' ')[2])
            for i in range(n):
                s=fin.readline()
                line = s.split(' ')
                tmpset = []
                for j in range(m):
                    tmpset.append(line[j])
                now_situation.append(tmpset)
            self.situation.append(now_situation)
        self.frame_num=len(self.situation)

    def solve(self,n,m,c,a,h,v):
        """
        m,n表示矩阵的长宽,c表示一个矩阵但以一维状态存储,a,h,v表示计算时的要求参数amorphous,vertical,horizontal
        """
        import os
        import ctypes
        from ctypes import c_int
        lib_handleO = ctypes.cdll.LoadLibrary('./bin/original.dll')
        lib_handleA = ctypes.cdll.LoadLibrary('./bin/atype.dll')
        ans = 0
        tmp = (c_int*(n*m))()#一个临时存储c_int格式类型的数组
        for i in range(n*m):
            tmp[i] = c[i]
        ans = 0
        if a==True :
            ans = lib_handleA.deal(n,m,tmp,v,h)#调用atype.dll将tmp中所有被选取的部分置为SOLVE_CHOSEN,并把得到的最大值返回给ans
        else:
            ans = lib_handleO.maxsumblock(tmp,n,m,h,v)#调用original.dll
        time.sleep(0.5)
        self.history()
        self.now_frame=0
        self.pushButton_3.setDisabled(False)
        self.pushButton_4.setDisabled(True)
        self.pushButton_5.setDisabled(True)
        self.pushButton_6.setDisabled(False)
        self.m = m
        self.n = n
        self.tmp = tmp
        for i in range(n):
            for j in range(m):
                if int(tmp[i*m+j]) == -10001:
                    nitem = self.t[self.now].item(i,j)
                    if nitem==None:
                        nitem = QTableWidgetItem ('0')
                    nitem.setBackgroundColor(QColor(0,215,0))
                    self.t[self.now].setItem(i,j,nitem)
        self.l[self.now].setText('ans:'+str(ans))
        self.repaint()
        self.check()
        self.l[self.now].setText('ans:'+str(ans))
        
    def __init__(self, parent = None):
        """
        Constructor
        """
        QDialog.__init__(self, parent)
        self.setupUi(self)
        self.t[0] = self.tableWidget
        self.t[1] = self.tableWidget_2
        self.l[0] = self.label
        self.l[1] = self.label_2

    @pyqtSignature("")
    def on_pushButton_clicked(self):
        """
        进行计算操作的按钮
        """
        amorphous = self.checkBox_2.isChecked()
        horizontal = self.checkBox.isChecked()
        vertical = self.checkBox_3.isChecked()
        self.now = int(self.tabWidget.currentIndex())
        if (self.matrixinput(self)==ERROR_INVALID_INPUT):
            self.l[self.now].setText("Please input numbers in the table.")
            self.t[self.now].setDisabled(True)
            return 0
        if (self.matrixinput(self)==ERROR_OUT_OF_RANGE):
            self.l[self.now].setText("Please input numbers in range(-100,100) in the table.")
            self.t[self.now].setDisabled(True)
            return 0
        else:
            [n,m,matrix] = self.matrixinput(self)
        self.solve(n,m,matrix,amorphous,horizontal,vertical)
        self.t[self.now].setCurrentCell(32,32)
        self.pushButton.setDisabled(True)
    @pyqtSignature("")
    def on_pushButton_2_clicked(self):
        """
        进行重置操作的按钮
        """        
        self.situation = []
        for i in range(32):
            for j in range(32):
                self.t[self.now].setItem(i,j,None)
        self.t[self.now].setDisabled(False)  
        self.pushButton.setDisabled(False)
        self.pushButton_3.setDisabled(True)
        self.pushButton_4.setDisabled(True)
        self.pushButton_5.setDisabled(True)
        self.pushButton_6.setDisabled(True)
        self.l[self.now].setText("")
        self.now_frame = 0
        self.frame_num = 0
        self.m=0
        self.n=0 #整张表为空时表示矩阵[0]
    @pyqtSignature("")
    def on_pushButton_3_clicked(self):
        self.now_frame+=1
        self.repaint()
        self.check()
    @pyqtSignature("")
    def on_pushButton_4_clicked(self):
        self.now_frame-=1
        self.repaint()
        self.check()
    @pyqtSignature("")
    def on_pushButton_6_clicked(self):
        thread.start_new_thread(self.timer,(0,1))
    @pyqtSignature("")
    def on_pushButton_5_clicked(self):
        self.now_frame=0
        self.repaint()
        self.check()
    def on_pushButton_7_clicked(self):
        try:
            fin = open("input.txt","r")
        except:
            self.l[self.now].setText("IOerror!")
        i = 0
        while True:
            s = fin.readline()
            if s=="":break
            for j in range(len(s.split(' '))-1):
                nitem = QTableWidgetItem(s.split(' ')[j])
                self.t[self.now].setCurrentItem(None)
                self.t[self.now].setItem(i,j,nitem)
            i+=1
app = QApplication(sys.argv)  
window = main_GUI()  
window.show()  
app.exec_()  
    

