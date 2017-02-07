from blinkstick import blinkstick
import math
import random
import time
import sys


def modifier(i,offset):
	return max(0,min(16,math.sin(i/32)))

print(blinkstick.find_all())
while(1):
	try:
		buff = open("visOut.pik","r").read()
	except:
		continue;
	#print(buff)

	nums = [float(x.replace(",",".")) for x in buff.split(" ")[:-1]]
	#nums = [(sum(nums)/len(nums)) for x in nums]
	for i in range(len(nums)):
		bstick = blinkstick.find_all()[0]
		v = float(nums[i])

		#if(v < 3):
		#	v*=6
		#bstick.set_mode(0)
		#red = (nums[i]/20*255)*modifier(i,0)
		#green = 255-((nums[i]/20*255)*modifier(i,1))
		#blue = (nums[i]/20*255)*modifier(i,2)

		#red = -5.0*(v**2)+(50*v)+150
		#green = -5.0*(v**2)+(100*v)-210
		#blue = -5.0*(v**2)+(170*v)-1170
		ran = 1#time.time()%5#random.random()*2

		redBuf = 0
		greenBuf = 7
		blueBuf = 20
		if(len(sys.argv) == 4):
			redBuf = float(sys.argv[1])
			greenBuf = float(sys.argv[2])
			blueBuf = float(sys.argv[3])

		red = math.sin(redBuf+v/3*ran) *256
		green = math.sin(greenBuf+v/3*ran) *256
		blue = math.sin(blueBuf+v/3*ran) *256

		"""
		red = 0
		green = 0
		blue = 0
		ran = random.random()
		if(random.random()>0.4):
			red = 255*v/20
		if(random.random()>0.4):
			green = 255*v/20
		if(random.random()>0.4):
			blue = 255*v/20
		if(not red and not blue and not green):
			red = blue = green = 200*v/20

		"""
		#print(red,green,blue,v)
		div = 10

		red = max(10,min(255,red*(sum(nums)/len(nums))/div))
		green = max(10,min(255,green*(sum(nums)/len(nums))/div))
		blue = max(10,min(255,blue*(sum(nums)/len(nums))/div))

		bstick.set_color(index=i,
			red=red, 
			green=green, 
			blue=blue)


