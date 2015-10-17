OpenGL Demo
=======================

What is it
-----------------------

This demo is using morden OpenGL(3.3 or later) and with the purpose of understanding shaders.

And this demo is anticipating to be a :
	
- render enging
	
- partical system.

Feature
-----------------------

Here's object that integrated:

- LoadShader
	
	Load shaders into program.

- Demobase

	A class to figure things which a OpenGL application always do.

Planing
------------------------

10/08/2015 :

	finish loadShader class(completed).

10/09/2015 ： 
	
	LoadShader has run time null pointer, need fix it(fixed).
10/13/2015 ：

	Implement some to test shader loading stream.
10/16/2015 :

	- Needs to separate glLinkPragram out from LoadShader func so that we can use glBindAttribLocation(Done).


	- A little confuse about base class, need to think about which interface is indispensable.

		- Found that Function Render in mApp is not called, needs fix(application appears white)!


