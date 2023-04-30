# -*- coding: utf-8 -*-
# @Author: Maicon-Asus
# @Date:   2018-03-09 14:58:38
# @Last Modified by:   Maicon-Asus
# @Last Modified time: 2018-04-01 11:24:08
from subprocess import Popen, PIPE



class SearchPathAutoRun:		
	
	def ExecFile(self,filename,):
		process = Popen(filename, stdout=PIPE)
		(output, err) = process.communicate()		
		exit_code = process.wait()		
		return (exit_code)
	

