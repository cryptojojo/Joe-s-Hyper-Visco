# -*- coding: utf-8 -*-
"""
Created on Tue August 03  2016
GUI developed with python and the library matplotlib to display in real time the curve fitting performed
using Isight and the C++ calibration tool

The program was compile to a .exe file using pyinstaller script

Note: The program is sensitive to the format of the test data files. As a recomendation, when exporting the data
from excel, format all the cells to have the same number of digits.

@author: MOA3
"""
# -*- coding: utf-8 -*-

#Libraries
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import numpy as np
import Tkinter
import FileDialog


#Deactive options dialog on the GUI
plt.rcParams['toolbar'] = 'None'



#Open Data Suite to get the data files names
data_files=open('Data_Suite.txt','r')
td=data_files.readlines()
tt=[]
tt_exp=[]
tt_test=[]

for line in td:
	if not line.strip():
		continue
	else:
		tt.append(line.split('.')[0]+'_sim.txt')
		tt_exp.append(line.split('.')[0])
		tt_test.append(line.split('\n')[0])

#Number of data files
files_n=len(tt) 

#Save the number of data files
f_sizes=[]

#Configure the plot
fig = plt.figure(facecolor='white')
ax1 = fig.add_subplot(1,1,1)
plt.ylabel('Stress')
plt.xlabel('Strain')
fig.canvas.set_window_title('Hyper-Viscoelastic Calibration')

#Define a list of colors
colors = ['red', 'black', 'blue', 'brown', 'green','cyan','magenta','orangered','indianred','gold','darkred','indigo']

#Axis max limit variable
x_max=0
y_max=0



#Animation function to update the graph	
def animate(i):
	ax1.clear()
	for index, item in enumerate(tt):
		xs_data = []
		xs = []
		ys_data = []
		yss=[]
		a=0
		file_data=open(tt_test[index],'r')
		pos=0
		
		#skip the header lines
		file_data.next()
		file_data.next()
		file_data.next()
		
		#Plot original data
		for line in iter(file_data):
			if not line.strip():
				continue
			else:
				xs_data.append(line.split('\t')[1])
				ys_data.append(line.split('\t')[2])
			a+=1
		file_data.close()
		
		file_r=open(tt[index],'r')
		file_r.next()	
		for line in iter(file_r):
			if not line.strip():
				continue
			else:
					
				xs.append(line.split(',')[1])
				yss.append(line.split(',')[3])
						
		file_r.close()	
		
		ax1.set_xlim(0,float(x_max)*1.1)
		ax1.set_ylim(0,float(y_max)*1.1)
		
		ax1.set_xticks(np.arange(0,float(x_max)*1.1,0.1))
		ax1.set_yticks(np.arange(0,float(y_max)*1.1,0.2))
		
		plt.grid()
		ax1.plot(xs_data, ys_data,markeredgecolor=colors[index],label=tt_exp[index],marker='o',linestyle='none',markersize=4,markerfacecolor='none',markeredgewidth=1)
		ax1.plot(xs, yss,color=colors[index],label='Simulation',linewidth=2.5)
		
		ax1.legend(fontsize = 'small').get_frame().set_alpha(0.5)
		plt.ylabel('Stress')
		plt.xlabel('Strain')


# Sequence to plot the initial data when the GUI is initialized
for index, item in enumerate(tt):
	xs = []
	ys = []
	yss=[]
	file_r=open(tt[index],'r')
	pos=0
	
	file_r.next() #skip first line
	for line in iter(file_r):
			if not line.strip():
				continue
			else:
				xs.append(line.split(',')[1])
				ys.append(line.split(',')[2])
				yss.append(line.split(',')[3])
				
		
	file_r.close()
	#Set the axis dynamically	
	if x_max<xs[-1]:
		#print 'Aq'
		x_max=xs[-1]
		##print x_max
	if y_max<ys[-1]:
		y_max=ys[-1]	
	##
	ax1.set_xlim(0,float(x_max)*1.1)
	ax1.set_ylim(0,float(y_max)*1.1)
	ax1.set_xticks(np.arange(0,float(x_max)*1.1,0.1))
	ax1.set_yticks(np.arange(0,float(y_max)*1.1,0.2))
	plt.grid()
	ax1.plot(xs, ys,markeredgecolor=colors[index],label=tt_exp[index],marker='o',linestyle='none',markersize=4,markerfacecolor='none',markeredgewidth=1)
	ax1.plot(xs, yss,color=colors[index],label='Simulation',linewidth=2.5)
	ax1.legend(fontsize = 'small').get_frame().set_alpha(0.5)
	
	file_r.close()


#Animation routine to update the graphs every 1s
ani = animation.FuncAnimation(fig, animate, interval=1000)
plt.show()



