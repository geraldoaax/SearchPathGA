# -*- coding: utf-8 -*-
# @Author: Maicon-Asus
# @Date:   2018-03-09 14:03:42
# @Last Modified by:   Maicon-Asus
# @Last Modified time: 2018-04-01 11:51:10
from Tools import SearchPathAutoRun
from sys import platform
import os

manager = SearchPathAutoRun()

if platform == "linux" or platform == "linux2": # linux
	ExeName = "./SearchPathGA"
elif platform == "win32":# Windows...
	ExeName = "SearchPathGA.exe"

if os.path.isfile("~/Downloads/SearchPathGA_linux/log.txt"):
		os.remove("~/Downloads/SearchPathGA_linux/log.txt")

i = 5
while i<40:#c == 0:		
	print("Executando com "+str(i)+" CMs")
	for j in range(0, 10):		
		exitCode = manager.ExecFile([ExeName,"0","10",str(i)])
		if exitCode == 0:
			break
	i+=1
	
