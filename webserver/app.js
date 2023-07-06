const express = require('express');
const app = express();
const http = require('http');
const { setUncaughtExceptionCaptureCallback } = require('process');
const server = http.createServer(app);
const { Server } = require("socket.io");
const io = new Server(server, {
  cors: {
    origin: "http://localhost:8080"
  }
});

const CONFIG = require('./config.json');

let lightState = false;

app.get('/', (req, res) => {
  res.sendFile(__dirname + '/index.html');
});

io.on('connection', (socket) => {
  socket.emit('setLights', lightState);
  socket.on('beep', (msg) => {
    lightState = !lightState; // Flips whether the light is on or off
    io.emit('setLights', lightState);
  });
});

server.listen(CONFIG.PORT, () => {
  console.log(`listening on *:${CONFIG.PORT}`);
});