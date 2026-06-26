import { SerialPort } from 'serialport';
import { ReadlineParser } from '@serialport/parser-readline';
import { WebSocketServer } from 'ws';

// auto-detect the Arduino port
const list = await SerialPort.list();
const found = list.find(p => /usbmodem|usbserial|wchusbserial|SLAB/i.test(p.path));
if (!found) {
  console.error('No Arduino found. Plugged in? IDE Serial Monitor closed?');
  process.exit(1);
}
console.log('Using', found.path);

const port = new SerialPort({ path: found.path, baudRate: 9600 });
const parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }));
const wss = new WebSocketServer({ port: 8080 });

let clients = [];
wss.on('connection', (ws) => {
  clients.push(ws);
  ws.on('close', () => { clients = clients.filter(c => c !== ws); });
});

parser.on('data', (line) => {
  try {
    const data = JSON.parse(line.trim());
    clients.forEach(c => c.readyState === 1 && c.send(JSON.stringify(data)));
  } catch {
    /* ignore non-JSON lines (e.g. boot noise) */
  }
});

console.log('WS server on ws://localhost:8080');