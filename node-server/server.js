const express = require("express");
const http = require("http");
const path = require("path");
const WebSocket = require("ws");

const app = express();
const port = 3000;

const server = http.createServer(app);

const wss = new WebSocket.Server({ server });

// this snippet gets the local ip of the node.js server. copy this ip to the client side code and add ':3000'
require("dns").lookup(require("os").hostname(), function (err, add, fam) {
  console.log("addr: " + add);
});

app.use(express.static(path.join(__dirname, "public")));

wss.on("connection", (ws, req, client) => {
  // add user authenticate *
  ws.on("message", (data, isBinary) => {
    console.log(`Received message ${data} from user ${client}`);

    // send data to every other connected WebSocket clients, excluding itself
    wss.clients.forEach((client) => {
      if (client !== ws && client.readyState === WebSocket.OPEN) {
        client.send(data, { binary: isBinary });
      }
    });
  });
});

server.listen(process.env.PORT || port, () => {
  // eslint-disable-next-line no-console
  console.log(`Example app listening at http://localhost:${port}`);
});
