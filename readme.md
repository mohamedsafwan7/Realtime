# Soil Moisture Realtime Dashboard

Realtime soil moisture dashboard. An Arduino reads soil moisture and controls a watering pump, a Node.js bridge forwards the serial data over WebSocket, and a Vite frontend displays live numbers.

```
Arduino → USB Serial → Node.js bridge (WebSocket) → Vite dashboard
```

## Prerequisites

- Node.js 18+ installed
- Arduino connected over USB with the sketch uploaded
- Arduino IDE Serial Monitor **closed** (only one program can hold the serial port)

## Setup

Install dependencies:

```bash
npm install
```

## Configure the serial port

The serial port name differs per OS. Find yours:

```bash
# macOS
ls /dev/cu.*

# Linux
ls /dev/ttyUSB* /dev/ttyACM*

# Windows: check Device Manager → Ports (COM & LPT), e.g. COM3
```

Then set the `path` in `server.js` to match (e.g. `/dev/cu.usbmodem14201`, `/dev/ttyUSB0`, or `COM3`).

## Run

Start the WebSocket bridge server:

```bash
node server.js
```

You should see:

```
WS server on ws://localhost:8080
```

In a separate terminal, start the Vite dev server:

```bash
npm run dev
```

Open the URL Vite prints (usually http://localhost:5173) to view the dashboard.

## Troubleshooting

- **`No such file or directory, cannot open ...`** — wrong serial port path. Re-run the `ls` command above and update `server.js`.
- **`Resource busy`** — another program holds the port. Close the Arduino IDE Serial Monitor.
- **No port found at all** — try a different USB cable (some are charge-only), and for clone boards install the matching USB-serial driver (CH340, CP210x, or FTDI).