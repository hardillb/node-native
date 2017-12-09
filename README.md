#An example native NodeJS module

A basic NodeJS Native module using nan to show how to make callbacks from native code while 
maintaing the wrapper objects context.

##Description

This node module simulates a asynchronous process that reports status regularly using an 
event emitter pattern.

You can test it using the `test.js` script. This starts the service, lets it run for 
5 seconds then shuts it down.