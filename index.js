const native = require('bindings')('addon.node');
const events = require('events');
const util = require('util');

module.exports = Native;

function Native() {
	events.EventEmitter.call(this);
	//passes "this" to the C++ side for callback
	this._native = new native.Test(this);
}

util.inherits(Native,events.EventEmitter);

//callback from C++ side
Native.prototype._status = function(status) {
	this.emit("loop", status.counter);
}

//start background loop
Native.prototype.start = function() {
	this._native.start(this._status);
}

//stop background loop
Native.prototype.stop = function() {
	this._native.stop();
}

