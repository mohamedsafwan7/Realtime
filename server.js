import { SerialPort } from 'serialport';
import { ReadlineParser } from '@serialport/parser-readline';
import { WebSocketServer } from 'ws';

const port = new SerialPort({
  path: '/dev/ttyUSB0', // Windows: 'COM3'  Mac: '/dev/tty.usbmodemXXXX'
  baudRate: 9600,
});

const parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }));
const wss = new WebSocketServer({ port: 8080 });

let clients = [];
wss.on('connection', (ws) => {
  clients.push(ws);
  ws.on('close', () => { clients = clients.filter(c => c !== ws); });
});

parser.on('data', (line) => {
  try {
    JSON.parse(line); // validate
    clients.forEach(c => c.readyState === 1 && c.send(line));
  } catch { /* ignore malformed lines */ }
});

console.log('WS server on ws://localhost:8080');