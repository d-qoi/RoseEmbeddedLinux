#!/usr/bin/env node

var b = require('bonescript');
var led = 'USR0'

b.pinMode(led, b.OUTPUT)

var state = 0;


setInterval(toggle, 500);

function toggle() {
    if(state == 0) {
        state = 1;
    } else {
        state = 0;
    }
    b.digitalWrite(led, state);
}